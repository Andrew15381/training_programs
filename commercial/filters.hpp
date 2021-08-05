#ifndef FILTERS_H
#define FILTERS_H

#include <cmath>
#include <vector>
#include <map>
#include <string>
#include <memory>
#include "split.hpp"

namespace Algo
{
    struct Point
    {
        double open, high, low, close, volume;
    };

    //base filter and its factory
    //filter is a double to rank assets
    class AssetFilter
    {
        protected:
            std::string name;
            double value;
            AssetFilter(const std::string& name) :
                name(name),
                value(0.0)
            {}
        public:
            inline double get_value() const
            {
                return value;
            }
            inline std::string get_name() const
            {
                return name;
            }
            //friend bool operator>(const AssetFilter& lhs, const AssetFilter& rhs)
            virtual ~AssetFilter(){}
    };
    class AssetFilterFactory
    {
        public:
            virtual std::shared_ptr<AssetFilter> create(const std::vector<Point>& points) = 0;
            virtual ~AssetFilterFactory(){}
    };


    //expectancy filter (in percents) and its factory
    //default strategy: buy 1 asset, sell next date
    class ExpectancyFilter : public AssetFilter
    {
        public:
            ExpectancyFilter(const std::vector<Point>& points) :
                AssetFilter("expectancy_filter")
            {
                double won = 0.0, lost = 0.0;
                for(size_t i = 1; i < points.size(); ++i)
                {
                    double delta = (points[i].close / points[i - 1].close - 1) * 100.0;
                    if(delta > 0)
                    {
                        won += delta;
                    }
                    else
                    {
                        lost -= delta;
                    }
                }
                value = (won - lost) / (points.size() - 1);
            }
            friend bool operator>(const ExpectancyFilter& lhs, const ExpectancyFilter& rhs)
            {
                return lhs.value > rhs.value;
            }
    };
    class ExpectancyFilterFactory : public AssetFilterFactory
    {
        public:
            std::shared_ptr<AssetFilter> create(const std::vector<Point>& points)
            {
                return std::make_shared<ExpectancyFilter>(points);
            }
    };


    //kelly filter (in percents) and its factory
    //default strategy: buy 1 asset, sell next tick
    class KellyFilter : public AssetFilter
    {
        public:
            KellyFilter(const std::vector<Point>& points) :
                AssetFilter("kelly_filter")
            {
                double won = 0.0, lost = 0.0;
                int wins_count = 0;
                for(size_t i = 0; i < points.size() - 1; ++i)
                {
                    double delta = points[i + 1].close - points[i].close;
                    if(delta > 0)
                    {
                        ++wins_count;
                        won += delta;
                    }
                    else
                    {
                        lost -= delta;
                    }
                }
                if(won > 0.0)
                {
                    value = 100.0 * (wins_count - (points.size() - wins_count) * lost / won) / points.size();
                }
            }
            friend bool operator>(const KellyFilter& lhs, const KellyFilter& rhs)
            {
                return lhs.value > rhs.value;
            }
    };
    class KellyFilterFactory : public AssetFilterFactory
    {
        public:
            std::shared_ptr<AssetFilter> create(const std::vector<Point>& points)
            {
                return std::make_shared<KellyFilter>(points);
            }
    };

    //Stochastic dominance utils
    static std::vector<std::pair<double, double>> calculate_base_pdf(const std::map<double, double>& base_probs,
        const std::map<double, double>& probs)
    {
        std::map<double, double> base(base_probs);
        for(auto prob : probs)
        {
            prob.second = 0.0;
            base.insert(prob);
        }
        std::vector<std::pair<double, double>> base_pdf;
        base_pdf.reserve(base.size());
        for(const auto& prob : base)
        {
            base_pdf.push_back(prob);
        }
        return base_pdf;
    }

    static void integrate(std::vector<std::pair<double, double>>& f, int order)
    {
        for(int i = 0; i < order; ++i)
        {
            for(size_t j = f.size() - 1; j > 0; --j)
            {
                f[i].second = 0.5 * (f[i].second + f[i - 1].second) * (f[i].first - f[i - 1].first);
            }
            f.erase(f.begin());
            for(size_t j = 1; j < f.size(); ++j)
            {
                f[i].second += f[i - 1].second;
            }
        }
    }

    //stochastic dominance filter and its factory
    //4th-order weighted SD
    class StochasticDominanceFilter : public AssetFilter
    {
        static constexpr double alpha = 0.1;
        public:
            std::map<double, double> probs;
            StochasticDominanceFilter(const std::vector<Point>& points) :
                AssetFilter("stochdom_filter")
            {
                //returns in percents
                std::vector<double> values;
                values.reserve(points.size() - 1);
                for(size_t i = 1; i < points.size(); ++i)
                {
                    values.push_back(points[i].close / points[i - 1].close - 1.0);
                }
                //calculating probs
                const double t = 1.0 / values.size();
                for(const auto& value : values)
                {
                    probs[value] += t;
                }
                //weighing probs
                std::vector<double> weights(probs.size());
                size_t i = 0;
                for(auto probit = probs.begin(); i < weights.size(); ++i, ++probit)
                {
                    weights[i] = std::exp(-std::pow(std::log(1.0 / probit->second), alpha));
                }
                for(i = weights.size() - 1; i > 0; --i)
                {
                    weights[i] -= weights[i - 1];
                }
                i = 0;
                for(auto probit = probs.begin(); i < weights.size(); ++i, ++probit)
                {
                    probit->second *= weights[i];
                }
            }
            friend bool operator>(const StochasticDominanceFilter& lhs, const StochasticDominanceFilter& rhs)
            {
                std::vector<std::pair<double, double>> lhs_pdf_int = calculate_base_pdf(lhs.probs, rhs.probs),
                    rhs_pdf_int = calculate_base_pdf(rhs.probs, lhs.probs);
                integrate(lhs_pdf_int, 4);
                integrate(rhs_pdf_int, 4);
                double sum = 0.0;
                for(size_t i = 0; i < lhs_pdf_int.size(); ++i)
                {
                    sum += lhs_pdf_int[i].second - rhs_pdf_int[i].second;
                }
                return sum < 0.0;
            }
    };
    class StochasticDominanceFilterFactory : public AssetFilterFactory
    {
        public:
            std::shared_ptr<AssetFilter> create(const std::vector<Point>& points)
            {
                return std::make_shared<StochasticDominanceFilter>(points);
            }
    };

    //Hurst utils
    static double calculate_rs(const std::vector<double>& x)
    {
        double mean = 0.0;
        for(auto i : x)
        {
            mean += i;
        }
        mean /= x.size();

        double cum_var = x[0] - mean,
            std = cum_var * cum_var,
            min_cum_var = cum_var,
            max_cum_var = cum_var;
        for(size_t i = 1; i < x.size(); ++i)
        {
            double var = x[i] - mean;
            std += var * var;
            cum_var += var;
            if(cum_var > max_cum_var)
            {
                max_cum_var = cum_var;
            }
            else if(cum_var < min_cum_var)
            {
                min_cum_var = cum_var;
            }
        }
        std /= x.size();
        std = std::sqrt(std);

        return std != 0.0 ? (max_cum_var - min_cum_var) / std : 0.0;
    }

    static double calculate_slope(const std::vector<double>& x, const std::vector<double>& y)
    {
        double sumx = 0.0, sumy = 0.0, sumx2 = 0.0, sumxy = 0.0;
        for(size_t i = 0; i < x.size(); ++i)
        {
            sumx += x[i];
            sumy += y[i];
            sumx2 += x[i] * x[i];
            sumxy += x[i] * y[i];
        }
        return (x.size() * sumxy - sumx * sumy) / (x.size() * sumx2 - sumx * sumx);
    }

    static double calculate_hurst(const std::vector<double>& x, size_t division_depth)
    {
        std::vector<double> ranges_mean_rs(division_depth), range_sizes(division_depth);
        for(size_t i = 1, j = 0; j < division_depth; i *= 2, ++j)
        {
            for(auto range : Algo::split(x, i))
            {
                ranges_mean_rs[j] += calculate_rs(range);
            }
            range_sizes[j] = std::log(x.size() / i);
            if(ranges_mean_rs[j] != 0.0)
            {
                ranges_mean_rs[j] = std::log(ranges_mean_rs[j] / i);
            }
        }

        return calculate_slope(range_sizes, ranges_mean_rs);
    }

    //counts persistent windows percentage
    class HurstFilter : public AssetFilter
    {
        public:
            HurstFilter(const std::vector<Point>& points, size_t window_size) :
                AssetFilter("hurst_filter")
            {
                //returns in percents
                std::vector<double> returns;
                returns.reserve(points.size() - 1);
                for(size_t i = points.size() - window_size; i < points.size(); ++i)
                {
                    returns.push_back(points[i].close / points[i - 1].close - 1.0);
                }
                const size_t division_depth = std::log2(window_size);
                value = calculate_hurst(std::vector<double>(returns.end() - window_size, returns.end()), division_depth);
            }
            friend bool operator>(const HurstFilter& lhs, const HurstFilter& rhs)
            {
                return lhs.value > rhs.value;
            }
    };
    class HurstFilterFactory : public AssetFilterFactory
    {
        size_t window_size;
        public:
            HurstFilterFactory(size_t window_size) : window_size(window_size)
            {}
            std::shared_ptr<AssetFilter> create(const std::vector<Point>& points)
            {
                return std::make_shared<HurstFilter>(points, window_size);
            }
    };

    struct Asset
    {
        std::string symbol;
        time_t latest_timestamp;
        Point latest_point;
        std::vector<std::shared_ptr<AssetFilter>> filters;
    };

    class Portfolio
    {
        std::vector<std::shared_ptr<AssetFilterFactory>> filter_foctories;
        std::vector<Asset> assets;
        public:
            Portfolio(const std::vector<std::shared_ptr<AssetFilterFactory>>& filter_foctories) :
                filter_foctories(filter_foctories)
            {}
            void add_asset(const std::string& symbol, const time_t latest_timestamp, const std::vector<Point>& points)
            {
                Asset asset{symbol, latest_timestamp, points.back()};
                for(const auto& factory : filter_foctories)
                {
                    asset.filters.push_back(factory->create(points));
                }
                assets.push_back(asset);
            }
            const std::vector<Asset>& rank_by_nth_filter(size_t n)
            {
                std::sort(assets.begin(), assets.end(), [&](const auto& lhs, const auto& rhs){return lhs.filters[n] > rhs.filters[n];});
                return assets;
            }
            const std::vector<Asset>& get_assets() const
            {
                return assets;
            }
            inline int get_filters_count() const
            {
                return filter_foctories.size();
            }
    };
}

#endif
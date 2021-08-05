#include <math.h>
#include <stdio.h>
#include <string.h>

double f1(double);
double f2(double);
double f3(double);
double df1(double);
double df2(double);
double df3(double);

double f3f1(double x)
{
	return f3(x) - f1(x);
}

double f2f1(double x)
{
	return f2(x) - f1(x);
}

//iterations counter
int iterations;

#ifdef NEWTON
double root(double (*f)(double), double (*g)(double), double (*df)(double), double (*dg)(double), double a, double b, double eps1)
{
	iterations = 0;
	//x0 is set b in case F' is positive and increasing, or negative and decreasing 
    double r = ((df(a) - dg(a)) * (df(b) - dg(b) - df(a) + dg(a)) > 0.0) ? b : a;
    //F' infimum
    double inf_d = (fabs(df(a) - dg(a)) > fabs(df(b) - dg(b))) ? fabs(df(b) - dg(b)) : fabs(df(a) - dg(a));
    while(fabs(f(r) - g(r)) / inf_d >= eps1)
    {
        r = r - (f(r) - g(r)) / (df(r) - dg(r));
        ++iterations;
    }
    return r;
}
#else
double root(double (*f)(double), double (*g)(double), double (*df)(double), double (*dg)(double), double a, double b, double eps1)
{
	iterations = 0;
	//x0 is set a in case F' is positive and increasing, or negative and decreasing 
    double r = ((df(a) - dg(a)) * (df(b) - dg(b) - df(a) + dg(a)) > 0.0) ? a : b;
    //F' infimum
    double inf_d = (fabs(df(a) - dg(a)) > fabs(df(b) - dg(b))) ? fabs(df(b) - dg(b)) : fabs(df(a) - dg(a));
    while(fabs(f(r) - g(r)) / inf_d >= eps1)
    {
        r = (r == b) ? (b - (f(b) - g(b)) / (df(b) - dg(b))) : (r - (f(r) - g(r)) * (b - r) / (f(b) - g(b) - f(r) + g(r)));
        ++iterations;
    }
    return r;
}
#endif

double integral(double (*f)(double), double a, double b, double eps2)
{
	//asimptotic n is squared for better precision
    int n = 1 / eps2 / eps2;
    double sum = (f(a) + f(b)) / 2, dxk = (b - a) / n, xk = a;
    for(int k = 1; k < n; ++k)
    {
        sum += f(xk);
        xk += dxk;
    }
    return sum * dxk;
}

int main(int argc, char **argv)
{
	if(argc > 1 && (strcmp(argv[1], "-help") == 0))
	{
		printf("-help print flags\n"
			"-a print points\n"
			"-b print iterations (only if -a is enabled)\n"
			"-test-root enables root testing mode\n"
			"-test-integral enables integral testing mode\n");
		return 0;
	}

	if(argc > 1 && (strcmp(argv[1], "-test-root") == 0))
	{
		double (*f[3])(double) = {f1, f2, f3};
		double (*df[3])(double) = {df1, df2, df3};
		int n1, n2;
		double b1, b2, eps;
		printf("Enter functions, bounds, and epsilon (example: 1 2 1.12 2.23 0.1): ");
		scanf("%d%d%lf%lf%lf", &n1, &n2, &b1, &b2, &eps);
		printf("%lf\n", root(f[n1 - 1], f[n2 - 1], df[n1 - 1], df[n2 - 1], b1, b2, eps));
		return 0;
	}

	if(argc > 1 && (strcmp(argv[1], "-test-integral") == 0))
	{
		double (*f[3])(double) = {f1, f2, f3};
		int n;
		double b1, b2, eps;
		printf("Enter function, bounds, and epsilon (example: 2 1.12 2.23 0.1): ");
		scanf("%d%lf%lf%lf", &n, &b1, &b2, &eps);
		printf("%lf\n", integral(f[n - 1], b1, b2, eps));
		return 0;
	}
	
	double a = root(f1, f3, df1, df3, 2.065, 2.3, 0.0001);
	if(argc > 1 && (strcmp(argv[1], "-a") == 0))
	{
		printf("f1 f3: %lf", a);
		if(argc > 2 && (strcmp(argv[2], "-b") == 0))
		{
			printf(", %d iterations", iterations);
		}
		printf("\n");
	}

	double b = root(f2, f3, df2, df3, 3, 5, 0.0001);
	if(argc > 1 && (strcmp(argv[1], "-a") == 0))
	{
		printf("f2 f3: %lf", b);
		if(argc > 2 && (strcmp(argv[2], "-b") == 0))
		{
			printf(", %d iterations", iterations);
		}
		printf("\n");
	}

	double c = root(f1, f2, df1, df2, 5, 7, 0.0001);
	if(argc > 1 && (strcmp(argv[1], "-a") == 0))
	{
		printf("f1 f2: %lf", c);
		if(argc > 2 && (strcmp(argv[2], "-b") == 0))
		{
			printf(", %d iterations", iterations);
		}
		printf("\n");
	}
	
	double s = integral(f3f1, a, b, 0.0001) + integral(f2f1, b, c, 0.0001);
	printf("%lf\n", s);
	
	return 0;
}
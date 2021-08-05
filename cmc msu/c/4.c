#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INF LLONG_MAX

typedef struct point
{
	int n, m;
} point;

long long mabs(long long x)
{
	return x > 0 ? x : -x;
}

int h(int n0, int m0, int n1, int m1)
{
	return mabs(n1 - n0) + mabs(m1 - m0);
}

int main(void)
{
	int n, m;
	point p0, p1;
	scanf("%d%d%d%d%d%d", &n, &m, &p0.n, &p0.m, &p1.n, &p1.m);
	////////////////////////////////////////////////////////////
	long long **height = malloc(sizeof(long long*) * n),
		**len = malloc(sizeof(long long*) * n),
		**used = malloc(sizeof(long long*) * n);
	for(int i = 0; i < n; ++i)
	{
		long long *height_row = malloc(sizeof(long long) * m),
			*len_row = malloc(sizeof(long long) * m),
			*used_row = malloc(sizeof(long long) * m);
		height[i] = height_row;
		len[i] = len_row;
		used[i] = used_row;
	}
	//////////////////////////////////////////////////////////////
	for(int i = 0; i < n; ++i)
	{
		for(int j = 0; j < m; ++j)
		{
			scanf("%lld", &height[i][j]);
			len[i][j] = INF;
			used[i][j] = 0;
		}
	}
	len[p0.n][p0.m] = 0;
	///////////////////////////////////////////////////////////
	while(1)
	{
		point v;
		v.n = -1;
		v.m = -1;
		for(int i = 0; i < n; ++i)
		{
			for(int j = 0; j < m; ++j)
			{
				if(!used[i][j] &&
					(v.n == -1 || (len[v.n][v.m] > len[i][j])))
				{
					v.n = i;
					v.m = j;
				}
			}
		}
		if(used[p1.n][p1.m] || v.n == -1 || len[v.n][v.m] == INF)
		{
			break;
		}
		used[v.n][v.m] = 1;
		if(v.n - 1 >= 0 &&
			len[v.n - 1][v.m] > len[v.n][v.m] +
			mabs(height[v.n][v.m] - height[v.n - 1][v.m]) -
			h(p1.n, p1.m, v.n, v.m) + h(p1.n, p1.m, v.n - 1, v.m))
		{
			len[v.n - 1][v.m] = len[v.n][v.m] +
				mabs(height[v.n][v.m] - height[v.n - 1][v.m]) -
				h(p1.n, p1.m, v.n, v.m) + h(p1.n, p1.m, v.n - 1, v.m);
		}
		if(v.n + 1 < n &&
			len[v.n + 1][v.m] > len[v.n][v.m] +
			mabs(height[v.n][v.m] - height[v.n + 1][v.m]) -
			h(p1.n, p1.m, v.n, v.m) + h(p1.n, p1.m, v.n + 1, v.m))
		{
			len[v.n + 1][v.m] = len[v.n][v.m] +
				mabs(height[v.n][v.m] - height[v.n + 1][v.m]) -
				h(p1.n, p1.m, v.n, v.m) + h(p1.n, p1.m, v.n + 1, v.m);
		}
		if(v.m - 1 >= 0 &&
			len[v.n][v.m - 1] > len[v.n][v.m] +
			mabs(height[v.n][v.m] - height[v.n][v.m - 1]) -
			h(p1.n, p1.m, v.n, v.m) + h(p1.n, p1.m, v.n, v.m - 1))
		{
			len[v.n][v.m - 1] = len[v.n][v.m] +
				mabs(height[v.n][v.m] - height[v.n][v.m - 1]) -
				h(p1.n, p1.m, v.n, v.m) + h(p1.n, p1.m, v.n, v.m - 1);
		}
		if(v.m + 1 < m &&
			len[v.n][v.m + 1] > len[v.n][v.m] +
			mabs(height[v.n][v.m] - height[v.n][v.m + 1]) -
			h(p1.n, p1.m, v.n, v.m) + h(p1.n, p1.m, v.n, v.m + 1))
		{
			len[v.n][v.m + 1] = len[v.n][v.m] +
				mabs(height[v.n][v.m] - height[v.n][v.m + 1]) -
				h(p1.n, p1.m, v.n, v.m) + h(p1.n, p1.m, v.n, v.m + 1);
		}
	}
	//////////////////////////////////////////////////////////////
	printf("%lld", len[p1.n][p1.m] + h(p1.n, p1.m, p0.n, p0.m));
	//////////////////////////////////////////////////////////////
	for(int i = 0; i < n; ++i)
	{
		free(height[i]);
		free(len[i]);
		free(used[i]);
	}
	free(height);
	free(len);
	free(used);
	return 0;
}
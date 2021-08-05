#include <stdio.h>
#include <string.h>
 
const int MAX_SIZE = 1e6 + 1;

int min(int a, int b)
{
	return a < b ? a : b;
}

//based
//on z-function
int find_period(const char *s, int n)
{
	int z[n];
	z[0] = n;
	//блок совпадения
	int left = 0, right = 0;
	for(int i = 1; i < n; ++i)
	{
		z[i] = 0;
		if(i <= right)
		{
			z[i] = min(z[i - left], right - i + 1);
		}
		while(i + z[i] < n && s[z[i]] == s[i + z[i]])
		{
			++z[i];
		}
		if(i + z[i] - 1 > right)
		{
			left = i;
			right = i + z[i] - 1;
		}
		if(n == z[i] + i)
		{
			printf("%d ", i);
		}
	}
	return 0;
}
 
int main(void)
{
	char s[MAX_SIZE];
	scanf("%s", s);
	int n = strlen(s);
	find_period(s, n);
	printf("%d ", n);
	return 0;
}
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define SIZE 20001

int max(int a, int b)
{
	return a > b ? a : b;
}

int min(int a, int b)
{
	return a < b ? a : b;
}

typedef struct num
{
	int sign, power;
	char digits[SIZE];
} num;

void num_init(num *n, int sign, int power, char *str)
{
	n->sign = sign;
	n->power = power;
	for(int i = 0; i < power; ++i)
	{
		n->digits[i] = str[power - 1 - i] - '0';
	}
}

void num_init_default(num *n, int sign, int power)
{
	n->sign = sign;
	n->power = power;
	for(int i = 0; i < n->power; ++i)
	{
		n->digits[i] = 0;
	}
}

void num_copy(num *target, num source)
{
	target->sign = source.sign;
	target->power = source.power;
	for(int i = 0; i < source.power; ++i)
	{
		target->digits[i] = source.digits[i];
	}
}

//l >= r, no sign
num sub(num l, num r)
{
	num s;
	int rem = 0; 
	for(int i = 0; i < l.power || rem; ++i)
	{
		int d = l.digits[i] - (i < r.power ? r.digits[i] : 0) - rem;
		rem = d < 0;
		s.digits[i] = rem ? d + 10 : d;
	}
	for(s.power = l.power; s.power > 1 && s.digits[s.power - 1] == 0; --s.power);
	return s;
}

num sum(num l, num r)
{
	num s;
	if(l.sign * r.sign > 0)
	{
		s.sign = l.sign;
		s.power = max(l.power, r.power);
		int rem = 0; 
		for(int i = 0; i < s.power; ++i)
		{
			int la = i < l.power ? l.digits[i] : 0,
				ra = i < r.power ? r.digits[i] : 0,
				d = la + ra + rem;
			s.digits[i] = d % 10;
			rem = d >= 10;
		}
		if(rem > 0)
		{
			++s.power;
			s.digits[s.power - 1] = rem;
		}
	}
	else
	{
		if(l.power > r.power ||
			(l.power == r.power && l.digits[l.power - 1] >= r.digits[r.power - 1]))
		{
			num_copy(&s, sub(l, r));
			s.sign = 1;
		}
		else
		{
			num_copy(&s, sub(r, l));
			s.sign = -1;
		}
	}
	return s;
}

num naive_mul(num l, num r)
{
	num p;
	num_init_default(&p, l.sign * r.sign, l.power + r.power);
	int rem = 0; 
	for(int i = 0; i < r.power; ++i)
	{
		for(int j = 0; j < l.power || rem > 0; ++j)
		{
			int la = j < l.power ? l.digits[j] : 0,
				d = rem +  p.digits[i + j] + r.digits[i] * la;
			rem = d / 10;
			p.digits[i + j] = d % 10;
		}
	}
	for(; p.power > 1 && p.digits[p.power - 1] == 0; --p.power);
	return p;
}

void print(num n)
{
	if(n.sign < 0)
	{
		printf("-");
	}
	for(int i = n.power - 1; i >= 0; --i)
	{
		printf("%d", n.digits[i]);
	}
}

void scan(num *n1, char *op, num *n2)
{
	char str[2 * SIZE];
	scanf("%s", str);
	char s1[SIZE], s2[SIZE];
	int i = -1;
	while(isdigit(str[++i])) s1[i] = str[i];
	num_init(n1, 1, i, s1);
	*op = str[i];
	int len = strlen(str), j = -1;
	while(++j < len - i - 1) s2[j] = str[i + j + 1];
	num_init(n2, 1, j, s2);
}

int main(void)
{
	num n1, n2, res;
	char op;
	scan(&n1, &op, &n2);
	switch(op)
	{
		case '+':
			num_copy(&res, sum(n1, n2));
			break;
		case '-':
			n2.sign = -1;
			num_copy(&res, sum(n1, n2));
			break;
		case '*':
			num_copy(&res, naive_mul(n1, n2));
			break;
		default:
			break;
	}
	print(res);
	return 0;
}
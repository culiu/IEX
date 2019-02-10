#ifndef PRIME
#define PRIME

#include <iostream>
#include <cstdio>
#include <cmath>
#include <assert.h>
#define eps 1e-9
using namespace std;


bool IsPrime[10001];
int PrimeTable[10001];
int PrimeNumber;

void filter()
{
	int i, j;
	for (i = 0; i<10000; i++)
		IsPrime[i] = true;
	IsPrime[0] = IsPrime[1] = false;
	for (i = 2; i<10000; i++)
	{
		if (IsPrime[i])
		{
			for (j = i + i; j <= 10000; j += i)
				IsPrime[j] = false;
		}
	}
	PrimeNumber = 0;
	for (i = 0; i<10000; i++)
	{
		if (IsPrime[i])
			PrimeTable[PrimeNumber++] = i;
	}
}

int MinPrimeFactor(int n)
{
	assert(n>0);
	int m = (int)sqrt(n + eps);
	int i;
	for (i = 2; i <= m; i++)
	{
		if (IsPrime[i] && n%i == 0)
		{
			return i;
		}
	}
	return n;
}

int FindTableSize(int n, int DownBound, int UpBound)
{
	assert(n >= 0 && DownBound >= 0 && DownBound <= UpBound);
	int change = (n & 1) + 1;
	int tn;
	int max_prime = 0;
	int ret = -1;
	int factor;
	if ((factor = MinPrimeFactor(n)) == n)
	{
		return n;
	}
	else if (factor > max_prime)
	{
		max_prime = factor;
		ret = n;
	}
	while (change <= DownBound)
	{
		tn = n + change;
		if ((factor = MinPrimeFactor(tn)) == tn)
		{
			return tn;
		}
		else if (factor > max_prime)
		{
			max_prime = factor;
			ret = tn;
		}
		tn = n - change;
		if ((factor = MinPrimeFactor(tn)) == tn)
		{
			return tn;
		}
		else if (factor > max_prime)
		{
			max_prime = factor;
			ret = tn;
		}
		change += 2;
	}
	while (change <= UpBound)
	{
		tn = n + change;
		if ((factor = MinPrimeFactor(tn)) == tn)
		{
			return tn;
		}
		else if (factor > max_prime)
		{
			max_prime = factor;
			ret = tn;
		}
		change += 2;
	}
	return ret;
}

void FindPrime(int n, int &s, int &b)
{
	filter();
	int change = (n & 1) + 1;
	int tn;
	if (MinPrimeFactor(n) == n)
	{
		s = b = n;
	}
	else
	{
		s = b = -1;
		tn = n - change;
		while (tn>1)
		{
			if (MinPrimeFactor(tn) == tn)
			{
				s = tn;
				break;
			}
			tn -= 2;
		}
		tn = n + change;
		while (1)
		{
			if (MinPrimeFactor(tn) == tn)
			{
				b = tn;
				break;
			}
			tn += 2;
		}
	}
}

#endif
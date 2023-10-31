#ifndef UTILITIES
#define UTILITIES

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "utilities.h"

int hash_function(int postal,int round , int m)
{
    int mod=(pow(2,round) * m);
    int res= postal % mod;
    return res;
}

int POW(int i,int j)
{
    for(int k=0;k<j;k++)
    {
        i=i*i;
    }
    if(j == 0)
    {
        return 1;
    }

    return i;
}

int is_power_of_two(int num)
{
    if(num==0)
    {
        return 0;
    }
    while(num != 1)
    {
        if (num % 2 != 0)
        {
            return 0;
        }
        num = num / 2;
    }
    return 1;
}

#endif
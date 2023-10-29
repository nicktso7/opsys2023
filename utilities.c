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

#endif
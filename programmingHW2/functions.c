#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LOW_23_BITS ((1 << 23) - 1)
#define LOW_8_BITS ((1 << 8) - 1)

void print_bits(int x)
{
    for (int i = sizeof(int) * 8; i >= 0; i--)
    {
        if (x & (1 << i))
        {
            printf("1");
        }
        else
        {
            printf("0");
        }
    }
    printf("\n");
}

long int int_multiply(int x, int y)
{
    long int lx = x;
    long int ly = y;
    long int res = 0;
    for (int i = 0; i < sizeof(long int) * 8; i++)
    {
        if (ly & ((long)1 << i))
        {
            res += lx;
        }
        lx = lx << 1;
    }
    return res;
}

float float_multiply(float a, float b)
{
    unsigned int val_a = *(unsigned int *)(&a);
    unsigned int val_b = *(unsigned int *)(&b);
    if (val_a == 0 || val_b == 0)
    {
        return 0.0;
    }
    unsigned int exp_a = ((val_a >> 23) & LOW_8_BITS);
    unsigned int mant_a = (val_a & LOW_23_BITS);

    unsigned int exp_b = ((val_b >> 23) & LOW_8_BITS);
    unsigned int mant_b = (val_b & LOW_23_BITS);

    unsigned int new_exp = exp_a + exp_b - 127;
    mant_a |= (1 << 23);
    mant_b |= (1 << 23);
    unsigned long new_mant = int_multiply(mant_a, mant_b) >> 23;
    if (new_mant & (1 << 24))
    {
        new_mant = new_mant >> 1;
        new_exp++;
    }
    unsigned int mant = new_mant;
    unsigned int sign = (val_a >> 31) ^ (val_b >> 31);
    unsigned int res = (sign << 31) | (new_exp << 23) | (mant & LOW_23_BITS);

    return *(float *)&res;
}

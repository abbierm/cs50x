#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>

bool check_sum(long n);
int get_length(long n);
bool check_amex(int length, long cc);
bool check_visa(int length, long cc);
bool check_mastercard(int length, long cc);

int main(void)
{
    long cc_number = get_long("Number: ");
    if (!check_sum(cc_number))
    {
        printf("INVALID\n");
        return 0;
    }
    int length = get_length(cc_number);
    if (check_amex(length, cc_number))
    {
        printf("AMEX\n");
        return 0;
    }
    if (check_visa(length, cc_number))
    {
        printf("VISA\n");
        return 0;
    }
    if (check_mastercard(length, cc_number))
    {
        printf("MASTERCARD\n");
        return 0;
    }
    printf("INVALID\n");
    return 0;
}

int get_length(long n)
{
    int counter = 0;
    while (n > 0)
    {
        n /= 10;
        ++counter;
    }
    return counter;
}

bool check_sum(long n)
{
    int even = 0;
    int odd = 0;
    int check = 0;
    while (n > 0)
    {
        // odd numbers
        int x = n % 10;
        odd += x;
        n /= 10;

        // even numbers
        int y = (n % 10) * 2;
        while (y > 0)
        {
            even += y % 10;
            y /= 10;
        }
        n /= 10;
    }
    check = even + odd;
    if (!(check % 10 == 0))
    {
        return false;
    }
    return true;
}

bool check_amex(int length, long cc)
{
    if (!(length == 15))
    {
        return false;
    }
    int front_two = cc / 10000000000000;
    if ((front_two == 34 || front_two == 37))
    {
        return true;
    }
    return false;
}

bool check_visa(int length, long cc)
{
    if (!(length == 13 || length == 16))
    {
        return false;
    }
    if (length == 16 && cc / 1000000000000000 == 4)
    {
        return true;
    }
    if (length == 13 && (cc / 1000000000000 == 4))
    {
        return true;
    }
    return false;
}

bool check_mastercard(int length, long cc)
{
    if (!(length == 16))
    {
        return false;
    }
    int front_two = cc / 100000000000000;
    if ((front_two < 51 || front_two > 55))
    {
        return false;
    }
    return true;
}

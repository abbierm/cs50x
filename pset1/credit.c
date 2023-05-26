#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    // Flag that when = 0, keeps the program running
    int flag = 0;

    // Take users credit card number
    long USERNUM;
    USERNUM = get_long("Input your credit card number:  \n");

    int count = 0; // Counter I use to determin the numbers in the CC

    // While statement that if = 1, turns loop off and prints INVALID/ VISA/ MASTERCARD/ or AMEX
    while (flag == 0)
    {
        // Make sure it is 15, 13, or 16 digits long
        long counter = USERNUM;
        do
        {
            counter /= 10;
            ++count;
        }
        while (counter != 0);

        if (count == 15 || count == 13 || count == 16)
        {
            // check sum
            long luhns_algo = USERNUM;
            int even_sum = 0;
            int odd_sum = 0;

            while (luhns_algo > 0)
            {
                // Odd Digits
                int sum1 = 0;
                sum1 = luhns_algo % 10;
                luhns_algo /= 10;
                odd_sum += sum1;

                // Even Digits
                int multiply_two = 0;
                multiply_two = luhns_algo % 10;
                multiply_two *= 2;

                // Adds the digits in multiple of two together
                int add_digit = 0;
                while (multiply_two > 0)
                {
                    add_digit += multiply_two % 10;
                    multiply_two /= 10;
                }
                even_sum += add_digit;
                luhns_algo /= 10;

            }

            // Adds even + odd together
            int sum_total = 0;
            sum_total = even_sum + odd_sum;

            // Checksum
            if (sum_total % 10 == 0)
            {
                long cc_type_number = USERNUM;
                int first_digits; // variable to store beginning cc digits

                // AMEX 15 DIGITS
                if (count == 15)
                {
                    first_digits = cc_type_number / 10000000000000;
                    if (first_digits == 37 | first_digits == 34)
                    {
                        printf("AMEX\n");
                        flag = 1;
                    }
                }

                else if (count == 13)
                {
                    first_digits = cc_type_number / 1000000000000;
                    if (first_digits == 4)
                    {
                        printf("VISA\n");
                        flag = 1;
                    }
                    else
                    {
                        printf("INVALID\n");
                        flag = 1;
                    }
                }
                else
                {
                    first_digits = cc_type_number / 100000000000000;
                    if (first_digits >= 51 && first_digits <= 55)
                    {
                        printf("MASTERCARD\n");
                        flag = 1;
                    }
                    else if (first_digits >= 40 && first_digits <= 49)
                    {
                        printf("VISA\n");
                        flag = 1;
                    }
                    else
                    {
                        printf("INVALID\n");
                        flag = 1;
                    }
                }

            }
            else
            {
                printf("INVALID\n");
                flag = 1;
            }
        }
        else
        {
            flag = 1;
            printf("INVALID\n");
        }
    }
}

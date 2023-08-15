#include <cs50.h>
#include <stdio.h>
#include <math.h>

bool cardNumIsValid(long num);

int main(void)
{
    // Ask for credit card #
    long cardNum = get_long("Enter a credit card number: ");

    // Length
    long cardNumLength = 0;
    for (long i = cardNum; i > 0; i /= 10)
    {
        ++cardNumLength;
    }

    // First 2 digits
    long first2Digits = cardNum;
    while (first2Digits > 99)
    {
        first2Digits /= 10;
    }

    if (cardNumIsValid(cardNum))
    {
        // American Express
        if (cardNumLength == 15 && (first2Digits == 34 || first2Digits == 37))
        {
            printf("AMEX\n");
        }
        // MasterCard
        else if (cardNumLength == 16 && first2Digits >= 51 && first2Digits <= 55)
        {
            printf("MASTERCARD\n");
        }
        // Visa
        else if ((cardNumLength == 13 || cardNumLength == 16) && first2Digits / 10 == 4)
        {
            printf("VISA\n");
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

}

// Return if the credit card number is valid
bool cardNumIsValid(long num)
{
    long sum = 0;
    for (long i = num / 10; i > 0; i /= 100)
    {
        long product = i % 10 * 2;
        // If product is 2 digits
        if (product > 9)
        {
            sum += product % 10 + product / 10;
        }
        else
        {
            sum += product;
        }
    }
    for (long i = num; i > 0; i /= 100)
    {
        long digit = i % 10;
        sum += digit;
    }
    return sum % 10 == 0;
}
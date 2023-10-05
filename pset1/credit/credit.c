#include <math.h>
#include <stdio.h>

int main(void)
{
    long cardNumber;
    int cardSum = 0;
    int firstDigit, secondDigit;

    do {
        printf("What's your card number?");
        scanf("%li",&cardNumber);
    } while (cardNumber < 1);

    //Takes the sum of every other digit starting from second-to-last
    for (int i = 1; i <= 16; i += 2) {
        int valueSum = (int)((long)(cardNumber / pow(10,i)) % 10) * 2;
        cardSum += (valueSum / 10) + (valueSum % 10);
    }

    for (int i = 0; i <= 16; i += 2) {
        cardSum += (int)((long)(cardNumber / pow(10,i)) % 10);
    }

    if (cardSum % 10 != 0) {
        printf("INVALID\n");
        return 0;
    }

    int lenght = 0;

    for (long n = cardNumber; n != 0; n /= 10) {
        lenght++;
    }

    //Get the first and second digits;
    firstDigit = (int)(cardNumber / pow(10,lenght - 1)) % 10;
    secondDigit = (int)(cardNumber / pow(10,lenght - 2)) % 10;

    //Visa
    //Lenght 13 or 16 and first digit = 4;
    if ((lenght == 13 || lenght == 16) && firstDigit == 4)
    {
        printf("VISA\n");
    }
    //MasterCard
    //Lenght 16 and starts with 51-55;
    else if (lenght == 16 && firstDigit == 5 && (secondDigit > 0 && secondDigit < 6))
    {
        printf("MASTERCARD\n");
    }
    //American Express
    //Lenght 15 and starts with 34 or 37;
    else if (lenght == 15 && firstDigit == 3 && (secondDigit == 4 || secondDigit == 7))
    {
        printf("AMEX\n");
    }
    else {
        printf("INVALID\n");
    }

    return 0;
}
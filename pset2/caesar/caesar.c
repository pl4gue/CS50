#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool only_digits(string s);

int main(int argc, string argv[])
{
    if (argc != 2 || !only_digits(argv[1])) {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    int k = atoi(argv[1]);

    string plaintext = get_string("plaintext: ");

    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        char currentChar = plaintext[i];

        if ((currentChar >= 65 && currentChar <= 90) || (currentChar >= 97 && currentChar <= 122))
        {
            // Gets the difference
            int diff = currentChar < 91 ? 65 : 97;
            plaintext[i] = (currentChar - diff + k) % 26 + diff;

        }
    }

    printf("ciphertext: %s\n", plaintext);
    return 0;
}

bool only_digits(string s)
{
    for (int i = 0, n = strlen(s); i < n; i++)
    {
        if (!isdigit(s[i])) return false;
    }

    return true;
}
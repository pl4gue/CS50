#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

const int ALPHABET_SIZE = 26;

int main(int argc, string argv[])
{
    //Check number of arguments
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    //Check size of key
    if (strlen(argv[1]) != ALPHABET_SIZE)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    char k[26];

    //Check if key has duplicates;
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        for (int j = 0, n = strlen(k); j < n; j++)
        {
            if (argv[1][i] == k[j])
            {
                printf("There can't be any duplicate characters.\n");
                return 1;
            }

            if (!isalpha(argv[1][i]))
            {
                printf("There can't be any invalid characters.\n");
                return 1;
            }
        }

        k[i] = argv[1][i];
    }


    string plaintext = get_string("plaintext: ");

    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        // if isn't alphabetical char then continues
        if (!isalpha(plaintext[i])) continue;

        for (int j = 0; j < ALPHABET_SIZE; j++)
        {
            // if the char position isn't the position on k then continues
            if (toupper(plaintext[i]) - 65 != j) continue;

            printf("Current char of the plaintext: %c - %i\n", plaintext[i], toupper(plaintext[i]) - 65);
            printf("Same char on key: %c - %i\n", k[j], j);

            plaintext[i] = isupper(plaintext[i]) ? toupper(k[j]) : tolower(k[j]);
            break;
        }
    }

    printf("ciphertext: %s\n", plaintext);
    return 0;
}
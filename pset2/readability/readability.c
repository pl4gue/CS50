#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    string text = get_string("Text: ");
    float L, S;

    L = (count_letters(text) / (float) count_words(text)) * 100;
    S = (count_sentences(text) / (float) count_words(text)) * 100;

    int result = round((0.0588 * L) - (0.296 * S) - 15.8);

    if (result < 1) {
        printf("Before Grade 1\n");
        return 0;
    }

    if (result > 16) {
        printf("Grade 16+\n");
        return 0;
    }

    printf("Grade %i\n", (int)result);
}

int count_letters(string text) {
    int count = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char character = toupper(text[i]);
        if (character >= 65 && character <= 90) {
            count++;
        }
    }

    return count;
}

int count_words(string text) {
    int count = 0;
    bool newWord = true;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (newWord) count++;
        newWord = text[i] == ' ';
    }

    return count;
}

int count_sentences(string text) {
    int count = 0;

    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == '.' || text[i] == '!' || text[i] == '?')
        {
            count++;
        }
    }

    return count;
}
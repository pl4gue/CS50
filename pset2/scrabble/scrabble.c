#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print "Tie!" if the 2 scores are the same
    if (score1 == score2)
    {
        printf("Tie!\n");
        return 0;
    }

    /*
        printf("Player %i wins!\n", (score1 > score2) ? 1 : 2);

        The code bellow can be achieved by doing ternary operations like the above
    */

    int winner;
    if (score1 > score2)
    {
        winner = 1;
    }
    else
    {
        // Doesn't need to check for equality since it's checked above
        winner = 2;
    }

    printf("Player %i wins!\n", winner);
}

int compute_score(string word)
{
    int score = 0;

    for (int i = 0, n = strlen(word); i < n; i++)
    {
        /* Gets the current letter in uppercase as an integer */

        int letter = (int) toupper(word[i]);

        /* Checks if the letter is between the values of A-Z (Uppercase A = 65, Uppercase Z = 90),
           then add the value taken from the POINTS array */

        if (letter >= 65 && letter <= 90)
        {
            score += POINTS[letter - 65];
        }
    }

    return score;
}

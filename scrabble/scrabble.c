#include <ctype.h>
#include <cs50.h>
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
    // printf("Player 1: %i\n", score1);
    int score2 = compute_score(word2);
    // printf("Player 2: %i\n", score2);
    
    // TODO: Print the winner
    if (score1 > score2)
    {
        printf("Player 1 Wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 Wins!\n");
    }
    else if (score1 == score2)
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{
    // TODO: Compute and return score for string
    string alphabet = "abcdefghijklmnopqrstyvwxyz";
    int score = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        // printf("Letter: %c - ", word[i]);
        for (int x = 0, y = strlen(alphabet); x < y; x++)
        {
            if (tolower(word[i]) == alphabet[x])
            {
                // printf("Point:%i\n", POINTS[x]);
                score = score + POINTS[x];
            }
        }
    }

    return score;
}
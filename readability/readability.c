#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Ask user for text string.
    string text = get_string("Text: ");
    
    float letters = count_letters(text);
    float words =  count_words(text);
    float sentences =  count_sentences(text);
    
    // Calculate the averages and the readability.
    float L = letters / words * 100;
    float S = sentences / words * 100;
    // printf("Index = 0.0588 * %f - 0.296 * %f - 15.8\n", L, S);
    float index = ((0.0588 * L) - (0.296 * S)) - 15.8;
    
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index <= 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %i\n", (int) round(index));
    }
}

int count_letters(string text)
{
    int letters = 0;
    // Analyse the text for letter count.
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char character = text[i];
        if (isalpha(character))
        {
            letters = letters + 1;
        }
    }
    // printf("%i letter(s)\n", letters);
    return letters;
}

int count_words(string text)
{
    int words = 1;
    // Analyse the text for word count, starting at one because of the first word in the sentence.
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char character = text[i];
        if (isspace(character))
        {
            words = words + 1;
        }
    }
    // printf("%i word(s)\n", words);
    return words;
}

int count_sentences(string text)
{
    int sentences = 0;
    // Analyse the text for sentence count.
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        char character = text[i];
        if (character == '.' || character == '!' || character == '?')
        {
            sentences = sentences + 1;
        }
    }
    // printf("%i sentences(s)\n", sentences);
    return sentences;
}
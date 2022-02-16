#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

string encrypt(string plaintext, int key);

int main(int argc, string argv[])
{
    // printf("argc: %i\n", argc);
    // printf("argv: %s\n", argv[1]);
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // Iterating over all the characters in the string.
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        //Check if they are digits
        if (isdigit(argv[1][i]) && argc == 2)
        {
            //Keep looping if its a digit and has 1 argument. (2 including script title.)
            continue;
        }
        //Show error string and break script with error code 1.
        else
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    //Start of encryption
    int key = atoi(argv[1]);
    string plaintext = get_string("plaintext: ");
    string ciphertext = encrypt(plaintext, key);
    printf("ciphertext: %s\n", ciphertext);
    return 0;
}

string encrypt(string plaintext, int key)
{
    for (int i = 0, n = strlen(plaintext); i < n; i++)
    {
        if (isalpha(plaintext[i]))
        {
            if (isupper(plaintext[i]))
            {
                // printf("plaintext[%i]: %i\n", i, plaintext[i]);
                plaintext[i] = (plaintext[i] + key - 65) % 26 + 65;
            }
            if (islower(plaintext[i]))
            {
                // printf("plaintext[%i]: %i\n", i, plaintext[i]);
                plaintext[i] = (plaintext[i] + key - 97) % 26 + 97;
            }
            // printf("ciphertext[%i]: %i\n", i, plaintext[i]);
        }
    }
    return plaintext;
}
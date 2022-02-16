#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // Get block height from user.
    int blocks;
    do
    {
        blocks = get_int("Height: ");
    }
    while (blocks > 8 || blocks < 1);
    
    // Loops for printing blocks
    int loop = 0;
    while (loop < blocks)
    {
        // printf("loop: %i\n", loop);
        // While Loop for printing front spaces
        int space = blocks - loop;
        while (space > 1)
        {
            // printf("space: %i\n", space);
            printf(" ");
            space = space - 1;
        }
        // While loops for printing front hash
        int front_hash = 0;
        while (front_hash <= loop)
        {
            printf("#");
            front_hash++;
            // printf("hash: %i\n", hash);
        }
        //While loop for printing middle gap
        int gap = 0;
        int middle = 2;
        while (gap < middle)
        {
            printf(" ");
            gap++;
            // printf("gap: %i\n", gap);
        }
        // While loop for printing end hash
        int hash = 0;
        while (hash <= loop)
        {
            printf("#");
            hash++;
            // printf("hash: %i\n", hash);
        }
        printf("\n");
        loop++;
    }
}
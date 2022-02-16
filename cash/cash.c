#include <cs50.h>
#include <stdio.h>
#include <math.h>

int main(void)
{
    float dollars;
    int cents;
    do
    {
        dollars = get_float("Change owed: ");
        cents = round(dollars * 100);
        // printf("Change = %i cents\n", cents);
    }
    while (dollars < 0);
    int coins = 0;
    // Removing 25 cents
    while (cents >= 25)
    {
        cents = cents - 25;
        coins++;
    }
    // Removing 10 cents
    while (cents >= 10)
    {
        cents = cents - 10;
        coins++;
    }
    // Removing 5 cents
    while (cents >= 5)
    {
        cents = cents - 5;
        coins++;
    }
    // Removing 1 cents
    while (cents >= 1)
    {
        cents = cents - 1;
        coins++;
    }
    printf("Coins: %i\n", coins);
}
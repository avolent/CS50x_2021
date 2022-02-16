#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size 
    int start_size;
    do
    {
        start_size = get_int("Starting population size (> 9): ");
    }
    while (start_size < 9);
    // TODO: Prompt for end size
    int end_size;
    do
    {
        end_size = get_int("Ending population size (End Size > Start size): ");
    }
    while (end_size < start_size);
    // TODO: Calculate number of years until we reach threshold
    int year = 0;
    int growth = start_size;
    
    while (growth < end_size)
    { 
        growth = growth + (growth / 3) - (growth / 4);
        year++;
    }
    // TODO: Print number of years
    printf("Years: %i\n", year);
}

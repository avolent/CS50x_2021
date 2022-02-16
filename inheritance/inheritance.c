// Simulate genetic inheritance of blood type

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Each person has two parents and two alleles
typedef struct person
{
    struct person *parents[2];
    char alleles[2];
}
person;

const int GENERATIONS = 3;
const int INDENT_LENGTH = 4;

person *create_family(int generations);
void print_family(person *p, int generation);
void free_family(person *p);
char random_allele();

int main(void)
{
    // Seed random number generator
    srand(time(0));

    // Create a new family with three generations
    person *p = create_family(GENERATIONS);

    // Print family tree of blood types
    print_family(p, 0);

    // Free memory
    free_family(p);
}

// Create a new individual with `generations`
person *create_family(int generations)
{
    // TODO: Allocate memory for new person
    person *x = malloc(sizeof(person));
    if (x == NULL)
    {
        return NULL;
    }

    // Generation with parent data
    if (generations > 1)
    {
        // TODO: Recursively create blood type histories for parents
        generations--;
        x->parents[0] = create_family(generations);
        x->parents[1] = create_family(generations);

        // TODO: Randomly assign child alleles based on parents
        // Assign current persons allese randomly based off parents[0]
        x->alleles[0] = x->parents[0]->alleles[rand() % 2];
        // Assign current persons allese randomly based off parents[1]
        x->alleles[1] = x->parents[1]->alleles[rand() % 2];

    }

    // Generation without parent data (GEN 2)
    // It will keep looping till it hits Generations of 1 and set the parents as null and
    // assign random alleles
    else
    {
        // TODO: Set parent pointers to NULL as it is the starting parents
        x->parents[0] = NULL;
        x->parents[1] = NULL;
        // TODO: Randomly assign alleles since we dont care about their parents
        x->alleles[0] = random_allele();
        x->alleles[1] = random_allele();
    }
    // TODO: Return newly created person
    return x;
}

// Free `p` and all ancestors of `p`.
void free_family(person *p)
{
    // TODO: Handle base case
    if (p == NULL)
    {
        return;
    }
    // TODO: Free parents
    // Keep looping function for each parent until you hit null.
    free_family(p->parents[0]);
    free_family(p->parents[1]);
    
    // TODO: Free child
    // Clear the current persons memory.
    free(p);
}

// Print each family member and their alleles.
void print_family(person *p, int generation)
{
    // Handle base case
    if (p == NULL)
    {
        return;
    }

    // Print indentation
    for (int i = 0; i < generation * INDENT_LENGTH; i++)
    {
        printf(" ");
    }

    // Print person
    printf("Generation %i, blood type %c%c\n", generation, p->alleles[0], p->alleles[1]);
    print_family(p->parents[0], generation + 1);
    print_family(p->parents[1], generation + 1);
}

// Randomly chooses a blood type allele.
char random_allele()
{
    int r = rand() % 3;
    if (r == 0)
    {
        return 'A';
    }
    else if (r == 1)
    {
        return 'B';
    }
    else
    {
        return 'O';
    }
}

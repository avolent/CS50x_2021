#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int main(int argc, char *argv[])
{
    const int BLOCK_SIZE = 512;
    // Check the input
    // printf("argc: %i\n", argc);
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open file defined by argv[1] and error out if empty.
    FILE *file = fopen(argv[1], "r");
    if (file == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    // Initialize the 512 byte block.
    uint8_t block[BLOCK_SIZE];
    
    // 000.jpg file name.
    char image_name[7];
    int image = 0;
    sprintf(image_name, "%03i.jpg", image);
    FILE *img = fopen(image_name, "w");
    
    
    //while loop for reading and writing
    while (fread(&block, sizeof(uint8_t), BLOCK_SIZE, file))
    {

        // printf("Block: %hhu\n", block[0]);
        //If block contains the start of a jpeg
        if (block[0] == 255 && block[1] == 216 && block[2] == 255 && block[3] >= 224 && block[3] <= 239)
        {
            // printf("Found a JPEG header\n");
            // First JPEG
            if (image == 0)
            {
                printf("\nFound first JPEG header - Image:%i\n", image);
                // Write to first image file
                fwrite(&block, sizeof(uint8_t), BLOCK_SIZE, img);
                image++;
                
            }
            //Not the first JPEG
            else
            {
                printf("\nFound another JPEG header - Image:%i\n", image);
                // Close previous file.
                fclose(img);
                // Open new File
                sprintf(image_name, "%03i.jpg", image);
                img = fopen(image_name, "w");
                fwrite(&block, sizeof(uint8_t), BLOCK_SIZE, img);
                image++;
            }
        }
        else
        {
            if (image > 0)
            {
                // Continue writing image until a new jped head arises
                printf(".");
                fwrite(&block, sizeof(uint8_t), BLOCK_SIZE, img);
            }
        }
    }
    fclose(img);
}
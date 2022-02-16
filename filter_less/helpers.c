#include "helpers.h"
#include "stdio.h"
#include "math.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // printf("Height: %i, Width: %i\n", height, width);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // printf("BEFORE - Red:%i Green:%i Blue:%i\n", image[y][x].rgbtRed, image[y][x].rgbtGreen, image[y][x].rgbtBlue);
            float average = (image[y][x].rgbtRed + image[y][x].rgbtGreen + image[y][x].rgbtBlue) / 3.0;
            // printf("Average: %f\n", round(average));
            image[y][x].rgbtRed = round(average);
            image[y][x].rgbtGreen = round(average);
            image[y][x].rgbtBlue = round(average);
            // printf("AFTER - Red:%i Green:%i Blue:%i\n", image[y][x].rgbtRed, image[y][x].rgbtGreen, image[y][x].rgbtBlue);
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // printf("Height: %i, Width: %i\n", height, width);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            // printf("BEFORE - Red:%i Green:%i Blue:%i\n", image[y][x].rgbtRed, image[y][x].rgbtGreen, image[y][x].rgbtBlue);
            float sepiaRed =  round(image[y][x].rgbtRed * 0.393 + image[y][x].rgbtGreen * 0.769 + image[y][x].rgbtBlue * 0.189);
            float sepiaGreen =  round(image[y][x].rgbtRed * 0.349 + image[y][x].rgbtGreen * 0.686 + image[y][x].rgbtBlue * 0.168);
            float sepiaBlue =  round(image[y][x].rgbtRed * 0.272 + image[y][x].rgbtGreen * 0.534 + image[y][x].rgbtBlue * 0.131);
            if (sepiaRed > 255)
            {
                sepiaRed = 255;
            }
            if (sepiaGreen > 255)
            {
                sepiaGreen = 255;
            }
            if (sepiaBlue > 255)
            {
                sepiaBlue = 255;
            }

            image[y][x].rgbtRed = sepiaRed;
            image[y][x].rgbtGreen = sepiaGreen;
            image[y][x].rgbtBlue = sepiaBlue;
            // printf("AFTER - Red:%i Green:%i Blue:%i\n", image[y][x].rgbtRed, image[y][x].rgbtGreen, image[y][x].rgbtBlue);
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // printf("Height: %i, Width: %i\n", height, width);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width / 2; x++)
        {
            // printf("y: %i r: %i\n", y, rx);
            // printf("y: %i rx: %i\n", y, rx);
            int rx = x + 1;
            // printf("RGB Before:%i\n", image[y][x].rgbtRed);
            RGBTRIPLE temp = image[y][x];
            image[y][x] = image[y][width - rx];
            image[y][width - rx] = temp;
            // printf("RGB After:%i\n", image[y][x].rgbtRed);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width];
    // printf("Height: %i, Width: %i\n", height, width);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            temp[y][x] = image[y][x];
            int n = 0;
            float red = 0;
            float green = 0;
            float blue = 0;
            for (int h = -1; h <= 1; h++)
            {
                for (int w = -1; w <= 1; w++)
                {
                    if (y + h < 0 || x + w < 0 || y + h >= height || x + w >= width)
                    {
                        continue;
                    }

                    // printf("y:%i x:%i\n", y + h, x + w);
                    red += image[y + h][x + w].rgbtRed;
                    green += image[y + h][x + w].rgbtGreen;
                    blue += image[y + h][x + w].rgbtBlue;
                    n++;
                    // printf("red:%f green:%f blue:%f n:%i\n",red ,green, blue, n);
                }
                // printf("TOTALS - red:%f green:%f blue:%f n:%i\n",red ,green, blue, n);
            }
            // printf("AVERAGES - red:%f green:%f blue:%f n:%i\n\n",round(red / n) , round(green / n), round(blue / n), n);
            temp[y][x].rgbtRed = round(red / n);
            temp[y][x].rgbtGreen = round(green / n);
            temp[y][x].rgbtBlue = round(blue / n);

        }
    }
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            image[y][x].rgbtRed = temp[y][x].rgbtRed;
            image[y][x].rgbtGreen = temp[y][x].rgbtGreen;
            image[y][x].rgbtBlue = temp[y][x].rgbtBlue;
        }
    }
    return;
}

#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int averageColor = round((image[h][w].rgbtRed + image[h][w].rgbtGreen + image[h][w].rgbtBlue) / 3.0);
            image[h][w].rgbtRed = averageColor;
            image[h][w].rgbtGreen = averageColor;
            image[h][w].rgbtBlue = averageColor;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width / 2; w++)
        {
            RGBTRIPLE currentPixel = image[h][w];
            image[h][w] = image[h][width - w - 1];
            image[h][width - w - 1] = currentPixel;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE image_copy[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image_copy[h][w] = image[h][w];
        }
    }

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int av_red = 0, av_green = 0, av_blue = 0;
            float pixel_count = 0;

            for (int i = -1; i < 2; i++)
            {
                //skips the height above 0 and below the maximum height;
                if ((h == 0 && i == -1) || (h == height - 1 && i == 1)) continue;

                for (int j = -1; j < 2; j++)
                {
                    //skips the width outside the range;
                    if ((w == 0 && j == -1) || (w == width - 1 && j == 1)) continue;

                    RGBTRIPLE curPixel = image_copy[h + i][w + j];

                    av_red += curPixel.rgbtRed;
                    av_green += curPixel.rgbtGreen;
                    av_blue += curPixel.rgbtBlue;

                    pixel_count++;
                }
            }

            image[h][w].rgbtRed = round(av_red / pixel_count);
            image[h][w].rgbtGreen = round(av_green / pixel_count);
            image[h][w].rgbtBlue = round(av_blue / pixel_count);
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image_copy[height][width];

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image_copy[h][w] = image[h][w];
        }
    }

    int Gx[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
    };

    int Gy[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
    };

    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {

            int GxRed = 0, GxGreen = 0, GxBlue = 0,
                GyRed = 0, GyGreen = 0, GyBlue = 0;

            for (int row = -1; row < 2; row++)
            {
                for (int col = -1; col < 2; col++)
                {
                    //Check for boundaries
                    if (h + row < 0 || h + row > height - 1 || w + col < 0 || w + col > width - 1) continue;

                    RGBTRIPLE currentPixel = image_copy[h + row][w + col];
                    int CurrentGx = Gx[row + 1][col + 1],
                        CurrentGy = Gy[row + 1][col + 1];

                    GxRed += currentPixel.rgbtRed * CurrentGx;
                    GxGreen += currentPixel.rgbtGreen * CurrentGx;
                    GxBlue += currentPixel.rgbtBlue * CurrentGx;

                    GyRed += currentPixel.rgbtRed * CurrentGy;
                    GyGreen += currentPixel.rgbtGreen * CurrentGy;
                    GyBlue += currentPixel.rgbtBlue * CurrentGy;
                }
            }

            float colorRed = sqrt((GxRed * GxRed) + (GyRed * GyRed));
            float colorGreen = sqrt((GxGreen * GxGreen) + (GyGreen * GyGreen));
            float colorBlue = sqrt((GxBlue * GxBlue) + (GyBlue * GyBlue));

            if (colorRed > 255) colorRed = 255;
            if (colorGreen > 255) colorGreen = 255;
            if (colorBlue > 255) colorBlue = 255;

            image[h][w].rgbtRed = (int) round(colorRed);
            image[h][w].rgbtGreen = (int) round(colorGreen);
            image[h][w].rgbtBlue = (int) round(colorBlue);
        }
    }

    return;
}

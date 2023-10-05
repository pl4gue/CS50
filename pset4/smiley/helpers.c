#include "helpers.h"

void colorize(int height, int width, RGBTRIPLE image[height][width])
{
    // Change all black pixels to a color of your choosing
    RGBTRIPLE choseColor = {59,59,242};

    for (int curHeight = 0; curHeight < height; curHeight++)
    {
        for (int curWidth = 0; curWidth < width; curWidth++)
        {
            //check if the pixel is black
            RGBTRIPLE *curPixel = &image[curHeight][curWidth];

            if (curPixel->rgbtRed == 0 && curPixel->rgbtGreen == 0 && curPixel->rgbtBlue == 0)
            {
                *curPixel = choseColor;
            }
        }
    }
}

#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int average = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            RGBTRIPLE temp;
            temp.rgbtBlue = 0;
            temp.rgbtGreen = 0;
            temp.rgbtRed = 0;

            temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            RGBTRIPLE unp;
            unp.rgbtBlue = image[i][j].rgbtBlue;
            unp.rgbtGreen = image[i][j].rgbtGreen;
            unp.rgbtRed = image[i][j].rgbtRed;
            copy[i][j] = unp;
        }
    }
    // for every row
    for (int i = 0; i < height; i++)
    {
        // and every byte at every row
        for (int j = 0; j < width; j++)
        {
            float counter = 0;
            float averageBlue = 0;
            float averageGreen = 0;
            float averageRed = 0;

            // check the row, the upper row and the lower row
            for (int k = i - 1; k <= i + 1; k ++)
            {
                // as long as the rows are valid
                if (k >= 0 && k < height)
                {
                    // and then check every byte, upper byte and lower byte of the previous rows
                    for (int m = j - 1; m <= j + 1; m++)
                    {
                        // as long as de the bytes are valid
                        if (m >= 0 && m < width)
                        {
                            averageBlue += copy[k][m].rgbtBlue;
                            averageGreen += copy[k][m].rgbtGreen;
                            averageRed += copy[k][m].rgbtRed;
                            counter++;
                        }
                    }
                }
            }

            image[i][j].rgbtBlue = round(averageBlue / counter);
            image[i][j].rgbtGreen = round(averageGreen / counter);
            image[i][j].rgbtRed = round(averageRed / counter);
        }
    }

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Sobel operators as bidimensional arrays
    int GX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
    int GY[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    // copy of the original image to keep an unmodified version
    RGBTRIPLE copy[height][width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image [i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //gx and gy to 0

            int gxSumBlue = 0;
            int gxSumGreen = 0;
            int gxSumRed = 0;

            int gySumBlue = 0;
            int gySumGreen = 0;
            int gySumRed = 0;

            int sqrtsBlue = 0;
            int sqrtsGreen = 0;
            int sqrtsRed = 0;

            // int arr[2][3] = {{0,0,0},{0,0,0}}

            // iterate over an 3x3 grid of pixels
            for (int k = i - 1; k <= i + 1; k++)
            {
                // avoiding the borders
                if (k >= 0 && k < height)
                {
                    for (int m = j - 1; m <= j + 1; m++)
                    {
                        if (m >= 0 && m < width)
                        {
                            gxSumBlue += copy[k][m].rgbtBlue * GX[k - i + 1][m - j + 1];
                            gxSumGreen += copy[k][m].rgbtGreen * GX[k - i + 1][m - j + 1];
                            gxSumRed += copy[k][m].rgbtRed * GX[k - i + 1][m - j + 1];

                            gySumBlue += copy[k][m].rgbtBlue * GY[k - i + 1][m - j + 1];
                            gySumGreen += copy[k][m].rgbtGreen * GY[k - i + 1][m - j + 1];
                            gySumRed += copy[k][m].rgbtRed * GY[k - i + 1][m - j + 1];
                        }
                    }
                }
            }

            sqrtsBlue = round(sqrt((gxSumBlue * gxSumBlue) + (gySumBlue * gySumBlue)));
            sqrtsGreen = round(sqrt((gxSumGreen * gxSumGreen) + (gySumGreen * gySumGreen)));
            sqrtsRed = round(sqrt((gxSumRed * gxSumRed) + (gySumRed * gySumRed)));


            image[i][j].rgbtBlue = (sqrtsBlue > 255) ? 255 : sqrtsBlue;
            image[i][j].rgbtGreen = (sqrtsGreen > 255) ? 255 : sqrtsGreen;
            image[i][j].rgbtRed = (sqrtsRed > 255) ? 255 : sqrtsRed;
        }
    }
    return;
}

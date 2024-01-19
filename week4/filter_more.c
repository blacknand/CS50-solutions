#include "helpers.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int avg_red = image[i][j].rgbtRed;
            int avg_green = image[i][j].rgbtGreen;
            int avg_blue = image[i][j].rgbtBlue;
            int avg = round((avg_red + avg_green + avg_blue) / 3.0);
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
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
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy the original image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Blur the pixels in copy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int totalRed, totalGreen, totalBlue;
            totalRed = totalBlue = totalGreen = 0;
            float counter = 0.0;
            // Create a 9x9 grid that will move around the image
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    int currentX = i + x;
                    int currentY = j + y;
                    // If the current pixel is an edge/corner case
                    if (currentX < 0 || currentX > (height - 1) || currentY < 0 || currentY > (width - 1))
                    {
                        continue;
                    }
                    counter++;
                    totalRed += image[currentX][currentY].rgbtRed;
                    totalGreen += image[currentX][currentY].rgbtGreen;
                    totalBlue += image[currentX][currentY].rgbtBlue;
                }
                // Assign the average back to the current pixel
                copy[i][j].rgbtRed = fmin(round(totalRed / counter), 255);
                copy[i][j].rgbtGreen = fmin(round(totalGreen / counter), 255);
                copy[i][j].rgbtBlue = fmin(round(totalBlue / counter), 255);
            }
        }
    }

    // Assign the copy pixels to image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Copy the original image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int GxTotalRed, GxTotalGreen, GxTotalBlue;
            GxTotalRed = GxTotalBlue = GxTotalGreen = 0;
            int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

            // Convolutional matrix Gx
            for (int x = -1; x < 2; x++)
            {
                for (int y = -1; y < 2; y++)
                {
                    int currentX = x + i;
                    int currentY = y + j;
                    if (currentX < 0 || currentX > (height - 1) || currentY < 0 || currentY > (width - 1))
                    {
                        continue;
                    }
                    GxTotalRed += Gx[x + 1][y + 1] * (image[currentX][currentY].rgbtRed);
                    GxTotalGreen += Gx[x + 1][y + 1] * (image[currentX][currentY].rgbtGreen);
                    GxTotalBlue += Gx[x + 1][y + 1] * (image[currentX][currentY].rgbtBlue);
                }
            }

            int GyTotalRed, GyTotalGreen, GyTotalBlue;
            GyTotalRed = GyTotalBlue = GyTotalGreen = 0;
            int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

            // Convolutional matrix Gy
            for (int y = -1; y < 2; y++)
            {
                for (int x = -1; x < 2; x++)
                {
                    int currentY = i + y;
                    int currentX = j + x;
                    if (currentY < 0 || currentY > (height - 1) || currentX < 0 || currentX > (width - 1))
                    {
                        continue;
                    }
                    GyTotalRed += Gy[y + 1][x + 1] * (image[currentY][currentX].rgbtRed);
                    GyTotalGreen += Gy[y + 1][x + 1] * (image[currentY][currentX].rgbtGreen);
                    GyTotalBlue += Gy[y + 1][x + 1] * (image[currentY][currentX].rgbtBlue);
                }
            }

            copy[i][j].rgbtRed = fmin(round(sqrt(pow(GxTotalRed, 2) + pow(GyTotalRed, 2))), 255);
            copy[i][j].rgbtGreen = fmin(round(sqrt(pow(GxTotalGreen, 2) + pow(GyTotalGreen, 2))), 255);
            copy[i][j].rgbtBlue = fmin(round(sqrt(pow(GxTotalBlue, 2) + pow(GyTotalBlue, 2))), 255);
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = copy[i][j];
        }
    }
    return;
}

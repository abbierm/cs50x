#include "helpers.h"
#include <math.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    //iterates through height
    for (int i = 0; i < height; i ++)
    {
        // Iterates through width
        for (int j = 0; j < width; j++)
        {
            // Add all the values from the struct
            int red = image[i][j].rgbtRed;
            int blue = image[i][j].rgbtBlue;
            int green = image[i][j].rgbtGreen;

            int sum = red + blue + green;

            float average_unrounded = sum / 3.0;
            int average = round(average_unrounded);

            image[i][j].rgbtRed = average;
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through the width
    for (int h = 0; h < height; h++)
    {
        // Iterate through the height
        for (int w = 0; w < width; w++)
        {
            // Get the original int values for the rgb
            int original_red = image[h][w].rgbtRed;
            int original_blue = image[h][w].rgbtBlue;
            int original_green = image[h][w].rgbtGreen;

            // Use the sepia formulas to get the new colors for filter
            int sepia_red = round(.393 * original_red + .769 * original_green + .189 * original_blue);

            if (sepia_red > 255)
            {
                sepia_red = 255;
            }
            image[h][w].rgbtRed = sepia_red;

            // Green
            int sepia_green = round(.349 * original_red + .686 * original_green + .168 * original_blue);

            if (sepia_green > 255)
            {
                sepia_green = 255;
            }
            image[h][w].rgbtGreen = sepia_green;

            // Blue
            int sepia_blue = round(.272 * original_red + .534 * original_green + .131 * original_blue);

            if (sepia_blue > 255)
            {
                sepia_blue = 255;
            }
            image[h][w].rgbtBlue = sepia_blue;
        }
    }
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through each row (height)
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < (width / 2); w++)
        {
            // Swap h[i] with h[-(i + 1)]
            RGBTRIPLE tmp = image[h][w];
            image[h][w] = image[h][width - 1 - w];
            image[h][width - 1 - w] = tmp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Make a copy of the image
    RGBTRIPLE copy[height][width];

    // Iterate through the original image to fill in the image's pixel's for the copy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Iterate through the original image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Set Average Variables
            float average_red_unrounded;
            float average_blue_unrounded;
            float average_green_unrounded;

            // Total sums of different values
            int total_red = 0;
            int total_blue = 0;
            int total_green = 0;

            // Divide By Variable
            float divide_by = 0.0;

            // Access each pixel and add to respective sums

            // Conditional for top left
            if (h != 0 && w != 0)
            {
                // Top left [h-1][w-1]
                total_red += copy[h - 1][w - 1].rgbtRed;
                total_blue += copy[h - 1][w - 1].rgbtBlue;
                total_green += copy[h - 1][w - 1].rgbtGreen;
                divide_by++;
            }

            // Conditional for top
            if (h != 0)
            {
                // Top [h-1][w]
                total_red += copy[h - 1][w].rgbtRed;
                total_blue += copy[h - 1][w].rgbtBlue;
                total_green += copy[h - 1][w].rgbtGreen;
                divide_by++;
            }

            // Conditional for top right
            if (h != 0 && (w + 1) < width)
            {
                // Top Right [h-1][w+1]
                total_red += copy[h - 1][w + 1].rgbtRed;
                total_blue += copy[h - 1][w + 1].rgbtBlue;
                total_green += copy[h - 1][w + 1].rgbtGreen;
                divide_by++;
            }

            // Conditional for left
            if (w != 0)
            {
                // Left [h][w-1]
                total_red += copy[h][w - 1].rgbtRed;
                total_blue += copy[h][w - 1].rgbtBlue;
                total_green += copy[h][w - 1].rgbtGreen;
                divide_by++;
            }

            // center
            total_red += copy[h][w].rgbtRed;
            total_blue += copy[h][w].rgbtBlue;
            total_green += copy[h][w].rgbtGreen;
            divide_by++;

            // Conditional for right
            if ((w + 1) < width)
            {
                // Right [h][w+1]
                total_red += copy[h][w + 1].rgbtRed;
                total_blue += copy[h][w + 1].rgbtBlue;
                total_green += copy[h][w + 1].rgbtGreen;
                divide_by++;
            }

            // Conditional for bottom left
            if (w != 0 && (h + 1) < height)
            {
                // Bottom Left [h+1][w-1]
                total_red += copy[h + 1][w - 1].rgbtRed;
                total_blue += copy[h + 1][w - 1].rgbtBlue;
                total_green += copy[h + 1][w - 1].rgbtGreen;
                divide_by++;
            }

            // Conditional for bottom [h + 1][w]
            if (h + 1 < height)
            {
                // Bottom [h+1][w]
                total_red += copy[h + 1][w].rgbtRed;
                total_blue += copy[h + 1][w].rgbtBlue;
                total_green += copy[h + 1][w].rgbtGreen;
                divide_by++;
            }

            // Conditional for bottom right
            if (h + 1 < height && w + 1 < width)
            {
                // Bottom Right [h+1][w+1]
                total_red += copy[h + 1][w + 1].rgbtRed;
                total_blue += copy[h + 1][w + 1].rgbtBlue;
                total_green += copy[h + 1][w + 1].rgbtGreen;
                divide_by++;
            }

            // Get Averages for each color
            // Red
            average_red_unrounded = total_red / divide_by;
            int average_red = round(average_red_unrounded);

            // Blue
            average_blue_unrounded = total_blue / divide_by;
            int average_blue = round(average_blue_unrounded);

            // Green
            average_green_unrounded = total_green / divide_by;
            int average_green = round(average_green_unrounded);

            // Set new values for center pixel
            image[h][w].rgbtRed = average_red;
            image[h][w].rgbtBlue = average_blue;
            image[h][w].rgbtGreen = average_green;
        }
    }

    return;
}

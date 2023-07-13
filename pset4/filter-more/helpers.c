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

// Function that checks the output to make sure doesn't go over 255
int ceiling(int rgb_value)
{
    if (rgb_value > 255)
    {
        rgb_value = 255;
        return rgb_value;
    }
    else
    {
        return rgb_value;
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of the original image to use for making edges
    RGBTRIPLE copy[height][width];

    // Iterates through image pixels and copies RBG values to copy
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    // Iterate through the actural image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // Gx and Gy for each RGB around im[h][w]

            // Counting Variables
            float gx_sum_red = 0.0;
            float gy_sum_red = 0.0;

            float gx_sum_blue = 0.0;
            float gy_sum_blue = 0.0;

            float gx_sum_green = 0.0;
            float gy_sum_green = 0.0;

            int red;
            int blue;
            int green;

            // For loops that iterates through height of 9 pixels around image pixels
            for (int i = -1; i < 2; i++)
            {
                // Conditional that makes sure isn't top or bottom of picture
                if (i + h < 0 || i + h > height - 1)
                {
                    continue;
                }
                // For loop that iterates through width of 9 pixels surrounding image
                for (int j = -1; j < 2; j++)
                {
                    // Conditional the makes sure the width isn't right or left of bounds
                    if (j + w < 0 || j + w > width - 1)
                    {
                        continue;
                    }
                    // Calculates what to multiply by
                    float mult_x = i * (2 - abs(j));
                    float mult_y = j * (2 - abs(i));

                    // Calculates the gx values
                    gx_sum_red += mult_x * copy[h + i][w + j].rgbtRed;
                    gx_sum_blue += mult_x * copy[h + i][w + j].rgbtBlue;
                    gx_sum_green += mult_x * copy[h + i][w + j].rgbtGreen;

                    // Calculates the gy values
                    gy_sum_red += mult_y * copy[h + i][w + j].rgbtRed;
                    gy_sum_blue += mult_y * copy[h + i][w + j].rgbtBlue;
                    gy_sum_green += mult_y * copy[h + i][w + j].rgbtGreen;

                }
            }
            // Calculates the new pixel value for image[h][w]

            // Red
            red = ceiling(round(sqrt(gx_sum_red * gx_sum_red + gy_sum_red * gy_sum_red)));
            image[h][w].rgbtRed = red;

            // Blue
            blue = ceiling(round(sqrt(gx_sum_blue * gx_sum_blue + gy_sum_blue * gy_sum_blue)));
            image[h][w].rgbtBlue = blue;

            // Green
            green = ceiling(round(sqrt(gx_sum_green * gx_sum_green + gy_sum_green * gy_sum_green)));
            image[h][w].rgbtGreen = green;

        }
    }
    return;
}

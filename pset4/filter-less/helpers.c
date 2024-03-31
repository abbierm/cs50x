#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through all of the rows [x][0]
    for (int i = 0; i < height; i++)
    {
        // Iterate through all of the columns [0][x]
        for (int j = 0; j < width; j++)
        {
            // Get average of the pixel's RGB values
            float average =
                (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;
            int avg = round(average);

            // Set all the RGB values to that value
            image[i][j].rgbtBlue = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtRed = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate through all of the pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // TODO: Convert Values to integers
            int ogRed = image[i][j].rgbtRed;
            int ogGreen = image[i][j].rgbtGreen;
            int ogBlue = image[i][j].rgbtBlue;

            // Find the red value
            float red_float = .393 * ogRed + .769 * ogGreen + .189 * ogBlue;
            int red = round(red_float);
            if (red > 255)
            {
                red = 255;
            }

            // Find the Green Sepia Value
            float green_float = .349 * ogRed + .686 * ogGreen + .168 * ogBlue;
            int green = round(green_float);
            if (green > 255)
            {
                green = 255;
            }

            // Find the Blue Sepia Value
            float blue_float = .272 * ogRed + .534 * ogGreen + .131 * ogBlue;
            int blue = round(blue_float);
            if (blue > 255)
            {
                blue = 255;
            }

            // Set all of the new values.
            image[i][j].rgbtRed = red;
            image[i][j].rgbtGreen = green;
            image[i][j].rgbtBlue = blue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Iterate over all of the pixels
    for (int h = 0; h < height; h++)
    {
        for (int w = 0, half_width = width / 2; w < half_width; w++)
        {
            // Capture the current pixel in a temp value
            int tempRed = image[h][w].rgbtRed, tempGreen = image[h][w].rgbtGreen,
                tempBlue = image[h][w].rgbtBlue;

            // Get mirrored value for current pixels
            image[h][w].rgbtRed = image[h][width - w - 1].rgbtRed;
            image[h][w].rgbtBlue = image[h][width - w - 1].rgbtBlue;
            image[h][w].rgbtGreen = image[h][width - w - 1].rgbtGreen;

            // Make temp values the mirrored pixels values
            image[h][width - w - 1].rgbtRed = tempRed;
            image[h][width - w - 1].rgbtGreen = tempGreen;
            image[h][width - w - 1].rgbtBlue = tempBlue;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Make copy of the original image
    RGBTRIPLE copy[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            copy[i][j].rgbtRed = image[i][j].rgbtRed;
            copy[i][j].rgbtBlue = image[i][j].rgbtBlue;
            copy[i][j].rgbtGreen = image[i][j].rgbtGreen;
        }
    }

    // Iterate through the image
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            int red_sum = 0, green_sum = 0, blue_sum = 0;
            float counter = 0.0;

            for (int a = -1; a < 2; a++)
            {
                for (int b = -1; b < 2; b++)
                {
                    if ((a + h) >= 0 && (a + h) <= height - 1 && (b + w) >= 0 &&
                        (b + w <= width - 1))
                    {
                        red_sum += copy[h + a][w + b].rgbtRed;
                        green_sum += copy[h + a][w + b].rgbtGreen;
                        blue_sum += copy[h + a][w + b].rgbtBlue;
                        counter += 1;
                    }
                }
            }
            image[h][w].rgbtRed = round(red_sum / counter);
            image[h][w].rgbtGreen = round(green_sum / counter);
            image[h][w].rgbtBlue = round(blue_sum / counter);
        }
    }
    return;
}

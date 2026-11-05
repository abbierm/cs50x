#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>


// TODO: Get Rid of this!!!
void print_rgbtriple(RGBTRIPLE pixel) {
    BYTE red = pixel.rgbtRed;
    BYTE green = pixel.rgbtGreen;
    BYTE blue = pixel.rgbtBlue;

    printf("RED: %d\tBLUE: %d\tGREEN: %d\n", red, blue, green);
}


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{   
    // Loop over all the pixels
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Find average of all the color
            float avg_float = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;

            // Convert back to short int
            BYTE avg_byte = round(avg_float);

            // Set values to average
            image[i][j].rgbtBlue = avg_byte;
            image[i][j].rgbtGreen = avg_byte;
            image[i][j].rgbtRed = avg_byte;
        }
    }
    return;
}

float find_min(int a, int b) 
{
    float min_value = (a <= b) ? a : b; 
    return min_value;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{

    // Loop over all the pixels
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            RGBTRIPLE pixel = image[h][w];

            BYTE red = pixel.rgbtRed;
            BYTE green = pixel.rgbtGreen;
            BYTE blue = pixel.rgbtBlue;

            int sred = round(.393 * red + .769 * green + .189 * blue);
            int sblue = round(.272 * red + .534 * green + .131 * blue);
            int sgreen = round(.349 * red + .686 * green + .168 * blue);
            

            image[h][w].rgbtRed = find_min(sred, 255);
            image[h][w].rgbtBlue = find_min(sblue, 255);
            image[h][w].rgbtGreen = find_min(sgreen, 255);
        }
    }

    return;
}

void swap(RGBTRIPLE *a, RGBTRIPLE *b) {
    RGBTRIPLE tmp = *a;
    *a = *b;
    *b = tmp;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{

    // Loop over all of the pixels
    for (int h = 0; h < height; h++) {

        for (int w = 0; w < width / 2; w++) {
        
            swap(&image[h][w], &image[h][(width - 1) - w]);
        }
    }
    return;
}

// Sobel calculation 
int calculate_sobel(int gx, int gy) {
    return (int) round(sqrt(pow((double) gx, 2.0) + pow((double) gy, 2.0)));
}


// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]) {

    // create a copy
    RGBTRIPLE cpy[height][width]; 
    for (int h = 0; h < height; h++) {
        for (int w = 0; w < width; w++) {
            cpy[h][w] = image[h][w];
        }
    }

    // iterate through the image's height and width
    for (int h = 0; h < height; h++) 
    {
        for (int w = 0; w < width; w++) 
        {
            // 
            int rsum = 0;
            int bsum = 0;
            int gsum = 0;
            float total = 0.0;
            
            // iterate around current pixel
            for (int i = -1; i < 2; i++)
            {
                for (int j = -1; j < 2; j++) 
                {
                    if (
                        (h + i) >= 0 && 
                        (h + i) < (height - 1) && 
                        (w + j) >= 0 && 
                        (w + j) < (width - 1)
                    ) {
                        
                        RGBTRIPLE cur = cpy[h + i][w + j];
                        total += 1;
                        rsum += cur.rgbtRed;
                        bsum += cur.rgbtBlue;
                        gsum += cur.rgbtGreen;
                    }
                }
            }

            // Get the average
            image[h][w].rgbtRed = round((rsum / total));
            image[h][w].rgbtBlue = round((bsum / total));
            image[h][w].rgbtGreen = round((gsum / total));
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{

    // Make a copy of the original image
    RGBTRIPLE cpy[height][width];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            cpy[i][j] = image[i][j];
        }
    }

    // Iterate through the height of image with h
    for (int h = 0; h < height; h++) {

        // Iterate through the width of image with w
        for (int w = 0; w < width; w++) {
            
            // Copy the current pixel into a variable
            RGBTRIPLE cur = cpy[h][w]; 

            // Create sxred, sxgreen, & sxblue and syred, sygreen, & syblue int variables to store sums of sobel
            int sxred = 0, sxgreen = 0, sxblue = 0;
            int syred = 0, sygreen = 0, syblue = 0;
            
            // Iterate through the rows around with i = -1 until i = 2
            for (int i = -1; i < 2; i++) {
                
                // Iterate through the columns with j = -1 until j = 2
                for (int j = -1; j < 2; j++) {
                    
                    // Check if current kernel pixel is an edge
                    if (i + h >= 0 || i + h < height || j + w >= 0 || j + w < width) {

                        // Compute x kernel Sobel value
                        float multx = i * (2 - abs(j));
                        sxred += cpy[i + h][j + w].rgbtRed * multx;
                        sxgreen += cpy[i + h][j + w].rgbtGreen * multx;
                        sxblue += cpy[i + h][j + w].rgbtBlue * multx;

                        // Compute y kernal Sobel values
                        float multy = j * (2 - abs(i));
                        syred += cpy[i + h][j + w].rgbtRed * multy;
                        sygreen += cpy[i + h][j + w].rgbtGreen * multy;
                        syblue += cpy[i + h][j + w].rgbtBlue * multy;

                    }    
                }
            }

            // Update image's values
            image[h][w].rgbtRed = find_min(calculate_sobel(sxred, syred), 255);
            image[h][w].rgbtGreen = find_min(calculate_sobel(sxgreen, sygreen), 255);
            image[h][w].rgbtBlue = find_min(calculate_sobel(sxblue, syblue), 255);
        }
    }

    return;
}
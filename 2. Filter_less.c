#include "helpers.h"
#include <math.h>
#include <stdio.h>

float range(float value);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{

    // loop through pixels and rows in image array
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // calculate the average pixel value
            // round to whole number
            BYTE average = round((image[h][w].rgbtBlue + image[h][w].rgbtGreen + image[h][w].rgbtRed) / 3.0);

            // set each colour value to the average value
            image[h][w].rgbtRed = average;
            image[h][w].rgbtGreen = average;
            image[h][w].rgbtBlue = average;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    // loop through pixels and rows in image array
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // copy original colour values
            int originalRed = image[h][w].rgbtRed;
            int originalGreen = image[h][w].rgbtGreen;
            int originalBlue = image[h][w].rgbtBlue;

            // set each colour value to the sepia value
            // use function range - to check if sepia value is in range

            // Red
            image[h][w].rgbtRed = (float)round(range((0.393 * originalRed) + (0.769 * originalGreen) + (0.189 * originalBlue)));

            // Green
            image[h][w].rgbtGreen = (float)round(range((0.349 * originalRed) + (0.686 * originalGreen) + (0.168 * originalBlue)));

            // Blue
            image[h][w].rgbtBlue = (float)round(range((0.272 * originalRed) + (0.534 * originalGreen) + (0.131 * originalBlue)));;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // move pixels to temporary array in reverse order

    // define temp array
    RGBTRIPLE temp[height][width];

    // loop through pixels and rows - filling the temp array
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            temp[h][w] = image[h][width - 1 - w];
        }
    }

    // copy pixels from temporary array - back to original array
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = temp[h][w];
        }
    }

    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // define temp array
    RGBTRIPLE temp[height][width];

    // loop through pixels and rows - filling the temp array
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // define sum values for average
            float sumRed = 0;
            float sumGreen = 0;
            float sumBlue = 0;

            // define pixel counter to check size of average array
            float pixel_counter = 0;

            // define pixel direction for ease of reading
            int left = w - 1;
            int right = w + 1;
            int top = h - 1;
            int bottom = h + 1;

            // collect values for surroudning cells in array
            for (int i = 0; i < 9; i++)
            {

            // add top row of pixels to average sum

                // add top left pixel to average sum
                if ((i == 0) && (h > 0) && (w > 0))
                {
                    sumRed = sumRed + image[top][left].rgbtRed;
                    sumGreen = sumGreen + image[top][left].rgbtGreen;
                    sumBlue = sumBlue + image[top][left].rgbtBlue;
                    pixel_counter++;
                }

                // add top middle pixel to average sum
                if ((i == 1) && (h > 0))
                {
                    sumRed = sumRed + image[top][w].rgbtRed;
                    sumGreen = sumGreen + image[top][w].rgbtGreen;
                    sumBlue = sumBlue + image[top][w].rgbtBlue;
                    pixel_counter++;
                }

                // add top right pixel to average sum
                if ((i == 2) && (h > 0) && (w < (width - 1)))
                {
                    sumRed = sumRed + image[top][right].rgbtRed;
                    sumGreen = sumGreen + image[top][right].rgbtGreen;
                    sumBlue = sumBlue + image[top][right].rgbtBlue;
                    pixel_counter++;
                }

            // add middle row of pixels to average sum

                // add middle left pixel to average sum
                if ((i == 3) && (w > 0))
                {
                    sumRed = sumRed + image[h][left].rgbtRed;
                    sumGreen = sumGreen + image[h][left].rgbtGreen;
                    sumBlue = sumBlue + image[h][left].rgbtBlue;
                    pixel_counter++;
                }

                // add centre pixel to average sum
                if (i == 4)
                {
                    sumRed = sumRed + image[h][w].rgbtRed;
                    sumGreen = sumGreen + image[h][w].rgbtGreen;
                    sumBlue = sumBlue + image[h][w].rgbtBlue;
                    pixel_counter++;
                }

                // add middle right pixel to average sum
                if ((i == 5) && (w < (width - 1)))
                {
                    sumRed = sumRed + image[h][right].rgbtRed;
                    sumGreen = sumGreen + image[h][right].rgbtGreen;
                    sumBlue = sumBlue + image[h][right].rgbtBlue;
                    pixel_counter++;
                }

            // add bottom row of pixels to average sum

                // add bottom left pixel to average sum
                if ((i == 6) && (h < (height - 1)) && (w > 0))
                {
                    sumRed = sumRed + image[bottom][left].rgbtRed;
                    sumGreen = sumGreen + image[bottom][left].rgbtGreen;
                    sumBlue = sumBlue + image[bottom][left].rgbtBlue;
                    pixel_counter++;
                }

                // add bottom middle pixel to average sum
                if ((i == 7) && (h < (height - 1)))
                {
                    sumRed = sumRed + image[bottom][w].rgbtRed;
                    sumGreen = sumGreen + image[bottom][w].rgbtGreen;
                    sumBlue = sumBlue + image[bottom][w].rgbtBlue;
                    pixel_counter++;
                }

                // add bottom right pixel to average sum
                if ((i == 8) && (h < (height - 1)) && (w < (width - 1)))
                {
                    sumRed = sumRed + image[bottom][right].rgbtRed;
                    sumGreen = sumGreen + image[bottom][right].rgbtGreen;
                    sumBlue = sumBlue + image[bottom][right].rgbtBlue;
                    pixel_counter++;
                }
            }

            // calculate the average pixel value
            // assign to temporary array - round to whole number
            temp[h][w].rgbtRed = round(sumRed / pixel_counter);
            temp[h][w].rgbtGreen = round(sumGreen / pixel_counter);
            temp[h][w].rgbtBlue = round(sumBlue / pixel_counter);
        }
    }

    // copy pixels from temporary array - back to original array
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            image[h][w] = temp[h][w];
        }
    }

    return;
}

// Function used in Sepia
float range(float value)
{
    // if value is below range - return minimum value
    if (value < 0)
    {
        return 0;
    }

    // if value is above range - return maximum value
    else if (value > 255)
    {
        return 255;
    }

    // if value is in range - return same value (no change)
    else
    {
        return value;
    }
}

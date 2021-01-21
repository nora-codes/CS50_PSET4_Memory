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

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // define temp array
    RGBTRIPLE temp[height][width];

    // loop through pixels and rows - filling the temp array
    for (int h = 0; h < height; h++)
    {
        for (int w = 0; w < width; w++)
        {
            // define Gx sum values for final Sobel value
            float Gx_sumRed = 0;
            float Gx_sumGreen = 0;
            float Gx_sumBlue = 0;

            // define Gxy sum values for final Sobel value
            float Gy_sumRed = 0;
            float Gy_sumGreen = 0;
            float Gy_sumBlue = 0;

            // define pixel direction for ease of reading
            int left = w - 1;
            int right = w + 1;
            int top = h - 1;
            int bottom = h + 1;

            // collect values for surroudning cells in array
            for (int i = 0; i < 9; i++)
            {
                // add top row of pixels to Gx and Gy sum

                // add top left pixel to sum
                if ((i == 0) && (h > 0) && (w > 0))
                {
                    Gx_sumRed = Gx_sumRed + (image[top][left].rgbtRed * (-1));
                    Gx_sumGreen = Gx_sumGreen + (image[top][left].rgbtGreen * (-1));
                    Gx_sumBlue = Gx_sumBlue + (image[top][left].rgbtBlue * (-1));
                    Gy_sumRed = Gy_sumRed + (image[top][left].rgbtRed * (-1));
                    Gy_sumGreen = Gy_sumGreen + (image[top][left].rgbtGreen * (-1));
                    Gy_sumBlue = Gy_sumBlue + (image[top][left].rgbtBlue * (-1));
                }

                // add top middle pixel to sum
                if ((i == 1) && (h > 0))
                {
                    Gx_sumRed = Gx_sumRed + (image[top][w].rgbtRed * 0);
                    Gx_sumGreen = Gx_sumGreen + (image[top][w].rgbtGreen * 0);
                    Gx_sumBlue = Gx_sumBlue + (image[top][w].rgbtBlue * 0);
                    Gy_sumRed = Gy_sumRed + (image[top][w].rgbtRed * (-2));
                    Gy_sumGreen = Gy_sumGreen + (image[top][w].rgbtGreen * (-2));
                    Gy_sumBlue = Gy_sumBlue + (image[top][w].rgbtBlue * (-2));
                }

                // add top right pixel to sum
                if ((i == 2) && (h > 0) && (w < (width - 1)))
                {
                    Gx_sumRed = Gx_sumRed + (image[top][right].rgbtRed * 1);
                    Gx_sumGreen = Gx_sumGreen + (image[top][right].rgbtGreen * 1);
                    Gx_sumBlue = Gx_sumBlue + (image[top][right].rgbtBlue * 1);
                    Gy_sumRed = Gy_sumRed + (image[top][right].rgbtRed * (-1));
                    Gy_sumGreen = Gy_sumGreen + (image[top][right].rgbtGreen * (-1));
                    Gy_sumBlue = Gy_sumBlue + (image[top][right].rgbtBlue * (-1));
                }

                // add middle row of pixels to Gx and Gysum

                // add middle left pixel to sum
                if ((i == 3) && (w > 0))
                {
                    Gx_sumRed = Gx_sumRed + (image[h][left].rgbtRed * (-2));
                    Gx_sumGreen = Gx_sumGreen + (image[h][left].rgbtGreen * (-2));
                    Gx_sumBlue = Gx_sumBlue + (image[h][left].rgbtBlue * (-2));
                    Gy_sumRed = Gy_sumRed + (image[h][left].rgbtRed * 0);
                    Gy_sumGreen = Gy_sumGreen + (image[h][left].rgbtGreen * 0);
                    Gy_sumBlue = Gy_sumBlue + (image[h][left].rgbtBlue * 0);
                }

                    // add centre pixel to sum
                if (i == 4)
                {
                    Gx_sumRed = Gx_sumRed + (image[h][w].rgbtRed * 0);
                    Gx_sumGreen = Gx_sumGreen + (image[h][w].rgbtGreen * 0);
                    Gx_sumBlue = Gx_sumBlue + (image[h][w].rgbtBlue * 0);
                    Gy_sumRed = Gy_sumRed + (image[h][w].rgbtRed * 0);
                    Gy_sumGreen = Gy_sumGreen + (image[h][w].rgbtGreen * 0);
                    Gy_sumBlue = Gy_sumBlue + (image[h][w].rgbtBlue * 0);
                }

                // add middle right pixel to sum
                if ((i == 5) && (w < (width - 1)))
                {
                    Gx_sumRed = Gx_sumRed + (image[h][right].rgbtRed * 2);
                    Gx_sumGreen = Gx_sumGreen + (image[h][right].rgbtGreen * 2);
                    Gx_sumBlue = Gx_sumBlue + (image[h][right].rgbtBlue * 2);
                    Gy_sumRed = Gy_sumRed + (image[h][right].rgbtRed * 0);
                    Gy_sumGreen = Gy_sumGreen + (image[h][right].rgbtGreen * 0);
                    Gy_sumBlue = Gy_sumBlue + (image[h][right].rgbtBlue * 0);
                }

                // add bottom row of pixels to Gx and Gy sum

                // add bottom left pixel to sum
                if ((i == 6) && (h < (height - 1)) && (w > 0))
                {
                    Gx_sumRed = Gx_sumRed + (image[bottom][left].rgbtRed * (-1));
                    Gx_sumGreen = Gx_sumGreen + (image[bottom][left].rgbtGreen * (-1));
                    Gx_sumBlue = Gx_sumBlue + (image[bottom][left].rgbtBlue * (-1));
                    Gy_sumRed = Gy_sumRed + (image[bottom][left].rgbtRed * 1);
                    Gy_sumGreen = Gy_sumGreen + (image[bottom][left].rgbtGreen * 1);
                    Gy_sumBlue = Gy_sumBlue + (image[bottom][left].rgbtBlue * 1);
                }

                // add bottom middle pixel to sum
                if ((i == 7) && (h < (height - 1)))
                {
                    Gx_sumRed = Gx_sumRed + (image[bottom][w].rgbtRed * 0);
                    Gx_sumGreen = Gx_sumGreen + (image[bottom][w].rgbtGreen * 0);
                    Gx_sumBlue = Gx_sumBlue + (image[bottom][w].rgbtBlue * 0);
                    Gy_sumRed = Gy_sumRed + (image[bottom][w].rgbtRed * 2);
                    Gy_sumGreen = Gy_sumGreen + (image[bottom][w].rgbtGreen * 2);
                    Gy_sumBlue = Gy_sumBlue + (image[bottom][w].rgbtBlue * 2);
                }

                // add bottom right pixel to sum
                if ((i == 8) && (h < (height - 1)) && (w < (width - 1)))
                {
                    Gx_sumRed = Gx_sumRed + (image[bottom][right].rgbtRed * 1);
                    Gx_sumGreen = Gx_sumGreen + (image[bottom][right].rgbtGreen * 1);
                    Gx_sumBlue = Gx_sumBlue + (image[bottom][right].rgbtBlue * 1);
                    Gy_sumRed = Gy_sumRed + (image[bottom][right].rgbtRed * 1);
                    Gy_sumGreen = Gy_sumGreen + (image[bottom][right].rgbtGreen * 1);
                    Gy_sumBlue = Gy_sumBlue + (image[bottom][right].rgbtBlue * 1);
                }
            }

            // calculate the average pixel value
            // assign to temporary array - round to whole number
            temp[h][w].rgbtRed = round(range(sqrt((Gx_sumRed * Gx_sumRed) + (Gy_sumRed * Gy_sumRed))));
            temp[h][w].rgbtGreen = round(range(sqrt((Gx_sumGreen * Gx_sumGreen) + (Gy_sumGreen * Gy_sumGreen))));
            temp[h][w].rgbtBlue = round(range(sqrt((Gx_sumBlue * Gx_sumBlue) + (Gy_sumBlue * Gy_sumBlue))));

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

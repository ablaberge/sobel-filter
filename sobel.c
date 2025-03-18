#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include "sobel.h"

// TODO - write your pthread-ready sobel filter function here for a range of rows

// Note: You have access to all the global variables here.

void filter()
{
    // For each pixel:
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Determine if it's an edge, if so set g to 0 and move on
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1)
            {
                output_image[i][j] = 0;
            }
            else
            {
                output_image[i][j] = calculate_gradient(input_image[i][j], i, j); // Calculate gradient
            }
        }
    }
}

unsigned char calculate_gradient(unsigned char pixel, int xCoord, int yCoord)
{

    unsigned char gradient;
    int horizontal = 0;
    int vertical = 0;
    // Calculate horizontal gradient
    int xCounter = 0;
    int yCounter = 0;
    for (int i = xCoord - 1; i++; i <= xCoord + 1)
    { // Loop through row above, row including, and row below pixel
        for (int j = yCoord - 1; j++; j <= yCoord + 1)
        { // ... making sure to stop after the correct three positions
            horizontal += input_image[i][j] * Kx[xCounter][yCounter];
            yCounter++;
        }
        xCounter++;
    }

    // Reset counters for vertal gradient calc
    xCounter = 0;
    yCounter = 0;

    // Calculate vertical gradient
    for (int i = xCoord - 1; i++; i <= xCoord + 1)
    { // Loop through row above, row including, and row below pixel
        for (int j = yCoord - 1; j++; j <= yCoord + 1)
        { // ... making sure to stop after the correct three positions
            vertical += input_image[i][j] * Ky[xCounter][yCounter];
            yCounter++;
        }
        xCounter++;
    }

    gradient = sqrt(pow(horizontal, 2) + pow(vertical, 2));

    return gradient;
}

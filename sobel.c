#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include "sobel.h"
#include <string.h>

// TODO - write your pthread-ready sobel filter function here for a range of rows

// Note: You have access to all the global variables here.

void filter()
{
    output_image = (unsigned char **)malloc(sizeof(unsigned char *) * height);
    for (int i = 0; i < height; i++)
    {
        output_image[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }
    
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
    for (int i = xCoord - 1; i <= xCoord + 1; i++)
    { // Loop through row above, row including, and row below pixel
        int yCounter = 0;
        for (int j = yCoord - 1; j <= yCoord + 1; j++)
        { // ... making sure to stop after the correct three positions
            horizontal += input_image[i][j] * Kx[xCounter][yCounter];
            yCounter++;
        }
        xCounter++;
    }

    // Reset counters for vertal gradient calc
    xCounter = 0;

    // Calculate vertical gradient
    for (int i = xCoord - 1; i <= xCoord + 1; i++)
    { // Loop through row above, row including, and row below pixel
        int yCounter = 0;
        for (int j = yCoord - 1; j <= yCoord + 1; j++)
        { // ... making sure to stop after the correct three positions
            vertical += input_image[i][j] * Ky[xCounter][yCounter];
            yCounter++;
        }
        xCounter++;
    }

    gradient = sqrt(pow(horizontal, 2) + pow(vertical, 2));
    if (gradient < threshold) { // Set pixels below the threshold to 0
        gradient = 0; 
    }

    return gradient;
}

char *get_output_filename(const char *input_filename) {

    const char *dot = strrchr(input_filename, '.');
    
    size_t basename_length = dot - input_filename;
    size_t extension_length = strlen(dot); // Includes the dot
    
    char *output_filename = malloc(basename_length + 6 + extension_length + 1);
    
    // Copy the base name portion
    strncpy(output_filename, input_filename, basename_length);
    
    strcpy(output_filename + basename_length, "-sobel");
    strcpy(output_filename + basename_length + 6, dot);
    
    return output_filename;
}
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include "sobel.h"
#include <string.h>


void *thread_funct(void *arg)
{
    thread_args *work = (thread_args *)arg;
    int start = work->start_row;
    int end = work->end_row;

    for (int i = start; i < end; i++) // Process assigned rows
    {
        for (int j = 0; j < width; j++)
        {
            if (i == 0 || i == height - 1 || j == 0 || j == width - 1) // Check if we have an edge pixel
            {
                output_image[i][j] = 0;
            }
            else
            {
                output_image[i][j] = calculate_gradient(input_image[i][j], i, j);
            }
        }
    }
    return NULL;
}

void filter()
{
    pthread_t *threads;
    thread_args *thread_work;

    output_image = (unsigned char **)malloc(sizeof(unsigned char *) * height);
    for (int i = 0; i < height; i++)
    {
        output_image[i] = (unsigned char *)malloc(width * sizeof(unsigned char));
    }

    threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));
    thread_work = (thread_args *)malloc(num_threads * sizeof(thread_args));

    int rows_per_thread = height / num_threads;

    for (int i = 0; i < num_threads; i++)
    {
        thread_work[i].start_row = i * rows_per_thread;
        // Last thread may need to handle remaining rows if we have an odd number
        thread_work[i].end_row = (i == num_threads - 1) ? height : (i + 1) * rows_per_thread;
        pthread_create(&threads[i], NULL, thread_funct, (void *)&thread_work[i]); // Create threads with their specified work
    }

    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(threads[i], NULL);
    }

    free(threads);
    free(thread_work);
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
    if (gradient < threshold)
    { // Set pixels below the threshold to 0
        gradient = 0;
    }
    else if (gradient > 255){ // Clamp gradient at 255
        gradient = 255; 
    }

    return gradient;
}

char *get_output_filename(const char *input_filename)
{
    const char *dot = strrchr(input_filename, '.');
    size_t basename_length = dot - input_filename;
    size_t extension_length = strlen(dot); // Includes the dot

    char *output_filename = malloc(basename_length + 6 + extension_length + 1);

    strncpy(output_filename, input_filename, basename_length);
    strcpy(output_filename + basename_length, "-sobel");
    strcpy(output_filename + basename_length + 6, dot); // Did this just incase we have a .png instead of a .jpg

    return output_filename;
}

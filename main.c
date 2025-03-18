#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "sobel.h"
#include "rtclock.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// Sobel kernels
int Kx[3][3] = {
    {-1, 0, 1},
    {-2, 0, 2},
    {-1, 0, 1}};

int Ky[3][3] = {
    {-1, -2, -1},
    {0, 0, 0},
    {1, 2, 1}};

// Globals: Image and threading data
unsigned char **input_image;
unsigned char **output_image;
unsigned char threshold = 127;
int width, height;
int num_threads;

/**
 * Main method
 */
int main(int argc, char *argv[])
{

    // Handle command line inputs
    if (argc < 4)
    {
        printf("Usage: ./sobel <input-file> <num-threads (>= 1)> <threshold (0-255)>\n");
        return -1;
    }

    char *filename = argv[1];
    num_threads = atoi(argv[2]);
    threshold = atoi(argv[3]);

    // Read image file into array a 1D array (see assignment write-up)

    unsigned char *data = stbi_load(filename, &width, &height, NULL, 1);
    if (data == NULL)
    {
        printf("stbi_load failed\n");
        return -1;
    }
    // Remap image 1D array into a 2D array
    input_image = (unsigned char **)malloc(sizeof(unsigned char *) * height);

    for (int i = 0; i < height; i++)
    {
        // assign each row the proper pixel offset
        input_image[i] = &data[i * width];
    }

    printf("Loaded %s. Height=%d, Width=%d\n", filename, height, width);

    // Start clocking!
    double startTime, endTime;
    startTime = rtclock();

    // All threads are created and handled by this single function call 
    filter();

    // End clocking!
    endTime = rtclock();
    printf("Time taken (thread count = %d): %.6f sec\n", num_threads, (endTime - startTime));

    // This code re-maps output_image[][] back down to a 1D array so stbi can be called
    unsigned char *array1D = (unsigned char *)malloc(width * height * sizeof(unsigned char));
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            array1D[i * width + j] = output_image[i][j];
        }
    }

    char *newFilename = get_output_filename(filename);

    // Save the file!
    stbi_write_jpg(newFilename, width, height, 1, array1D, 80);

    // Free allocated memory

    free(input_image);
    input_image = NULL; // dangling pointer

    free(array1D);
    array1D = NULL; // dangling pointer

    for (int i = 0; i < height; i++)
    {
        free(output_image[i]);
        output_image[i] = NULL; // dangling pointer
    }
    free(output_image);
    output_image = NULL; // dangling pointer

    free(newFilename);
    stbi_image_free(data);

    return 0;
}

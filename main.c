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
    num_threads = argv[2];
    threshold = argv[3];

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

    // TODO - Prepare and create threads

    // TODO - Wait for threads to finish

    // End clocking!
    endTime = rtclock();
    printf("Time taken (thread count = %d): %.6f sec\n", num_threads, (endTime - startTime));

    // TODO - Save the file!

    // TODO - Free allocated memory

    return 0;
}

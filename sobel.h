#define BLACK 0
#define WHITE 255

// These globals are declared here, but defined externally in another .c file
extern unsigned char **input_image;
extern unsigned char **output_image;
extern unsigned char threshold;
extern int width, height;
extern int num_threads;
extern int Kx[3][3];
extern int Ky[3][3];

// Thread function argument 
typedef struct thread_args {
  int tid;    // the given thread id (0, 1, ...)
  long start_row; // where in the image to begin sobel calculations
  long end_row;   // where in the image to end sobel calculations
} thread_args;


// Function declarations 
void *thread_funct(void *arg);
void filter();
unsigned char calculate_gradient(unsigned char pixel, int xCoord, int yCoord);
char *get_output_filename(const char *input_filename);

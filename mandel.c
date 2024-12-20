/**********************************************************
* Lab 12
* Filename: mandel.c
* Section: 121
* Description: Creates 50 mandel images using
* multi-processing and multi-threading
* Author: Eric Mahn
* Date: 11/30/24
* Compile Instructions: make 
***********************************************************/
/// 
//  mandel.c
//  Based on example code found here:
//  https://users.cs.fiu.edu/~cpoellab/teaching/cop4610_fall22/project3.html
//
//  Converted to use jpg instead of BMP and other minor changes
//  
///
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include "jpegrw.h"

typedef struct {
	//pointer for the image
	imgRawImage *img; 
	//set the regions
	double xmin;	
	double xmax;
	double ymin;
	double ymax;
	int max;
	int start;
	int end;

}thread_data;

// local routines
static int iteration_to_color( int i, int max );
static int iterations_at_point( double x, double y, int max );
static void compute_image( imgRawImage *img, double xmin, double xmax,
									double ymin, double ymax, int max, int threads );
static void* compute_region(void* arg);
static void show_help();


int main( int argc, char *argv[] )
{
	char c;

	// These are the default configuration values used
	// if no command line arguments are given.
	const char *outfile = "mandel.jpg";
	double xcenter = 0;
	double ycenter = 0;
	double xscale = 4;
	double yscale = 0; // calc later
	int    image_width = 1000;
	int    image_height = 1000;
	int    max = 1000;
	int    children = 10;
	int    threads  = 10;

	// For each command line argument given,
	// override the appropriate configuration value.

	while((c = getopt(argc,argv,"x:y:s:W:H:m:c:t:o:h"))!=-1) {
		switch(c) 
		{
			case 'x':
				xcenter = atof(optarg);
				break;
			case 'y':
				ycenter = atof(optarg);
				break;
			case 's':
				xscale = atof(optarg);
				break;
			case 'W':
				image_width = atoi(optarg);
				break;
			case 'H':
				image_height = atoi(optarg);
				break;
			case 'm':
				max = atoi(optarg);
				break;
			case 'c':
				children = atoi(optarg);
				break;
			case 't':
				threads = atoi(optarg);
				if(threads < 1 || threads > 20) {
					printf("invalid number of threads, defaulting to 10\n");
					threads = 10;
				}
				break;
			case 'o':
				outfile = optarg;
				break;
			case 'h':
				show_help();
				exit(1);
				break;
		}
	}

	int image_count = 50;
	//scale_step is the input scale over the number of output images
    double scale_step = xscale / image_count;

    // Pre-calculate scales so it can be used in the loop
    double scales[image_count];
    for (int i = 0; i < image_count; i++) {
		//gets an equal incrementation for the scale
        scales[i] = xscale - (i * scale_step);
    }

	// Use fork and create images
	for(int i = 0; i < children; i++) {
		pid_t pid = fork();
		if (pid == 0) {
			//find the start increment
			int start = (i * image_count) / children;
			//find the end increment
        	int end = ((i + 1) * image_count) / children;
			

			//iterate through the start and end
			for (int j = start; j < end; j++) {
				
				//grab the current scale
				double scale = scales[j];
				// Calculate y scale based on x scale (settable) and image sizes in X and Y (settable)
				yscale = scale / image_width * image_height;

				// Display the configuration of the image.
				printf("mandel: x=%lf y=%lf xscale=%lf yscale=%1f max=%d outfile=%s\n",xcenter,ycenter,xscale,yscale,max,outfile);

				// Create a raw image of the appropriate size.
				imgRawImage* img = initRawImage(image_width,image_height);

				// Fill it with a black
				setImageCOLOR(img,0);

				// Compute the Mandelbrot image
				compute_image(img,xcenter-scale/2,xcenter+scale/2,ycenter-yscale/2,ycenter+yscale/2,max,threads);

				// Save the image in the stated file.
				char name[100] = "";
				sprintf(name, "%s%d.jpg", outfile, j+1);
				storeJpegImageFile(img,name);

				// free the mallocs
				freeRawImage(img);
			}
			exit(1);
		}
		
	}
	
	//waits for the children to be done
	for (int i = 0; i < children; i++) {
		wait(NULL);
	}
	return 0;
}




/*
Return the number of iterations at point x, y
in the Mandelbrot space, up to a maximum of max.
*/

int iterations_at_point( double x, double y, int max )
{
	double x0 = x;
	double y0 = y;

	int iter = 0;

	while( (x*x + y*y <= 4) && iter < max ) {

		double xt = x*x - y*y + x0;
		double yt = 2*x*y + y0;

		x = xt;
		y = yt;

		iter++;
	}

	return iter;
}

/*
Compute an entire Mandelbrot image, writing each point to the given bitmap.
Scale the image to the range (xmin-xmax,ymin-ymax), limiting iterations to "max"
*/

void compute_image(imgRawImage* img, double xmin, double xmax, double ymin, double ymax, int max, int threads)
{
	//create an array of threads
	pthread_t thread_arr[threads];
	//array to hold the thread data
	thread_data thread_datas[threads];

	//calculate height and width
	int height = img->height;
	int width = height / threads;

	// For every pixel in the image...

	for (int i = 0; i < threads; i++) {
		//assign values to the thread data
		thread_datas[i].img = img;
		thread_datas[i].xmin = xmin;
		thread_datas[i].xmax = xmax;
		thread_datas[i].ymin = ymin;
		thread_datas[i].ymax = ymax;
		thread_datas[i].max = max;
		thread_datas[i].start = i * width;
		if (i == threads - 1) {
    		thread_datas[i].end = height;
		} else {
    		thread_datas[i].end = (i + 1) * width;
		}

		//create the thread
		pthread_create(&thread_arr[i], NULL, compute_region, &thread_datas[i]);
	}

	//join all the threads at the end
	for(int i = 0; i < threads; i++) {
		pthread_join(thread_arr[i], NULL);
	}

}

void* compute_region(void* input) {
	//assign the data and img
	thread_data* data = (thread_data*)input;
	imgRawImage* img = data->img;

	//calculate the width of region to be computed
	int width = img->width;

	for (int i = data->start; i < data->end; i++) {
		for (int j = 0; j < width; j++) {
			// Determine the point in x,y space for that pixel.
			double x = data->xmin + i*(data->xmax-data->xmin)/width;
			double y = data->ymin + j*(data->ymax-data->ymin)/img->height;

			// Compute the iterations at that point.
			int iters = iterations_at_point(x,y,data->max);

			// Set the pixel in the bitmap.
			setPixelCOLOR(img,i,j,iteration_to_color(iters,data->max));
		}
	}
	return NULL;
}
/*
Convert a iteration number to a color.
Here, we just scale to gray with a maximum of imax.
Modify this function to make more interesting colors.
*/
int iteration_to_color( int iters, int max )
{
	int color = 0xFFFFFF*iters/(double)max;
	return color;
}


// Show help message
void show_help()
{
	printf("Use: mandel [options]\n");
	printf("Where options are:\n");
	printf("-m <max>    The maximum number of iterations per point. (default=1000)\n");
	printf("-x <coord>  X coordinate of image center point. (default=0)\n");
	printf("-y <coord>  Y coordinate of image center point. (default=0)\n");
	printf("-s <scale>  Scale of the image in Mandlebrot coordinates (X-axis). (default=4)\n");
	printf("-W <pixels> Width of the image in pixels. (default=1000)\n");
	printf("-H <pixels> Height of the image in pixels. (default=1000)\n");
	printf("-o <file>   Set output file. (default=mandel.bmp)\n");
	printf("-h          Show this help text.\n");
	printf("\nSome examples are:\n");
	printf("mandel -x -0.5 -y -0.5 -s 0.2\n");
	printf("mandel -x -.38 -y -.665 -s .05 -m 100\n");
	printf("mandel -x 0.286932 -y 0.014287 -s .0005 -m 1000\n\n");
}

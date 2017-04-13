#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

//declare global variables to be shared by threads
int ***pixels;
int ***rotatedPixels;
int height, width, maxValue, pixelColor, numThreads;
char *option;
float contrastPercentage;

//function to print pixels
void printPixels(int ***pixels) {
	fprintf(stdout, "P3\n");
	fprintf(stdout, "%i %i\n", width, height);
	fprintf(stdout, "%i\n", maxValue);
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			for (int k = 0; k < 3; ++k) {
				fprintf(stdout, "%i ", pixels[i][j][k]);
			}
		}
	}
}

//function to print the rotate pixels - height and width are swapped
void printRotatedPixels(int ***pixels) {
	fprintf(stdout, "P3\n");
	fprintf(stdout, "%i %i\n", height, width);
	fprintf(stdout, "%i\n", maxValue);
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < height; ++j) {
			for (int k = 0; k < 3; ++k) {
				fprintf(stdout, "%i ", pixels[i][j][k]);
			}
		}
	}
}

//transforms an image based on options inputted by user
void *transformImage(void *arg) {
	//typecast the argument as an int threadID
	int threadID = (int *)arg;
	//divide the image based on the number of threads
	int multiplier = height/numThreads;
	//set the start of each thread's responsibility to its ID * multiplier
	int start = threadID * multiplier;
	int end;
	
	//if it is the last thread then set the end of the thread's responsibility to the end of the image
	if (threadID == numThreads-1) {
		end = height;
	//else set it to the next threads start
	} else {
		end = (threadID + 1) * multiplier;
	}

	//if the option is to invert then use equation to invert all pixels
	if (strcmp(option, "-I") == 0) {
		for (int i = start; i < end; ++i) {
			for (int j = 0; j < width; ++j) {
				for (int k = 0; k < 3; ++k) {
					pixels[i][j][k] = maxValue - pixels[i][j][k];
				}
			}
		}
	}

	//if the option is to rotate it then use equation to rotate right
	if (strcmp(option, "-R") == 0) {
		for (int i = start; i < end; ++i) {
			for (int j = 0; j < width; ++j) {
				for (int k = 0; k < 3; ++k) {
					rotatedPixels[j][height-i-1][k] = pixels[i][j][k];
				}
			}
		}
	}

	//if the option is to rotate it then use the equation to rotate left
	if (strcmp(option, "-L") == 0) {
		for (int i = start; i < end; ++i) {
			for (int j = 0; j < width; ++j) {
				for (int k = 0; k < 3; ++k) {
					rotatedPixels[width-j-1][i][k] = pixels[i][j][k];
				}
			}
		}
	}

	//if the option is red then cancel out all other colors
	if (strcmp(option, "-red") == 0) {
		for (int i = start; i < end; ++i) {
			for (int j = 0; j < width; ++j) {
				pixels[i][j][1] = 0;
				pixels[i][j][2] = 0;
			}
		}
	}

	//if the option is green then cancel out all other colors
	if (strcmp(option, "-green") == 0) {
		for (int i = start; i < end; ++i) {
			for (int j = 0; j < width; ++j) {
				pixels[i][j][0] = 0;
				pixels[i][j][2] = 0;
			}
		}
	}

	//if the option is blue then cancel out all other colors
	if (strcmp(option, "-blue") == 0) {
		for (int i = start; i < end; ++i) {
			for (int j = 0; j < width; ++j) {
				pixels[i][j][0] = 0;
				pixels[i][j][1] = 0;
			}
		}
	}

	//if option is contrast then change pixels by contrast percentage
	//if it ends up being less than 0 or more than 255 then max it out to those numbers for aesthetics
	if (strcmp(option, "-C") == 0) {
		for (int i = start; i < end; ++i) {
			for (int j = 0; j < width; ++j) {
				for (int k = 0; k < 3; ++k) {
					if (pixels[i][j][k] <= (maxValue/2)) {
						pixels[i][j][k] -= (maxValue * contrastPercentage);
						if (pixels[i][j][k] < 0) {
							pixels[i][j][k] = 0;
						}
					} else {
						pixels[i][j][k] += (maxValue * contrastPercentage);
						if (pixels[i][j][k] > 255) {
							pixels[i][j][k] = 255;
						}
					}
				}
			}
		}
	}
	//return NULL in order to be used in pthread_create
	return NULL;
}

int main (int argc, char **argv) {
	//error check command line arguments
	if (argc < 3) {
		fprintf(stderr, "usage: ./a.out num_threads option [arg]\n");
		exit(1);
	} else if (argc < 4 && strcmp(argv[2], "-C") == 0) {
		fprintf(stderr, "usage: ./a.out num_threads option [arg]\n%s\n", "-C option must come with arg");
		exit(1);		
	} else if (argc == 3) {
		if (atoi(argv[1]) <= 0) {
			fprintf(stderr, "usage: ./a.out num_threads option [arg]\n%s\n", "num_threads must be greater than 0");
			exit(1);
		}
		if (strcmp(argv[2], "-I") != 0 && strcmp(argv[2], "-R") != 0 && strcmp(argv[2], "-L") != 0 &&
			strcmp(argv[2], "-red") != 0 && strcmp(argv[2], "-green") != 0 && strcmp(argv[2], "-blue") != 0 &&
			strcmp(argv[2], "-C") != 0) {
			fprintf(stderr, "usage: ./a.out num_threads option [arg]\n%s\n", "option: -I, -R, -L, -red, -green, -blue, or -C");
			exit(1);
		}
	} else if (argc == 4 && strcmp(argv[2], "-C") == 0) {
		if (atoi(argv[1]) <= 0) {
			fprintf(stderr, "usage: ./a.out num_threads option [arg]\n%s\n", "num_threads must be greater than 0");
			exit(1);
		}

		if ((atof(argv[3]) <= 0) || (atof(argv[3]) > 1)) {
			fprintf(stderr, "usage: ./a.out num_threads option [arg]\n%s\n", "arg must be decimal between 0 and 1");
			exit(1);
		}
	} else {
		fprintf(stderr, "usage: ./a.out num_threads option [arg]\n");
		exit(1);
	}

	//set variables based on arguments given
	numThreads = atoi(argv[1]);
	option = argv[2];
	if (argc == 4) {
		contrastPercentage = atof(argv[3]);
	}

	//scan the PPM header in
	if (fscanf(stdin, "P3 %i%i%i", &width, &height, &maxValue) == -1) {
		perror("Bad file read");
	}

	//allocate memory for pixels
	pixels = malloc(height * sizeof(int **));
	if (pixels == NULL) {
		perror("Malloc error");
		exit(1);
	}
	for (int i = 0; i < height; ++i) {
		pixels[i] = malloc(width * sizeof(int *));
		if (pixels[i] == NULL) {
			perror("Malloc error");
		}
		for (int j = 0; j < width; ++j) {
			pixels[i][j] = malloc(sizeof(int));
			if (pixels[i][j] == NULL) {
				perror("Malloc error");
			}
		}
	}

	//if the number of threads is greater than height or width then print error
	if(numThreads > height || numThreads > width) {
		fprintf(stderr, "Number of threads cannot be greater than height. Bummer.\n");
		exit(1);
	}

	//read in the rest of the file with the pixels and store in array
	//redPixel = when k is 0
	//greenPixel = when k is 1
	//bluePixel = when k is 2
	for (int i = 0; i < height; ++i) {
		for (int j = 0; j < width; ++j) {
			for (int k = 0; k < 3; ++k) {
				if (fscanf(stdin, "%i", &pixelColor) != -1) {
					pixels[i][j][k] = pixelColor;
				}
			}
		}
	}

	//if the option is to rotate then allocate memory to hold rotated pixels
	if (strcmp(option, "-R") == 0 || strcmp(option, "-L") == 0) {
		rotatedPixels = malloc(width * sizeof(int **));
		if (rotatedPixels == NULL) {
			perror("Malloc error");
			exit(1);
		}
		for (int i = 0; i < width; ++i) {
			rotatedPixels[i] = malloc(height * sizeof(int *));
			if (rotatedPixels[i] == NULL) {
				perror("Malloc error");
			}
			for (int j = 0; j < height; ++j) {
				rotatedPixels[i][j] = malloc(sizeof(int));
				if (rotatedPixels[i][j] == NULL) {
					perror("Malloc error");
				}
			}
		}
	}

	//allocate memory for the threads
	pthread_t *threads;
	threads = (pthread_t *)malloc(sizeof(pthread_t) * numThreads);
	if (threads == NULL) {
		perror("Malloc error");
	}

	//create each thread and have it transform a portion of the image
	for (int i = 0; i < numThreads; ++i) {
		if (pthread_create(&threads[i], NULL, transformImage, (void *) i) != 0) {
			perror("Pthread creation error");
			exit(1);
		}
	}

	//join the threads
	for (int i = 0; i < numThreads; ++i) {
		if (pthread_join(threads[i], NULL) != 0) {
			perror("Pthread join error");
			exit(1);
		}
	}

	//if the option was to rotate then print out the rotated pixels and free the array
	if (strcmp(option, "-R") == 0 || strcmp(option, "-L") == 0) {
		printRotatedPixels(rotatedPixels);
		for (int i = 0; i < width;  ++i) {
			for (int j = 0; j < height; ++j) {
				free(rotatedPixels[i][j]);
			}
			free(rotatedPixels[i]);
		}
		free(rotatedPixels);
	//else print out the normal pixels
	} else {
		printPixels(pixels);
	}

	//free the pixel array
	for (int i = 0; i < height;  ++i) {
		for (int j = 0; j < width; ++j) {
			free(pixels[i][j]);
		}
		free(pixels[i]);
	}
	free(pixels);
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int ***pixels;
int height, width, maxValue, pixelColor;

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

int main (int argc, char **argv) {
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

	// int num_threads = atoi(argv[1]);

	if (fscanf(stdin, "P3 %i%i%i", &width, &height, &maxValue) == -1) {
		perror("WHY");
	}

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

	if (strcmp(argv[2], "-I") == 0) {
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				for (int k = 0; k < 3; ++k) {
					pixels[i][j][k] = maxValue - pixels[i][j][k];
				}
			}
		}
		printPixels(pixels);
	}
	if (strcmp(argv[2], "-R") == 0) {
		int ***rotatedPixels;

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

		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				for (int k = 0; k < 3; ++k) {
					rotatedPixels[j][height-i-1][k] = pixels[i][j][k];
				}
			}
		}

		fprintf(stdout, "P3\n");
		fprintf(stdout, "%i %i\n", height, width);
		fprintf(stdout, "%i\n", maxValue);
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j) {
				for (int k = 0; k < 3; ++k) {
					fprintf(stdout, "%i ", rotatedPixels[i][j][k]);
				}
			}
		}
	}
	if (strcmp(argv[2], "-L") == 0) {
		int ***rotatedLeftPixels;

		rotatedLeftPixels = malloc(width * sizeof(int **));
		if (rotatedLeftPixels == NULL) {
			perror("Malloc error");
			exit(1);
		}
		for (int i = 0; i < width; ++i) {
			rotatedLeftPixels[i] = malloc(height * sizeof(int *));
			if (rotatedLeftPixels[i] == NULL) {
				perror("Malloc error");
			}
			for (int j = 0; j < height; ++j) {
				rotatedLeftPixels[i][j] = malloc(sizeof(int));
				if (rotatedLeftPixels[i][j] == NULL) {
					perror("Malloc error");
				}
			}
		}

		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				for (int k = 0; k < 3; ++k) {
					rotatedLeftPixels[width-j-1][i][k] = pixels[i][j][k];
				}
			}
		}

		fprintf(stdout, "P3\n");
		fprintf(stdout, "%i %i\n", height, width);
		fprintf(stdout, "%i\n", maxValue);
		for (int i = 0; i < width; ++i) {
			for (int j = 0; j < height; ++j) {
				for (int k = 0; k < 3; ++k) {
					fprintf(stdout, "%i ", rotatedLeftPixels[i][j][k]);
				}
			}
		}
	}
	if (strcmp(argv[2], "-red") == 0) {
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				pixels[i][j][1] = 0;
				pixels[i][j][2] = 0;
			}
		}
		printPixels(pixels);
	}
	if (strcmp(argv[2], "-green") == 0) {
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				pixels[i][j][0] = 0;
				pixels[i][j][2] = 0;
			}
		}
		printPixels(pixels);
	}
	if (strcmp(argv[2], "-blue") == 0) {
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				pixels[i][j][0] = 0;
				pixels[i][j][1] = 0;
			}
		}
		printPixels(pixels);
	}
	if (strcmp(argv[2], "-C") == 0) {
		float contrastPercentage = atof(argv[3]);
		for (int i = 0; i < height; ++i) {
			for (int j = 0; j < width; ++j) {
				for (int k = 0; k < 3; ++k) {
					if (pixels[i][j][k] <= (maxValue/2)) {
						pixels[i][j][k] -= (maxValue * contrastPercentage);
					} else {
						pixels[i][j][k] += (maxValue * contrastPercentage);
					}
				}
			}
		}
		printPixels(pixels);
	}

	//DONT FORGET TO FREE THOSE MOTHERFUCKERS PLEASE
}

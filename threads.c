#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int ***pixels;

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

	int height, width, maxValue;
	int pixelColor;

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

	//DONT FORGET TO FREE THOSE MOTHERFUCKERS PLEASE
}

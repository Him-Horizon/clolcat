/*
 * clolcat.c — A simple C implementation of lolcat
 * 
 * Author: Shubham Ramdeo (ramdeoshubham.com)
 * Version: 0.1
 * License: MIT License
 *
 * This program reads text from standard input and outputs it
 * to the terminal with rainbow colors using ANSI truecolor codes.
 * Features:
 *  - Random starting color for each run
 *  - Optional flags:
 *      -v / --version : show version and author
 *      -f <value>     : set color frequency (default: 0.01)
 *      -p <value>     : set color spread (default: 1.0)
 *      -h / --help    : show this help message
 *
 * Usage:
 *  cat file.txt | clolcat
 *  echo "Hello world" | clolcat -f 0.2 -p 4.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define DEFAULT_FREQ 0.01
#define DEFAULT_SPREAD 1.0
#define VERSION "0.1"
#define AUTHOR "Shubham Ramdeo (ramdeoshubham.com)"

void rainbow_color(int i, double freq, double spread, double phase, int *r, int *g, int *b) {
    *r = (int)(sin(freq * i + phase + 0) * 127 * spread + 128);
    *g = (int)(sin(freq * i + phase + 2 * M_PI / 3) * 127 * spread + 128);
    *b = (int)(sin(freq * i + phase + 4 * M_PI / 3) * 127 * spread + 128);

    if (*r > 255) *r = 255; if (*r < 0) *r = 0;
    if (*g > 255) *g = 255; if (*g < 0) *g = 0;
    if (*b > 255) *b = 255; if (*b < 0) *b = 0;
}

void process_file(FILE *fp, double freq, double spread, double phase) {
    int c, i = 0;
    while ((c = fgetc(fp)) != EOF) {
        if (c == '\n') {
            putchar(c);
            continue;
        }
        int r, g, b;
        rainbow_color(i++, freq, spread, phase, &r, &g, &b);
        printf("\033[38;2;%d;%d;%dm%c\033[0m", r, g, b, c);
    }
}

void print_help() {
    printf("clolcat version %s - by %s\n", VERSION, AUTHOR);
    printf("Usage: clolcat [options]\n");
    printf("Options:\n");
    printf("  -v, --version       Show version information\n");
    printf("  -h, --help          Show this help message\n");
    printf("  -f <value>          Set color frequency (default %.2f)\n", DEFAULT_FREQ);
    printf("  -p <value>          Set color spread (default %.2f)\n", DEFAULT_SPREAD);
}

int main(int argc, char **argv) {
    double freq = DEFAULT_FREQ;
    double spread = DEFAULT_SPREAD;

    srand(time(NULL));
    double phase = ((double)rand() / RAND_MAX) * 2 * M_PI;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--version") == 0) {
            printf("clolcat version %s — by %s\n", VERSION, AUTHOR);
            return 0;
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            print_help();
            return 0;
        } else if (strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) {
                freq = atof(argv[++i]);
                if (freq <= 0) {
                    fprintf(stderr, "Invalid frequency. Using default %.2f\n", DEFAULT_FREQ);
                    freq = DEFAULT_FREQ;
                }
            } else {
                fprintf(stderr, "Missing value for -f. Using default %.2f\n", DEFAULT_FREQ);
            }
        } else if (strcmp(argv[i], "-p") == 0) {
            if (i + 1 < argc) {
                spread = atof(argv[++i]);
                if (spread <= 0) {
                    fprintf(stderr, "Invalid spread. Using default %.2f\n", DEFAULT_SPREAD);
                    spread = DEFAULT_SPREAD;
                }
            } else {
                fprintf(stderr, "Missing value for -p. Using default %.2f\n", DEFAULT_SPREAD);
            }
        }
    }

    FILE *fp = stdin;
    process_file(fp, freq, spread, phase);

    return 0;
}


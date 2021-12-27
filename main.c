#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "utils.h"

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 10) {
        printf("Incorrect usage\n");
        exit(1);
    }

    char *filename;
    char outdir[255];

    int maxIter = 50;
    int dumpFreq = 1;

    int isShow = 0;

    for (int i = 1; i < argc; i += 2) {
        if (!strcmp(argv[i], "--input")) {
            filename = argv[i + 1];
        } else if (!strcmp(argv[i], "--output")) {
            strcpy(outdir, argv[i + 1]);
        } else if (!strcmp(argv[i], "--max_iter")) {
            maxIter = atoi(argv[i + 1]);
        } else if (!strcmp(argv[i], "--dump_freq")) {
            dumpFreq = atoi(argv[i + 1]);
        } else if (!strcmp(argv[i], "--show")) {
            isShow = 1;
        } else {
            printf("Incorrect usage\n");
            exit(1);
        }
    }

    FILE *fptr = fopen(filename, "rb");

    if (fptr == NULL) {
        printf("File was not found\n");
        exit(1);
    }

    struct stat st = {0};

    if (stat(outdir, &st) == -1) {
        mkdir(outdir, 0700);
    }

    BMPImage bmpImage = readBMP(fptr);
    run(bmpImage, maxIter, dumpFreq, isShow, outdir);

    return 0;
}
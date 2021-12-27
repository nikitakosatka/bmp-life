#ifndef BMPLIFE_UTILS_H
#define BMPLIFE_UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct BMPImage {
    int fileSize;
    int headerSize;
    int width;
    int height;
    uint8_t *header;
    uint8_t *image;
    uint8_t *data;
} BMPImage;

void show(uint8_t *board, int width, int height);

void evolve(uint8_t *board, int width, int height);

uint8_t countFromHeaderPos(uint8_t *header, int pos);

BMPImage readBMP(FILE *filePointer);

void saveBMP(BMPImage bmp, FILE *output);

void run(BMPImage bmp, int maxIter, int dumpFreq, int isShow, char outdir[255]);

#endif
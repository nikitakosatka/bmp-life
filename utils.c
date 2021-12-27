#include "utils.h"

const int HEADER_SIZE = 54;
const int PALETTE_SIZE = 8;

void show(uint8_t *board, int width, int height) {
    uint8_t *universe = board;

    printf("\033[H");

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (universe[y * width + x]) {
                printf("\033[07m  \033[m");
            } else {
                printf("  ");
            }
        }
        printf("\033[E");
    }
    fflush(stdout);
}

void evolve(uint8_t *board, int width, int height) {
    uint8_t *universe = board;
    unsigned *new = malloc(width * height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int neighbors = 0;

            for (int y1 = y - 1; y1 <= y + 1; y1++)
                for (int x1 = x - 1; x1 <= x + 1; x1++)
                    if (universe[(y1 + height) % height * width + (x1 + width) % width])
                        neighbors++;

            if (universe[y * width + x]) {
                neighbors--;
            }
            new[y * width + x] = (neighbors == 3 || (neighbors == 2 && universe[y * width + x]));
        }
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            universe[y * width + x] = new[y * width + x];
        }
    }
}

uint8_t countFromHeaderPos(uint8_t *header, int pos) {
    return header[pos] + (((uint8_t) header[pos + 1]) << 8) + (((uint8_t) header[pos + 2]) << 16) +
           (((uint8_t) header[pos + 3]) << 24);
}

BMPImage readBMP(FILE *filePointer) {
    int lineSize;

    BMPImage res;
    res.header = malloc((HEADER_SIZE + PALETTE_SIZE) * sizeof(uint8_t));

    res.headerSize = HEADER_SIZE + PALETTE_SIZE;

    fread(res.header, 1, HEADER_SIZE + PALETTE_SIZE, filePointer);

    int offset = countFromHeaderPos(res.header, 10);
    res.width = countFromHeaderPos(res.header, 18);
    res.height = countFromHeaderPos(res.header, 22);

    if (offset > HEADER_SIZE + PALETTE_SIZE) {
        free(res.header);
        res.header = malloc(offset * sizeof(uint8_t));
        fseek(filePointer, 0, SEEK_SET);
        fread(res.header, 1, offset, filePointer);
        res.headerSize = offset;
    }

    lineSize = (res.width / 32) * 4;

    if (res.width % 32 != 0) {
        lineSize += 4;
    }

    res.fileSize = lineSize * res.height;

    res.image = malloc(res.width * res.height);
    res.data = malloc(res.fileSize);

    fseek(filePointer, offset, SEEK_SET);
    fread(res.data, 1, res.fileSize, filePointer);

    int currentByte;

    for (int y = 0; y < res.height; y++) {
        for (int x = 0; x < res.width; x++) {
            currentByte = x / 8;

            uint8_t dataByte = res.data[y * lineSize + currentByte];
            uint8_t mask = 0x80 >> x % 8;

            if (dataByte & mask) {
                res.image[(res.height - (1 + y)) * res.width + x] = 1;
            } else {
                res.image[(res.height - (1 + y)) * res.width + x] = 0;
            }
        }
    }

    return res;
}

void saveBMP(BMPImage bmp, FILE *output) {
    uint8_t **matrix;
    int width = bmp.width;
    int height = bmp.height;

    matrix = (uint8_t **) malloc((height + 2) * sizeof(uint8_t *));

    for (int i = 0; i < height + 2; i++) {
        matrix[i] = (uint8_t *) malloc((width + 2) * sizeof(uint8_t));
    }

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (bmp.image[i * width + j]) {
                matrix[i + 1][j + 1] = '0';
            } else {
                matrix[i + 1][j + 1] = '1';
            }
        }
    }

    int currentPosition = 0;
    uint8_t currentByte = 0;

    for (int i = height; i >= 1; i--) {
        for (int j = 1; j <= width; j++) {

            uint8_t mask = 0x80 >> (j - 1) % 8;
            if (matrix[i][j] == '0') {
                currentByte = currentByte | mask;
            }

            if ((j % 8) == 0 || j == width) {
                bmp.data[currentPosition] = currentByte;
                currentPosition++;
                currentByte = 0;
                mask = 0x80;
            }
        }
        while ((currentPosition) % 4 != 0) {
            currentPosition++;
        }
    }

    fwrite(bmp.header, sizeof(uint8_t), bmp.headerSize, output);
    fwrite(bmp.data, sizeof(uint8_t), bmp.fileSize, output);
}

void run(BMPImage bmp, int maxIter, int dumpFreq, int isShow, char outdir[255]) {

    for (int i = 1; i <= maxIter; i++) {
        if (i % dumpFreq == 0) {
            if (isShow) {
                usleep(100000);
                show(bmp.image, bmp.width, bmp.height);
            }

            char *filepath = calloc(sizeof(char), 255);
            char number[10];
            sprintf(number, "%d", i);

            strcat(filepath, outdir);
            strcat(filepath, "/");
            strcat(filepath, number);
            strcat(filepath, ".bmp");
            FILE *out = fopen(filepath, "wb");
            saveBMP(bmp, out);
        }
        uint8_t *last = malloc(bmp.height * bmp.width * sizeof(uint8_t));
        for (int j = 0; j < bmp.height; j++) {
            for (int k = 0; k < bmp.width; k++) {
                last[j * bmp.width + k] = bmp.image[j * bmp.width + k];
            }
        }
        evolve(bmp.image, bmp.width, bmp.height);

        int flag = 0;

        for (int j = 0; j < bmp.height; j++) {
            for (int k = 0; k < bmp.width; k++) {
                if (last[j * bmp.width + k] != bmp.image[j * bmp.width + k]) {
                    flag = 1;
                }
            }
            if (flag) {
                break;
            }
        }
        if (!flag) {
            return;
        }
    }
}
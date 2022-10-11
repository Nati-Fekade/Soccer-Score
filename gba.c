#include "gba.h"

// The start of the video memory
unsigned volatile short *videoBuffer = (unsigned short *)0x6000000;


// ---- Miscellaneous Functions ----

int collision(int colA, int rowA, int widthA, int heightA, int colB, int rowB, int widthB, int heightB) {
    return rowA <= rowB + heightB - 1 && rowA + heightA - 1 >= rowB && colA <= colB + widthB - 1 && colA + widthA - 1 >= colB;
}

void waitForVBlank() {
    while (SCANLINECOUNTER >= 160);
    while (SCANLINECOUNTER < 160);
}

// ---- Mode 3 ----

void setPixel(int col, int row, unsigned short color) {
    videoBuffer[OFFSET(col, row, SCREENWIDTH)] = color;
}

void drawRect(int col, int row, int width, int height, volatile unsigned short color) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            setPixel(col + j, row + i, color);
        }
    }
}

void drawHorizontal(int col, int row, int size, volatile unsigned short color) {
    for (int i = 0; i < size; i++) {
        setPixel(col + i, row, color);
    }
}

void drawVertical(int col, int row, int size, volatile unsigned short color) {
    for (int i = 0; i < size; i++) {
        setPixel(col, row + i, color);
    }
}

void drawVerticalBroken(int col, int row, int size, volatile unsigned short color) {
    for (int i = 0; i < size; i+=5) {
        setPixel(col, row + i, color);
    }
}

void fillScreen(volatile unsigned short color) {
    // for (int i = 0; i < 240 * 160; i++) {
    //     videoBuffer[i] = color;
    // }
    volatile unsigned int temp = color | color << 16;
    DMANow(3, &temp, videoBuffer, DMA_SOURCE_FIXED | 38400/2 | DMA_32);
}

// ---- DMA ----

DMA *dma = (DMA *)0x40000B0;

void DMANow(int channel, volatile const void *src, volatile void *dst, unsigned int cnt) {
    dma[channel].cnt = 0;
    dma[channel].src = src;
    dma[channel].dst = dst;

    dma[channel].cnt = cnt | DMA_ON;
}
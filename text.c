#include "gba.h" // HW03 Scaffold
#include "text.h"
#include "font.h"

// Draws the specified character at the specified location in Mode 3
void drawChar(int col, int row, char ch, unsigned short color) {
    for (int c = 0; c < 6; c++) {
        for (int r = 0; r < 8; r++) {
            if (fontdata_6x8[(48 * ch + c + 6 * r)]) setPixel(col + c, row + r, color);
        }
    }
}

// Draws the specified string at the specified location in Mode 3
void drawString(int col, int row, char *str, unsigned short color) {
    int i = 0;
    while (*(str + i) != '\0') {
        drawChar(col + (i * 6), row, *(str + i), color);
        i++;
    }
}
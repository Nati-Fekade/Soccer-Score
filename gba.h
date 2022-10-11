#ifndef HW3_FALL_2022
#define HW3_FALL_2022

// Common Typedefs 
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

// Miscellaneous Macros
#define OFFSET(col, row, rowLength) ((row) * (rowLength) + (col))


// ---- Miscellaneous Functions ----
int collision(int colA, int rowA, int widthA, int heightA, int colB, int rowB, int widthB, int heightB);
void waitForVBlank();


// ---- Display ----

// Display Control Register Macros
#define REG_DISPCTL (*(volatile unsigned short *)0x4000000)
#define MODE0 0
#define MODE3 3
#define MODE4 4
#define DISP_BACKBUFFER (1<<4)
#define BG0_ENABLE      (1<<8)
#define BG1_ENABLE      (1<<9)
#define BG2_ENABLE      (1<<10)
#define BG3_ENABLE      (1<<11)
#define SPRITE_ENABLE   (1<<12)
#define SPRITE_MODE_2D  (0<<6)
#define SPRITE_MODE_1D  (1<<6)

// Display Status Registers
#define SCANLINECOUNTER (*(volatile unsigned short *)0x4000006)

// Display Constants
#define SCREENHEIGHT 160
#define SCREENWIDTH  240

// Video Buffer
extern volatile unsigned short *videoBuffer;


// ---- Color ----

// Colors
#define COLOR(r,g,b) ((r) | (g)<<5 | (b)<<10)
#define BLACK 0
#define GRAY COLOR(15,15,15)
#define WHITE COLOR(31,31,31)
#define RED COLOR(31,0,0)
#define GREEN COLOR(0,31,0)
#define BLUE COLOR(0,0,31)
#define CYAN COLOR(0,31,31)
#define MAGENTA COLOR(31,0,31)
#define YELLOW COLOR(31,31,0)


// ---- Mode 3 ----

// Mode 3 Drawing Functions
void setPixel(int col, int row, unsigned short color);
void drawRect(int col, int row, int width, int height, volatile unsigned short color);
void drawHorizontal(int col, int row, int size, volatile unsigned short color);
void drawVertical(int col, int row, int size, volatile unsigned short color);
void drawVerticalBroken(int col, int row, int size, volatile unsigned short color);
void fillScreen(volatile unsigned short color);


// ---- Input ----

// Button Register
#define BUTTONS (*(volatile unsigned short *)0x04000130)

// Button Masks
#define BUTTON_A		(1<<0)
#define BUTTON_B		(1<<1)
#define BUTTON_SELECT	(1<<2)
#define BUTTON_START	(1<<3)
#define BUTTON_RIGHT	(1<<4)
#define BUTTON_LEFT		(1<<5)
#define BUTTON_UP		(1<<6)
#define BUTTON_DOWN		(1<<7)
#define BUTTON_R		(1<<8)
#define BUTTON_L		(1<<9)

// Variables for Button Macros
extern unsigned short oldButtons;
extern unsigned short buttons;

// Button Macros
#define BUTTON_HELD(key)    (~(BUTTONS) & (key))
#define BUTTON_PRESSED(key) (!(~(oldButtons) & (key)) && (~buttons & (key)))

// ---- Direct Memory Access (DMA) ----

// DMA Struct
typedef volatile struct {
    volatile const void *src;
    volatile void *dst;
    volatile unsigned int cnt;
} DMA;

// First DMA Register Address
extern DMA *dma;

// Destination Adjustment
#define DMA_DESTINATION_INCREMENT (0 << 21)
#define DMA_DESTINATION_DECREMENT (1 << 21)
#define DMA_DESTINATION_FIXED (2 << 21)
#define DMA_DESTINATION_RESET (3 << 21)

// Source Adjustment
#define DMA_SOURCE_INCREMENT (0 << 23)
#define DMA_SOURCE_DECREMENT (1 << 23)
#define DMA_SOURCE_FIXED (2 << 23)

// VBlank and HBlank Repeats
#define DMA_REPEAT (1 << 25)

// Chunk Size
#define DMA_16 (0 << 26)
#define DMA_32 (1 << 26)

// Timing Mode
#define DMA_AT_NOW (0 << 28)
#define DMA_AT_VBLANK (1 << 28)
#define DMA_AT_HBLANK (2 << 28)
#define DMA_AT_REFRESH (3 << 28)

// Trigger Interrupts
#define DMA_IRQ (1 << 30)

// Begin DMA
#define DMA_ON (1 << 31)

// DMA Functions
void DMANow(int channel, volatile const void *src, volatile void *dst, unsigned int cnt);


#endif
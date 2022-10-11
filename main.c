#include <stdlib.h>
#include "gba.h"
#include "game.h"
#include "text.h"
#include "print.h"

enum {START, GAME, PAUSE, WIN, LOSE};
int state;

unsigned short buttons;
unsigned short oldButtons;

// Function Prototype
void initialize();
void goToStart();
void start();
void goToGame();
void game();
void goToPause();
void pause();
void goToWin();
void win();
void goToLose();
void lose();
void srand();

int seed;

char buffer0[41];
char buffer1[41];

void initialize() {
    REG_DISPCTL = MODE3 | BG2_ENABLE;

    goToStart();
}

void goToStart() {
    fillScreen(YELLOW);
    int col = (240 - (20 * 6)) / 2;
    drawString(col, 50, "press START to play!", BLACK);
    int col2 = (240 - (11 * 6)) / 2;
    drawString(col2, 80, "INSTRUCTION", BLACK);
    int col3 = (240 - (20 * 6)) / 2;
    drawString(col3, 100, "!!!score 5 to win!!!", BLACK);
    int col4 = (240 - (33 * 6)) / 2;
    drawString(col4, 120, "DO NOT LET THE DEFENDER TOUCH YOU", RED);
    int col5 = (240 - (37 * 6)) / 2;
    drawString(col5, 140, "DO NOT LET THE GOALKEEPER BLOCK YOU!!", RED);

    state = START;
}

void start() {
    seed++;
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToGame();
        srand(seed);

        initializeGame();
    }
}

void goToGame() {
    fillScreen(GREEN);

    drawString(10, 5, "Opponent Score: ", RED);
    drawString(130, 5, "Player Score: ", BLUE);

    state = GAME;
}

void game() {
    updateGame();

    sprintf(buffer0, "%d", opponentScore);
    sprintf(buffer1, "%d", playerScore);

    waitForVBlank();

    drawGame();

    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToPause();
    }
    if (playerScore == 5) {
        goToWin();
    }
    if (opponentScore == 5) {
        goToLose();
    }
}

void goToPause() {
    fillScreen(MAGENTA);
    int col = (240 - (6 * 6)) / 2;
    drawString(col, 50, "PAUSED", BLACK);
    int col2 = (240 - (23 * 6)) / 2;
    drawString(col2, 80, "press SELECT to return", BLACK);
    int col3 = (240 - (22 * 6)) / 2;
    drawString(col3, 100, "press START to restart", BLACK);

    state = PAUSE;
}

void pause() {
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        goToGame();
    }
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}

void goToWin() {
    fillScreen(CYAN);
    int col = (240 - (12 * 6)) / 2;
    drawString(col, 50, "~~~WOHHOO~~~", BLACK);
    int col2 = (240 - (25 * 6)) / 2;
    drawString(col2, 80, "press START to play again", BLACK);

    state = WIN;
}

void win() {
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}

void goToLose() {
    fillScreen(RED);
    int col = (240 - (12 * 6)) / 2;
    drawString(col, 50, "Tough Luck :(", BLACK);
    int col2 = (240 - (25 * 6)) / 2;
    drawString(col2, 80, "press START to play again", BLACK);

    state = LOSE;
}

void lose() {
    if (BUTTON_PRESSED(BUTTON_START)) {
        goToStart();
    }
}

int main() {
    mgba_open();

    initialize();

    while(321) {
        oldButtons = buttons;
        buttons = BUTTONS;

        switch (state) {
            case START:
                start();
                break;
            case GAME:
                game();
                break;
            case PAUSE:
                pause();
                break;
            case WIN:
                win();
                break;
            case LOSE:
                lose();
                break;
            default:
                break;
        }
    }
}
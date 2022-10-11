#include <stdlib.h>
#include "game.h"
#include "gba.h"
#include "text.h"
#include "analogLib.h"
#include "print.h"

PLAYER player;
DEFENDER defender[DEFENDERCOUNT];
GOALKEEPER goalKeeper[GOALKEEPERCOUNT];
BALL ball[BALLCOUNT];
HEALTH health;

int playerScore;
int opponentScore;

u16 drumR;
u16 drumS;

void initializeGame() {
    initializePlayer();
    initializeGoalKeeper();
    initializeDefender();
    initializeBall();
    initializeHealth();
    playerScore = 0;
    opponentScore = 0;
    initializeSound();
}

void initializeSound() {
    // Enable sound (Master control)
    REG_SOUNDCNT_X = SND_ENABLED;

    // Master sound controls for DMG (GameBoy) Sound Generators
    REG_SOUNDCNT_L = DMG_VOL_LEFT(5) |
                    DMG_VOL_RIGHT(5) |
                    DMG_SND1_LEFT |
                    DMG_SND1_RIGHT |
                    DMG_SND2_LEFT |
                    DMG_SND2_RIGHT |
                    DMG_SND3_LEFT |
                    DMG_SND3_RIGHT |
                    DMG_SND4_LEFT |
                    DMG_SND4_RIGHT;

    REG_SOUNDCNT_H = DMG_MASTER_VOL(2);
}

void initializePlayer() {
    player.playerRow = 70;
    player.playerCol = 200;
    player.playerOldRow = player.playerRow;
    player.playerOldCol = player.playerCol;
    player.playerHeight = 10;
    player.playerWidth = 5;
    player.playerHspeed = 1;
    player.playerVspeed = 1;
    player.playerColor = BLUE;
    
    player.ballTimer = 20;

    player.active = 1;
}

void initializeDefender() {
    for (int i = 0; i < DEFENDERCOUNT; i++) {
        defender[i].defenderHeight = 15;
        defender[i].defenderWidth = 8;
        defender[i].defenderRow = (rand() % 105) + 21;
        defender[i].defenderCol = (rand() % 102) + 71;
        defender[i].defenderOldRow = defender[i].defenderRow;
        defender[i].defenderOldCol = defender[i].defenderCol;
        defender[i].defenderHspeed = 1;
        defender[i].defenderVspeed = 1;
        defender[i].defenderColor = RED;

        defender[i].active = 1;
    }
}

void initializeGoalKeeper() {
    for (int i = 0; i < GOALKEEPERCOUNT; i++) {
        goalKeeper[i].goalKeeperCol = 15;
        goalKeeper[i].goalKeeperRow = 70;
        goalKeeper[i].goalKeeperOldCol = goalKeeper[i].goalKeeperCol;
        goalKeeper[i].goalKeeperOldRow = goalKeeper[i].goalKeeperRow;
        goalKeeper[i].goalKeeperHeight = 20;
        goalKeeper[i].goalKeeperWidth = 5;
        goalKeeper[i].goalKeeperVspeed = 1;
        goalKeeper[i].goalKeeperColor = BLACK;
        
        goalKeeper[i].active = 1;
        goalKeeper[i].activateSecond = 0;
    }
}

void initializeBall() {
    for (int i = 0; i < BALLCOUNT; i++) {
        ball[i].ballRow = player.playerRow;
        ball[i].ballCol = player.playerCol;
        ball[i].ballOldRow = player.playerRow;
        ball[i].ballOldCol = player.playerCol;
        ball[i].ballHeight = 3;
        ball[i].ballWidth = 3;
        ball[i].ballHspeed = 2;
        ball[i].ballColor = WHITE;

        ball[i].active = 0;
    }
}

void initializeHealth() {
    health.healthRow = (rand() % 105) + 21;
    health.healthCol = (rand() % 102) + 71;
    health.healthOldRow = health.healthRow;
    health.healthOldCol = health.healthCol;
    health.healthHeight = 2;
    health.healthWidth = 2;
    health.healthColor = MAGENTA;

    health.active = 1; 
}

void updateGame() {
    updatePlayer();
    for (int i = 0; i < GOALKEEPERCOUNT; i++) {
        updateGoalKeeper(&(goalKeeper[i]));
    }
    for (int i = 0; i < DEFENDERCOUNT; i++) {
        updateDefender(&(defender[i]));
    }
    for (int i = 0; i < 1; i++) {
        updateBall(&(ball[i]));
    }
    updateHealth();
}

void updatePlayer() {
    // Boundary checks for player
    if (player.playerCol < 61) { // left
        player.playerOldCol = player.playerCol;
        player.playerCol = 61;
    }
    if (player.playerCol + player.playerWidth > 230) { // right
        player.playerOldCol = player.playerCol;
        player.playerCol = 230 - (player.playerWidth - 1);
    }
    if (player.playerRow < 20) { // top
        player.playerOldRow = player.playerRow;
        player.playerRow = 20;
    }
    if (player.playerRow + player.playerHeight > 140) { // bottom
        player.playerOldRow = player.playerRow;
        player.playerRow = 140 - (player.playerHeight - 1);
    }

    if (BUTTON_HELD(BUTTON_UP)) {
        player.playerRow -= player.playerVspeed;
    }
    if (BUTTON_HELD(BUTTON_DOWN)) {
        player.playerRow += player.playerVspeed;
    }
    if (BUTTON_HELD(BUTTON_LEFT)) {
        player.playerCol -= player.playerHspeed;
    }
    if (BUTTON_HELD(BUTTON_RIGHT)) {
        player.playerCol += player.playerHspeed;
    }

    if (BUTTON_PRESSED(BUTTON_A) && player.ballTimer >= 20) {
        mgba_printf("shoot");
        shootBall();
        player.ballTimer = 0;
    }
    player.ballTimer++;
}

void updateDefender(DEFENDER* defender) {
    if (defender->active) {
        if (defender->defenderRow <= 20 || defender->defenderRow + defender->defenderHeight - 1 >= 140) {
            defender->defenderVspeed *= -1;
        }
        if (defender->defenderCol <= 70 || defender->defenderCol + defender->defenderWidth - 1 >= 180) {
            defender->defenderHspeed *= -1;
        }
    }

    if (collision(defender->defenderCol, defender->defenderRow, defender->defenderWidth, defender->defenderHeight, player.playerCol, player.playerRow, player.playerWidth, player.playerHeight)) {
        mgba_printf("collision with defender");
        player.playerRow = 70;
        player.playerCol = 200;

        REG_SND1CNT = DMG_ENV_VOL(15) | DMG_DIRECTION_DECR | DMG_STEP_TIME(7) | DMG_DUTY_50;
        REG_SND1FREQ = NOTE_E2 | SND_RESET | DMG_FREQ_TIMED;
    }

    defender->defenderRow += defender->defenderVspeed;
    defender->defenderCol += defender->defenderHspeed;
}

void updateGoalKeeper(GOALKEEPER* goalKeeper) {
    if (goalKeeper->active) {
        if (goalKeeper->goalKeeperRow <= 40 || goalKeeper->goalKeeperRow + goalKeeper->goalKeeperHeight - 1 >= 120) {
            goalKeeper->goalKeeperVspeed *= -1;
        }
    }
    // MOVE THE GOALKEEPER
    goalKeeper->goalKeeperRow += goalKeeper->goalKeeperVspeed;

    for (int i = 0; i < BALLCOUNT; i++) {
        if (collision(goalKeeper->goalKeeperCol, goalKeeper->goalKeeperRow, goalKeeper->goalKeeperWidth, goalKeeper->goalKeeperHeight, ball[i].ballCol, ball[i].ballRow, ball[i].ballWidth, ball[i].ballHeight)) {
            mgba_printf("collision with goalkeeper");
            opponentScore += 1;
            mgba_printf("opponent score: %d", opponentScore);
            ball[i].active = 0;
            ball[i].ballCol += 10;

            player.playerRow = 70;
            player.playerCol = 200;
            player.playerHeight += 2;
            player.playerWidth += 2;

            goalKeeper->goalKeeperHeight += 3;

            REG_SND4CNT = DMG_ENV_VOL(15) | 1 << 12 ;
            REG_SND4FREQ = DMG_FREQ_TIMED | 1 << 15 | drumR | drumS << 4 | 1 << 3;
            drumR = (drumR + 1) % 8;
        }
        if (ball[i].ballCol <= 10 && (ball[i].ballRow >= 40 && ball[i].ballRow + ball[i].ballHeight - 1 <= 120)) {
            mgba_printf("goal");
            playerScore += 1;
            mgba_printf("player score: %d", playerScore);
            ball[i].active = 0;
            ball[i].ballCol += 100;

            REG_SND1CNT = DMG_ENV_VOL(15) | DMG_DIRECTION_DECR | DMG_STEP_TIME(7) | DMG_DUTY_50;
            REG_SND1FREQ = NOTE_AS3 | SND_RESET | DMG_FREQ_TIMED;
            REG_SND1SWEEP = DMG_SWEEP_NUM(7) | DMG_SWEEP_STEPTIME(7) | DMG_SWEEP_DOWN;
        }
    }
}

void updateBall(BALL* ball) {
    if (ball->active && ((ball->ballCol + ball->ballWidth) > 10)) {
        ball->ballCol -= ball->ballHspeed;
    } else {
        ball->active = 0;
    }
}

void shootBall() {
    for (int i = 0; i < BALLCOUNT; i++) {
        if (ball[i].active == 0) {
            ball[i].ballRow = player.playerRow + (player.playerHeight / 2);
            ball[i].ballCol = player.playerCol;
            ball[i].active = 1;
            ball[i].inactive = 0;

            break;
        }
    }
}

void updateHealth() {
    if ((collision(health.healthCol, health.healthRow, health.healthWidth, health.healthHeight, player.playerCol, player.playerRow, player.playerWidth, player.playerHeight)) && (BUTTON_HELD(BUTTON_B))) {
        mgba_printf("collision with health");
        health.healthCol = player.playerCol + (player.playerWidth / 2);
        health.healthRow = player.playerRow + (player.playerHeight / 2);
        if ((health.healthCol + health.healthWidth) > 180 && BUTTON_PRESSED(BUTTON_L)) {
            mgba_printf("safezone");
            if (opponentScore > 2) {
                opponentScore -= 1;
                mgba_printf("opponent score: %d", opponentScore);
                
                REG_SND1CNT = DMG_ENV_VOL(15) | DMG_DIRECTION_DECR | DMG_STEP_TIME(7) | DMG_DUTY_50;
                REG_SND1FREQ = NOTE_C6 | SND_RESET | DMG_FREQ_TIMED;
            }
            health.healthRow = (rand() % 105) + 21;
            health.healthCol = (rand() % 102) + 71;
        }
    }
}

void drawGame() {
    drawPitch();
    drawPlayer();
    for (int i = 0; i < GOALKEEPERCOUNT; i++) {
        drawGoalKeeper(&(goalKeeper[i]));
    }
    for (int i = 0; i < DEFENDERCOUNT; i++) {
        drawDefender(&(defender[i]));
    }
    for (int i = 0; i < BALLCOUNT; i++) {
        drawBall(&(ball[i]));
    }
    drawHealth();
    drawScore();
}

void drawPitch() {
    drawHorizontal(10, 20, 220, WHITE); // TOP
    drawVertical(10, 20, 120, WHITE); // LEFT
    drawHorizontal(10, 140, 220, WHITE); // BOTTOM
    drawVertical(230, 20, 121, WHITE); // RIGHT
    drawVertical(SCREENWIDTH/2, 20, 120, WHITE); // MIDDLE
    
    drawHorizontal(10, 40, 50, WHITE); // top-left
    drawVertical(60, 40, 81, WHITE); // left vertical
    drawHorizontal(10, 120, 50, WHITE); // bottom-left

    drawVerticalBroken(70, 20, 120, BLUE); // broken line -> area to shoot

    drawHorizontal(180, 40, 50, WHITE); // top-right
    drawVertical(180, 40, 80, WHITE); // right vertical
    drawHorizontal(180, 120, 50, WHITE); // bottom-right
}

void drawPlayer() {
    if (player.active) {
        drawRect(player.playerOldCol, player.playerOldRow, player.playerWidth, player.playerHeight, GREEN);
        drawRect(player.playerCol, player.playerRow, player.playerWidth, player.playerHeight, player.playerColor);     
    }
    player.playerOldCol = player.playerCol;
    player.playerOldRow = player.playerRow;
}

void drawDefender(DEFENDER* defender) {
    if (defender->active) {
        drawRect(defender->defenderOldCol, defender->defenderOldRow, defender->defenderWidth, defender->defenderHeight, GREEN);
        drawRect(defender->defenderCol, defender->defenderRow, defender->defenderWidth, defender->defenderHeight, defender->defenderColor);
    }
    defender->defenderOldCol = defender->defenderCol;
    defender->defenderOldRow = defender->defenderRow;
}

void drawGoalKeeper(GOALKEEPER* goalKeeper) {
    if (goalKeeper->active) {
        drawRect(goalKeeper->goalKeeperOldCol, goalKeeper->goalKeeperOldRow, goalKeeper->goalKeeperWidth, goalKeeper->goalKeeperHeight, GREEN);
        drawRect(goalKeeper->goalKeeperCol, goalKeeper->goalKeeperRow, goalKeeper->goalKeeperWidth, goalKeeper->goalKeeperHeight, goalKeeper->goalKeeperColor);
    }
    goalKeeper->goalKeeperOldCol = goalKeeper->goalKeeperCol;
    goalKeeper->goalKeeperOldRow = goalKeeper->goalKeeperRow;
}

void drawBall(BALL* ball) {
    if (ball->active) {
        drawRect(ball->ballOldCol, ball->ballOldRow, ball->ballWidth, ball->ballHeight, GREEN);
        drawRect(ball->ballCol, ball->ballRow, ball->ballWidth, ball->ballHeight, ball->ballColor);
    } else if (!ball->inactive) {
        drawRect(ball->ballOldCol, ball->ballOldRow, ball->ballWidth, ball->ballHeight, GREEN);
        ball->inactive = 1;
    }
    ball->ballOldCol = ball->ballCol;
    ball->ballOldRow = ball->ballRow;
}

void drawHealth() {
    if (health.active) {
        drawRect(health.healthOldCol, health.healthOldRow, health.healthWidth, health.healthHeight, GREEN);
        drawRect(health.healthCol, health.healthRow, health.healthWidth, health.healthHeight, health.healthColor);
    }
    health.healthOldCol = health.healthCol;
    health.healthOldRow = health.healthRow;
}

void drawScore() {
    drawRect(105, 5, 6, 8, GREEN);
    drawString(105, 5, buffer0, RED);

    drawRect(213, 5, 6, 8, GREEN);
    drawString(213, 5, buffer1, BLUE);
}

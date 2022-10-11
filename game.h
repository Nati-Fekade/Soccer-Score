// Player Struct
typedef struct {
    int playerRow;
    int playerCol;
    int playerOldRow;
    int playerOldCol;
    int playerHeight;
    int playerWidth;
    int playerHspeed;
    int playerVspeed;
    unsigned short playerColor;
    int ballTimer;
    int active;
} PLAYER;
// Defender Struct
typedef struct {
    int defenderRow;
    int defenderCol;
    int defenderOldRow;
    int defenderOldCol;
    int defenderHeight;
    int defenderWidth;
    int defenderHspeed;
    int defenderVspeed;
    unsigned short defenderColor;
    int active;
} DEFENDER;
// Goalkeeper Struct
typedef struct {
    int goalKeeperRow;
    int goalKeeperCol;
    int goalKeeperOldRow;
    int goalKeeperOldCol;
    int goalKeeperHeight;
    int goalKeeperWidth;
    int goalKeeperVspeed;
    unsigned short goalKeeperColor;
    int active;
    int activateSecond;
} GOALKEEPER;
// Ball Struct
typedef struct {
    int ballRow;
    int ballCol;
    int ballOldRow;
    int ballOldCol;
    int ballHeight;
    int ballWidth;
    int ballHspeed;
    unsigned short ballColor;
    int active;
    int inactive;
} BALL;
// Health Struct
typedef struct {
    int healthRow;
    int healthCol;
    int healthOldRow;
    int healthOldCol;
    int healthHeight;
    int healthWidth;
    unsigned short healthColor;
    int active;
} HEALTH;

#define DEFENDERCOUNT 3
#define GOALKEEPERCOUNT 1
#define BALLCOUNT 1


// Variables
extern PLAYER player;
extern BALL ball[BALLCOUNT];
extern DEFENDER defender[DEFENDERCOUNT];
extern GOALKEEPER goalKeeper[GOALKEEPERCOUNT];
extern HEALTH health;

extern int playerScore;
extern int opponentScore;

extern char buffer0[41];
extern char buffer1[41];

// Function Prototypes
void initializeGame();
void initializePlayer();
void initializeDefender();
void initializeGoalKeeper();
void initializeBall();
void initializeHealth();
void initializeSound();

void drawGame();
void drawPitch();
void drawPlayer();
void drawDefender(DEFENDER* defender);
void drawGoalKeeper(GOALKEEPER* goalKeeper);
void drawBall(BALL* ball);
void drawHealth();
void drawScore();

void updateGame();
void updatePlayer();
void updateDefender(DEFENDER* defender);
void updateGoalKeeper(GOALKEEPER* goalKeeper);
void updateBall(BALL* ball);
void shootBall();
void updateHealth();
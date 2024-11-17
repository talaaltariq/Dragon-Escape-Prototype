#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROWS 20
#define COLS 20

// Function to display the dungeon with hidden obstacles until the player is close
void printDungeon(char dungeon[ROWS][COLS], int playerX, int playerY, int dragonX, int dragonY) {
    printf("\n  +");
    for (int i = 0; i < COLS; i++) printf("---+");
    printf("\n");

    for (int i = 0; i < ROWS; i++) {
        printf("  | ");
        for (int j = 0; j < COLS; j++) {
            if (i == playerX && j == playerY)
                printf("P | ");   // Player's position
            else if ((i == dragonX || i == dragonX + 1) && (j == dragonY || j == dragonY + 1))
                printf("D | ");   // Dragon's 2x2 position
            else if (i == ROWS - 1 && j == COLS - 1)
                printf("E | ");   // Exit position
            else if (dungeon[i][j] == 'O' && (abs(i - playerX) <= 1 && abs(j - playerY) <= 1))
                printf("O | ");   // Reveal obstacles only if close to player
            else
                printf(". | ");   // Empty space
        }
        printf("\n  +");
        for (int k = 0; k < COLS; k++) printf("---+");
        printf("\n");
    }
    printf("\n");
}

// Function to add obstacles to the dungeon
void addObstacles(char dungeon[ROWS][COLS], int numObstacles) {
    int placed = 0;
    while (placed < numObstacles) {
        int obsX = rand() % ROWS;
        int obsY = rand() % COLS;
        if (dungeon[obsX][obsY] == '.' && !(obsX == ROWS - 1 && obsY == COLS - 1)) {
            dungeon[obsX][obsY] = 'O';  // 'O' represents an obstacle
            placed++;
        }
    }
}

// Move dragon towards the player
void moveDragonTowardsPlayer(int *dragonX, int *dragonY, int playerX, int playerY) {
    if (abs(playerX - *dragonX) > abs(playerY - *dragonY)) {
        if (playerX > *dragonX && *dragonX < ROWS - 2) (*dragonX)++;
        else if (playerX < *dragonX && *dragonX > 0) (*dragonX)--; 
    } else {
        if (playerY > *dragonY && *dragonY < COLS - 2) (*dragonY)++;
        else if (playerY < *dragonY && *dragonY > 0) (*dragonY)--; 
    }
}

// Intro screen before starting the game
void displayIntroScreen() {
    printf("************************************************************************\n");
    printf("*                                                                      *\n");
    printf("*          _____                                                       *\n");
    printf("*         |  __ \\                                                      *\n");
    printf("*         | |  | |   _ __    __ _   __ _    ___    _ __                *\n");
    printf("*         | |  | | | '__|  / _` | / _` |  / __ \\  | '__\\               *\n");
    printf("*         | |__| | | |    | (_| || (_| | | |__| | | | | |              *\n");
    printf("*         |_____/  |_|     \\__,_| \\__  |  \\_____/ |_| |_|              *\n");
    printf("*                                 __/  |                               *\n");
    printf("*                                |____/                                *\n");
    printf("*                                                                      *\n");
    printf("************************************************************************\n");
    printf("*                                                                      *\n");
    printf("*                        Press Enter to Start                          *\n");
    printf("*                                                                      *\n");
    printf("************************************************************************\n");

    // Wait for user to press Enter before starting the game
    getchar();
}

int main() {
    srand(time(0));

    // Display the intro screen
    displayIntroScreen();

    char dungeon[ROWS][COLS];
    int level = 1;

    // Game loop for each level
    while (level <= 3) {
        // Initialize dungeon
        for (int i = 0; i < ROWS; i++) {
            for (int j = 0; j < COLS; j++) {
                dungeon[i][j] = '.';
            }
        }
        dungeon[ROWS - 1][COLS - 1] = 'E';

        // Set obstacles for Levels 2 and 3
        if (level == 2 || level == 3) {
            addObstacles(dungeon, 25);  // Add 25 obstacles
        }

        // Player and dragon start positions
        int playerX = 0, playerY = 0;
        int dragonX = rand() % (ROWS - 1), dragonY = rand() % (COLS - 1);

        while ((dragonX == playerX && dragonY == playerY) || (dragonX >= ROWS - 2 && dragonY >= COLS - 2)) {
            dragonX = rand() % (ROWS - 1);
            dragonY = rand() % (COLS - 1);
        }

        printf("Welcome to Dungeon Escape - Level %d!\n", level);
        printf("Reach the exit 'E' at the bottom-right corner, avoiding the dragon 'D'.\n");
        printf("Controls: W (up), A (left), S (down), D (right)\n");

        // Dragon speed modifier for each level
        int dragonSpeedModifier = (level == 1) ? 2 : (level == 3) ? 2 : 3;

        while (1) {
            printDungeon(dungeon, playerX, playerY, dragonX, dragonY);

            // Check if player encounters dragon
            if ((playerX == dragonX || playerX == dragonX + 1) && (playerY == dragonY || playerY == dragonY + 1)) {
                printf("Oh no! The dragon caught you. Game over!\n");
                return 0;
            }

            // Check if player has reached the exit
            if (playerX == ROWS - 1 && playerY == COLS - 1) {
                printf("Congratulations! You've completed Level %d!\n\n", level);
                level++;
                if (level > 3) {
                    printf("You've escaped all levels! Game complete!\n");
                    return 0;
                }
                break;
            }

            // Ask for the player's move
            printf("Enter move (W=up, A=left, S=down, D=right): ");
            char move;
            scanf(" %c", &move);

            // Calculate new player position
            int newX = playerX, newY = playerY;
            if (move == 'W' && playerX > 0) newX--;
            else if (move == 'S' && playerX < ROWS - 1) newX++;
            else if (move == 'A' && playerY > 0) newY--;
            else if (move == 'D' && playerY < COLS - 1) newY++;
            else {
                printf("Invalid move! Try again.\n");
                continue;
            }

            // Check if player hits an obstacle
            if (dungeon[newX][newY] == 'O') {
                printf("There's an obstacle! Choose another direction.\n");
                continue;
            } else {
                playerX = newX;
                playerY = newY;
            }

            // Dragon movement with modified speed in Levels 1 and 3
            if (rand() % dragonSpeedModifier == 0) {
                moveDragonTowardsPlayer(&dragonX, &dragonY, playerX, playerY);
            }
        }
    }
    return 0;
}

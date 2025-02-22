#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SQUARES 10
#define SQUARE_SIZE 60
#define CIRCLE_RADIUS 22
#define MOVEMENT_SPEED 80
#define FALL_SPEED 25
#define GROUND_Y 430  // Y position of the cyan line

struct Square {
    int x, y;
    int active;
};

int main() {
 int gd = DETECT, gm;
    initgraph(&gd, &gm, (char*)"");

    int maxX = getmaxx();
    int maxY = getmaxy();
    int circleX = maxX / 2, circleY = maxY - 50;
    int gameOver = 0;
    struct Square squares[MAX_SQUARES] = {0};
    int score = 0;
    char key;

    srand(time(0));

    while (1) {
        if (!gameOver) {
            // Handle keyboard input
            if (kbhit()) {
                key = getch();
                if (key == 0) {  // Extended key code
                    key = getch();
                    switch (key) {
                        case 75:  // Left arrow
                            circleX = (circleX - MOVEMENT_SPEED < CIRCLE_RADIUS) ? 
                                     CIRCLE_RADIUS : circleX - MOVEMENT_SPEED;
                            break;
                        case 77:  // Right arrow
                            circleX = (circleX + MOVEMENT_SPEED > maxX - CIRCLE_RADIUS) ? 
                                     maxX - CIRCLE_RADIUS : circleX + MOVEMENT_SPEED;
                            break;
                    }
                }
            }

            // Spawn new squares
            if (rand() % 20 == 0) {
                for (int i = 0; i < MAX_SQUARES; i++) {
                    if (!squares[i].active) {
                        squares[i].x = rand() % (maxX - SQUARE_SIZE);
                        squares[i].y = 0;
                        squares[i].active = 1;
                        break;
                    }
                }
            }

            // Update square positions and check collisions
            for (int i = 0; i < MAX_SQUARES; i++) {
                if (squares[i].active) {
                    squares[i].y += FALL_SPEED;

                    // Collision detection
                    if (circleX + CIRCLE_RADIUS > squares[i].x &&
                        circleX - CIRCLE_RADIUS < squares[i].x + SQUARE_SIZE &&
                        circleY + CIRCLE_RADIUS > squares[i].y &&
                        circleY - CIRCLE_RADIUS < squares[i].y + SQUARE_SIZE) {
                        gameOver = 1;
                    }

                    // Deactivate off-screen squares
                    if (squares[i].y >=GROUND_Y) {
                        squares[i].active = 0;
                        score++;
                    }
                }
            }

            // Draw everything
            cleardevice();

            // Draw cyan line through the center of the circle
            setcolor(CYAN);
            line(0, GROUND_Y, maxX, GROUND_Y);

            // Draw player circle
            setcolor(GREEN);
            setfillstyle(SOLID_FILL, GREEN);
            circle(circleX, circleY, CIRCLE_RADIUS);
            floodfill(circleX, circleY, GREEN);

            // Draw falling squares
            setcolor(RED);
            setfillstyle(SOLID_FILL, RED);
            for (int i = 0; i < MAX_SQUARES; i++) {
                if (squares[i].active) {
                    rectangle(squares[i].x, squares[i].y, 
                             squares[i].x + SQUARE_SIZE, squares[i].y + SQUARE_SIZE);
                    floodfill(squares[i].x + 1, squares[i].y + 1, RED);
                }
            }

            // Display score
            char scoreStr[20];
            setcolor(YELLOW);
            sprintf(scoreStr, "Score: %d", score);
            outtextxy(10, 10, scoreStr);
        }
        else {
            // Game Over screen
            cleardevice();
            outtextxy(maxX / 2 - 50, maxY / 2 - 10, (char*)"GAME OVER!");
            outtextxy(maxX / 2 - 80, maxY / 2 + 10, (char*)"Press R to restart");

            key = getch();
            if (toupper(key) == 'R') {
                // Reset game state
                gameOver = 0;
                circleX = maxX / 2;
                circleY = maxY - 50;
                score = 0;

                for (int i = 0; i < MAX_SQUARES; i++) {
                    squares[i].active = 0;
                }
            }
        }

        delay(50);
    }

    closegraph();
    return 0;
}

#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/welcome.h"
#include "images/enemy.h"
#include "images/win.h"

enum gba_state {
  START,
  PLAY,
  WIN,
  LOSE,
};

int main(void) {
  REG_DISPCNT = MODE3 | BG2_ENABLE;

  u32 currentButtons = BUTTONS;

  enum gba_state state = START;
  struct state gameState;
  for (int i = 0; i < 3; i++) {
    struct obstacle newObstacle;
    newObstacle.row = 25;
    newObstacle.col = (50 * (i+1)) + 25;
    newObstacle.rp = newObstacle.row;
    newObstacle.cp = newObstacle.col;
    newObstacle.movingDown = 1;
    newObstacle.movingUp = 0;
    gameState.obstacles[i] = newObstacle;
  }
  for (int x = 0; x < 3; x++) {
    struct obstacle newObstacle;
    newObstacle.row = 125;
    newObstacle.col = 50 * (x+1);
    newObstacle.rp = newObstacle.row;
    newObstacle.cp = newObstacle.col;
    newObstacle.movingDown = 0;
    newObstacle.movingUp = 1;
    gameState.obstacles[x+3] = newObstacle;
  }
  struct player newPlayer;
  newPlayer.row = 75;
  newPlayer.col = 10;
  newPlayer.rp = newPlayer.row;
  newPlayer.cp = newPlayer.col;
  newPlayer.color = CYAN;
  int timer = 0;

  while (1) {
    currentButtons = BUTTONS; 

    if (KEY_DOWN(BUTTON_SELECT, currentButtons)) {
      newPlayer.row = 75;
      newPlayer.col = 10;
      vBlankCounter = 0;
      state = START;
    }

    waitForVBlank();

    switch (state) {
      case START:
        drawFullScreenImageDMA(welcomescreen);
        if (KEY_DOWN(BUTTON_START, currentButtons)) {
          vBlankCounter = 0;
          state = PLAY;
        }
        break;
      case PLAY:
        timer = vBlankCounter / 60;
        char time[100];
        snprintf(time, 100, "%d", timer);
        fillScreenDMA(MAGENTA);
        drawString(10, 10, time, BLACK);
        drawRectDMA(newPlayer.row, newPlayer.col, 10, 10, newPlayer.color);
        for (int i = 0; i < 6; i++) {
          drawRectDMA(gameState.obstacles[i].row, gameState.obstacles[i].col, 10, 10, RED);
        }
        drawRectDMA(75, 225, 10, 10, YELLOW);
        if (KEY_DOWN(BUTTON_RIGHT, currentButtons)) {
          drawRectDMA(newPlayer.rp, newPlayer.cp, 10, 10, MAGENTA);
          newPlayer.col++;
          drawRectDMA(newPlayer.row, newPlayer.col, 10, 10, newPlayer.color);
          newPlayer.cp = newPlayer.col;
        }
        if (KEY_DOWN(BUTTON_LEFT, currentButtons)) {
          drawRectDMA(newPlayer.rp, newPlayer.cp, 10, 10, MAGENTA);
          newPlayer.col -= 1;
          drawRectDMA(newPlayer.row, newPlayer.col, 10, 10, newPlayer.color);
          newPlayer.cp = newPlayer.col;
        }
        if (KEY_DOWN(BUTTON_DOWN, currentButtons)) {
          drawRectDMA(newPlayer.rp, newPlayer.cp, 10, 10, MAGENTA);
          newPlayer.row++;
          drawRectDMA(newPlayer.row, newPlayer.col, 10, 10, newPlayer.color);
          newPlayer.rp = newPlayer.row;
        }
        if (KEY_DOWN(BUTTON_UP, currentButtons)) {
          drawRectDMA(newPlayer.rp, newPlayer.cp, 10, 10, MAGENTA);
          newPlayer.row -= 1;
          drawRectDMA(newPlayer.row, newPlayer.col, 10, 10, newPlayer.color);
          newPlayer.rp = newPlayer.row;
        }
        for (int x = 0; x < 6; x++) {
          if (gameState.obstacles[x].movingDown == 1) {
            drawRectDMA(gameState.obstacles[x].rp, gameState.obstacles[x].cp, 10, 10, MAGENTA);
            gameState.obstacles[x].row++;
            drawImageDMA(gameState.obstacles[x].row, gameState.obstacles[x].col, 10, 10, enemy);
            gameState.obstacles[x].rp = gameState.obstacles[x].row;
            if (gameState.obstacles[x].row == 125) {
              gameState.obstacles[x].movingUp = 1;
              gameState.obstacles[x].movingDown = 0;
            }
          }
          if (gameState.obstacles[x].movingUp == 1) {
            drawRectDMA(gameState.obstacles[x].rp, gameState.obstacles[x].cp, 10, 10, MAGENTA);
            gameState.obstacles[x].row -= 1;
            drawImageDMA(gameState.obstacles[x].row, gameState.obstacles[x].col, 10, 10, enemy);
            gameState.obstacles[x].rp = gameState.obstacles[x].row;
            if (gameState.obstacles[x].row == 25) {
              gameState.obstacles[x].movingUp = 0;
              gameState.obstacles[x].movingDown = 1;
            }
          }
        }
        if (newPlayer.row >= 65 && newPlayer.col >= 215
          && newPlayer.row <= 85 && newPlayer.col <= 235) {
          state = WIN;
        }
        for (int m = 0; m < 6; m++) {
          if (newPlayer.row >= gameState.obstacles[m].row - 10
           && newPlayer.col >= gameState.obstacles[m].col - 10
           && newPlayer.row <= gameState.obstacles[m].row + 10
           && newPlayer.col <= gameState.obstacles[m].col + 10) {
            state = LOSE;
          }
        }
        timer++;
        break;
      case WIN:
        drawFullScreenImageDMA(win);
        newPlayer.row = 75;
        newPlayer.col = 10;
        vBlankCounter = 0;
        break;
      case LOSE:
        fillScreenDMA(GRAY);
        drawString(50, 20, "Press Select to Try Again!", RED);
        newPlayer.row = 75;
        newPlayer.col = 10;
        vBlankCounter = 0;
        break;
    }

  }

  return 0;
}

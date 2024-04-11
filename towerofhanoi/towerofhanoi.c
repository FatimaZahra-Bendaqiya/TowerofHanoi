#include "primlib.h"
#include <stdlib.h>
#include <unistd.h>



const int Disksnumber = 3;
const int Pegsnumber = 3;
const int BaseHeight = 20;
const int PegWidth = 18;
const int DiskWidth = 0.8 * (400 / (Pegsnumber * Disksnumber));
const int DiskHeight = Pegsnumber * DiskWidth;
const int PegHeight = Disksnumber * DiskHeight + 20;
const int animationStep = 5;
//pixels the disk will move in each iteration of the animation loop




void drawPegs() {
  // Clear screen
  gfx_filledRect(0, 0, gfx_screenWidth(), gfx_screenHeight(), BLACK);

  // Draw base
  gfx_filledRect(0, gfx_screenHeight() - BaseHeight, gfx_screenWidth(), gfx_screenHeight(), CYAN);

  // Draw poles
  float poleSpacing = gfx_screenWidth() / (Pegsnumber + 1);
  float poleLeft, poleRight;
  for (int i = 0; i < Pegsnumber; i++) {
    poleLeft = (i + 1) * poleSpacing - PegWidth / 2;
    poleRight = (i + 1) * poleSpacing + PegWidth / 2;
    gfx_filledRect(poleLeft, gfx_screenHeight() - PegHeight - BaseHeight, poleRight, gfx_screenHeight() - BaseHeight, BLUE);
  }
}


void drawdisks(int peg, int value, int pos) {
    // Calculate tower dimensions and position
    int x1 = peg * gfx_screenWidth() / (Pegsnumber + 1) - (value)*DiskWidth;
    int y1 = gfx_screenHeight() - ((pos + 1) * DiskHeight) - BaseHeight;
    int x2 = peg * gfx_screenWidth() / (Pegsnumber + 1) + (value)*DiskWidth;
    int y2 = gfx_screenHeight() - (pos * DiskHeight) - BaseHeight;

    // Assign different colors to disks based on size
    int color;
    switch (value) {
        case 1:
            color = RED;
            break;
        case 2:
            color = MAGENTA;
            break;
        case 3:
            color = YELLOW;
            break;
        case 4:
            color = GREEN;
            break;
        case 5:
            color = BLUE;
            break;
        case 6:
            color = WHITE;
            break;
        default:
            color = CYAN;
            break;
    }

    // Draw tower
    gfx_filledRect(x1, y1, x2, y2, color);
    gfx_rect(x1, y1, x2, y2, BLACK);
}



void drawtower(int stacks[Pegsnumber][Disksnumber]) {
	//Draws the current state of the towers of Hanoi game on the screen.
    for (int j = Disksnumber - 1; j >= 0; j--) {
        for (int i = 0; i < Pegsnumber; i++) {
            if (stacks[i][j] != 0) {
                drawdisks(i + 1, stacks[i][j], j);
            }}}
}

void initializeTowers(int stacks[Pegsnumber][Disksnumber]) {
    // Loop through each pole
    for (int poleIndex = 0; poleIndex < Pegsnumber; poleIndex++) {
        // Loop through each disk on the pole
        for (int diskIndex = 0; diskIndex < Disksnumber; diskIndex++) {
            if (poleIndex == 0) {
                // If it's the first pole, assign the disk values in descending order
                stacks[poleIndex][diskIndex] = Disksnumber - diskIndex;
            } else {
                // Otherwise, assign 0 to the disk
                stacks[poleIndex][diskIndex] = 0;
            }
        }
    }
}

int DisksCount(int stacks[Pegsnumber][Disksnumber], int peg) {
	// Count number of disks on specified pole
    int count = 0;
    int j = 0;
    while (j < Disksnumber && stacks[peg - 1][j] != 0) {
        count++;
        j++;
    }
    return count;
}


int removeTopValue(int stacks[Pegsnumber][Disksnumber], int peg, int* moveCount) {
	//Removes and returns the topmost value on a given pole of a set of Hanoi towers,
   // while also incrementing a move count. If the pole is empty, it returns the value 0.
    int value = 0;
    for (int i = Disksnumber - 1; i >= 0; i--) {
        if (stacks[peg - 1][i] != 0) {
            value = stacks[peg - 1][i];
            stacks[peg - 1][i] = 0; // remove the value from the pole
            (*moveCount)++; // increment the move count
            break;
        }
    }
    return value;
}

int get_animation_y_coordinate() {
	//y-coordinate at which the top of the animation should start.
  int PegHeight = Disksnumber * DiskHeight + 20;
  int DiskHeight = Pegsnumber * DiskWidth;
  return gfx_screenHeight() - (PegHeight + 50 + DiskHeight);
}

void MoveDisk(int stacks[Pegsnumber][Disksnumber], int from, int to, int value) {
  int heightFrom = DisksCount(stacks, from);
  int heightTo = DisksCount(stacks, to);

  int xStart = from * gfx_screenWidth() / (Pegsnumber + 1) - (value)*DiskWidth;
  int yStart = gfx_screenHeight() - (heightFrom * DiskHeight);

  int xEnd = to * gfx_screenWidth() / (Pegsnumber + 1) - (value)*DiskWidth;
  int yEnd = gfx_screenHeight() - (heightTo * DiskHeight);

  int currentX = xStart;
  int currentY = yStart;
  int top = get_animation_y_coordinate();


  // animate disk moving up
  while (currentY > top + DiskHeight) {
    drawPegs();
    drawtower(stacks);
    gfx_filledRect(xStart, currentY - DiskHeight, xStart + (value)*2 * DiskWidth, currentY, CYAN);
    gfx_rect(xStart, currentY - DiskHeight, xStart + (value)*2 * DiskWidth, currentY, BLACK);
    gfx_updateScreen();
    currentY -= animationStep;
    SDL_Delay(10);
  }

  // animate disk moving left or right
  while (currentX != xEnd) {
    drawPegs();
    drawtower(stacks);
    gfx_filledRect(currentX, top, currentX + 2 * value * DiskWidth, top + DiskHeight, CYAN);
    gfx_rect(currentX, top, currentX + 2 * value * DiskWidth, top + DiskHeight, BLACK);
    gfx_updateScreen();
    if (currentX > xEnd) {
      currentX -= animationStep;
    } else {
      currentX += animationStep;
    }
    SDL_Delay(10);
  }

  // animate disk moving down
  while (currentY < yEnd) {
    drawPegs();
    drawtower(stacks);
    gfx_filledRect(xEnd, currentY - DiskHeight, xEnd + (value)*2 * DiskWidth, currentY, CYAN);
    gfx_rect(xEnd, currentY - DiskHeight, xEnd + (value)*2 * DiskWidth, currentY, BLACK);
    gfx_updateScreen();
    currentY += animationStep;
    SDL_Delay(10);
  }
}


int checkInput() {
    int a = gfx_pollkey();
    while (a == -1) {
        a = gfx_pollkey();
    }
    if (a == SDLK_ESCAPE) {
        exit(1);
    } else if (a >= SDLK_0 && a <= SDLK_9) {
        a = a - SDLK_0;
        while (a < 1 || a > Pegsnumber) {
            a = gfx_pollkey() - SDLK_0;
        }
        return a;
    } else {
        return checkInput(); // recursively prompt the user for correct input
    }
}

void moveDiskonStack(int stacks[Pegsnumber][Disksnumber], int* moveCount) {
	// This function moves a disk from one stack to another stack based on user input and updates the move count.
    int from = checkInput();
    int to = checkInput();
    int value;
    if ((from >= 0 && from <= Pegsnumber) &&
        (to >= 0 && to <= Pegsnumber) && (from != to)) {
        if (from == 0) {
            from = 10;
        }
        if (to == 0) {
            to = 10;
        }
        int heightFrom = DisksCount(stacks, from);
        int heightTo = DisksCount(stacks, to);
        if (heightFrom == 0) {
            return;
        }
        if (heightTo > 0 && stacks[to - 1][heightTo - 1] < stacks[from - 1][heightFrom - 1]) {
            return;
        }
        value = removeTopValue(stacks, from, moveCount); // pass moveCount to removeTopValue
        MoveDisk(stacks, from, to, value);
        stacks[to - 1][heightTo] = value;
        (*moveCount)++; // increment the move count
    }
}


void Congrats(int moveCount) {
    gfx_filledRect(0, 0, gfx_screenWidth(), gfx_screenHeight(), BLACK);
    gfx_textout(gfx_screenWidth() / 4, gfx_screenHeight() / 2 - 20,
        "Congratulations! You won", MAGENTA);
    char moves[100];
    sprintf(moves, "Number of moves: %d", moveCount);
    gfx_textout(gfx_screenWidth() / 4, gfx_screenHeight() / 2,
        moves, MAGENTA);
    gfx_updateScreen();
    SDL_Delay(4000);
}


int main(int argc, char* argv[]) {
   if (gfx_init()) {
        exit(3);
    }
   int stacks[Pegsnumber][Disksnumber];
   initializeTowers(stacks);
   drawPegs();
   drawtower(stacks);
   gfx_updateScreen();
   int moveCount = 0; // declare and initialize moveCount variable to 0
   while (stacks[Pegsnumber - 1][Disksnumber - 1] != 1) {
	    moveDiskonStack(stacks, &moveCount);
	    drawPegs();
	    drawtower(stacks);
	    gfx_updateScreen();
 }
   // check if all discs are on the last pole
   if (DisksCount(stacks, Pegsnumber) == Disksnumber) {
	   Congrats(moveCount);
	   SDL_Delay(500);
    }
   gfx_getkey();
   return 0;
}

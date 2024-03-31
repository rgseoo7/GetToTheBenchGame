#include "main.h"

#include <stdio.h>
#include <stdlib.h>

#include "gba.h"
#include "images/batman.h"
#include "images/bench.h"
#include "images/loseScreen.h"
#include "images/startScreen.h"
#include "images/endScreen.h"


/* TODO: */
// Include any header files for title screen or exit
// screen images generated by nin10kit. Example for the provided garbage
// image:
// #include "images/garbage.h"

/* TODO: */
// Add any additional states you need for your app. You are not requried to use
// these specific provided states.
enum gba_state {
  START,
  PLAY,
  WIN,
  LOSE,
};



int main(void) {
  /* TODO: */
  // Manipulate REG_DISPCNT here to set Mode 3. //
  REG_DISPCNT =  MODE3 | BG2_ENABLE;
  // Save current and previous state of button input.
  u32 previousButtons = BUTTONS;
  u32 currentButtons = BUTTONS;


  // Load initial application state
  enum gba_state state = START;
  enum gba_state prevState = START;

   struct batman1 bat;
   UNUSED(bat);
   struct bench candies_display[N_CANDIES];
   UNUSED(candies_display);

    //initialize bat x,y position

    struct state curr, prev;
    struct bench *cc, *oc;
    UNUSED(curr);
    UNUSED(prev);
    UNUSED(cc);
    UNUSED(oc);

    curr.bat.x = HEIGHT/2;
    curr.bat.y = WIDTH/2;
    prev.bat.x = HEIGHT/2;
    prev.bat.y = WIDTH/2;

   int livesLeft = 1;
   int candiesEaten = 0;
   prev = curr;

  while (1) {
    currentButtons = BUTTONS; // Load the current state of the buttons
    prev = curr;
    waitForVBlank();
    /* TODO: */
    // Manipulate the state machine below as needed //
    // NOTE: Call waitForVBlank() before you draw
    switch (state) {
      case START: //start, press enter to begin
        waitForVBlank();
        drawFullScreenImageDMA(startScreen);

            if (KEY_JUST_PRESSED(BUTTON_START, currentButtons, previousButtons)) {
                prevState = START;
                state = PLAY;
                    for (int i = 0; i < N_CANDIES; i++) {
                        cc = &candies_display[i];
                        cc->x = randint(0, WIDTH + 160 - CANDY_WIDTH) % 160;
                        cc->y = randint(40, HEIGHT - CANDY_HEIGHT) % 240;
                    }

                 curr.bat.x = WIDTH/2;
                 curr.bat.y = HEIGHT/2;
               int livesLeft = 1;
               int candiesEaten = 0;
                 UNUSED(livesLeft);
                 UNUSED(candiesEaten);
                 UNUSED(prevState);
            }

        break;
      case PLAY:
      // reset game
          if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
            livesLeft = 1;
            candiesEaten = 0;
            state = START;
          }
          //update separately

            if (curr.bat.x <= 240 - CHAR_HEIGHT) {
                if (KEY_JUST_PRESSED(BUTTON_RIGHT, currentButtons, previousButtons)) {
                    prev.bat.y = curr.bat.y;
                    prev.bat.x = curr.bat.x;
                    curr.bat.x += 10;
                }
            }
            if (curr.bat.x >= 0) {
                if (KEY_JUST_PRESSED(BUTTON_LEFT, currentButtons, previousButtons)) {
                    prev.bat.y = curr.bat.y;
                    prev.bat.x = curr.bat.x;
                    curr.bat.x -= 10;
                }
            }
            if (curr.bat.y >= 0) {
                if (KEY_JUST_PRESSED(BUTTON_UP, currentButtons, previousButtons)) {
                    prev.bat.x = curr.bat.x;
                    prev.bat.y = curr.bat.y;
                    curr.bat.y -= 10;
                }
            }
            if (curr.bat.y <= 160 - CHAR_WIDTH) {
                if (KEY_JUST_PRESSED(BUTTON_DOWN, currentButtons, previousButtons)) {
                    prev.bat.x = curr.bat.x;
                    prev.bat.y = curr.bat.y;
                    curr.bat.y += 10;
                }
            }

            for (int i = 0; i < N_CANDIES; i++) {
            // 1) bottom candy < bottom bat < top candy
            // OR
            // 2) bottom candy < top bat < top candy
            //
            // AND
            //
            // 3) left candy < right bat < right candy
            // Or
            // 4) left candy < left bat < right candy


                // bat.x = x coordinate of the screen
                // bat.y = y coordinate of the screen

                int char_bottom = (curr.bat.y + 10);
                int char_top = curr.bat.y;
                int char_left = curr.bat.x;
                int char_right = (curr.bat.x + 10);

                int prev_char_bottom = (prev.bat.y + 10);
                int prev_char_top = prev.bat.y;
                int prev_char_left = prev.bat.x;
                int prev_char_right = (prev.bat.x + 10);

                int bench_bottom = (candies_display[i].y + 10);
                int bench_top = candies_display[i].y;
                int bench_left = candies_display[i].x;
                int bench_right = (candies_display[i].x + 10);


                if (
                    ( ((char_bottom <= bench_bottom) && (char_bottom >= bench_top)) || ((char_top <= bench_bottom) && (char_top >= bench_top)) )
                     &&
                    ( ((char_left >= bench_left) && (char_left <= bench_right)) ||  ((char_right >= bench_left) && ((char_right <= bench_right))) )
                ) {
                   if (
                       !(
                            ( ((prev_char_bottom <= bench_bottom) && (prev_char_bottom >= bench_top)) || ((prev_char_top <= bench_bottom) && (prev_char_top >= bench_top)) )
                             &&
                            ( ((prev_char_left >= bench_left) && (prev_char_left <= bench_right)) || ((prev_char_right >= bench_left) && (prev_char_right <= bench_right)))
                        )
                       ) {
                        candiesEaten += 1;
                        }
                    }
               }



            if (curr.bat.x == WIDTH - CHAR_WIDTH || curr.bat.y == HEIGHT - CHAR_HEIGHT || curr.bat.x == 0 || curr.bat.y == 0) {
                livesLeft -= 1;
            }

            if (candiesEaten >= 2) {
                state = WIN;
            }
            if (livesLeft == 0) {
                state = LOSE;
            }

            char buffer_candies[30];
            snprintf(buffer_candies, 30, "Eaten Candies: %d", candiesEaten);


            waitForVBlank();
            fillScreenDMA(BLACK);
            drawString(140, 30, buffer_candies, WHITE);

           for (int i = 0; i < N_CANDIES;  i++) {
            drawImageDMA(candies_display[i].y, candies_display[i].x, CANDY_WIDTH, CANDY_HEIGHT, bench);
           }
           drawImageDMA(curr.bat.y, curr.bat.x, CHAR_WIDTH, CHAR_HEIGHT, batman);



        break;
      case WIN:
        waitForVBlank();
        drawFullScreenImageDMA(endScreen);
          if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
            livesLeft = 1;
            candiesEaten = 0;
            state = START;
          }

        break;
      case LOSE:
        waitForVBlank();
        drawFullScreenImageDMA(loseScreen);
          if (KEY_JUST_PRESSED(BUTTON_SELECT, currentButtons, previousButtons)) {
            livesLeft = 1;
            candiesEaten = 0;
            state = START;
          }

        break;

}
previousButtons = currentButtons;

    }

  }
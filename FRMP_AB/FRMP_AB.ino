/*
  Fantasy Rampage: https://t-arg.github.io/ID-48-Fantasy-Rampage/

  Arduboy version 1.0:  https://github.com/T-arg/ID-48-Fantasy-Rampage

  MADE by TEAM a.r.g. : https://t-arg.github.io/

  2017 - Trodoss - JO3RI

  License: MIT : https://opensource.org/licenses/MIT

*/

//determine the game
#define GAME_ID 48

#include "globals.h"
#include "menu.h"
#include "font.h"
#include "game.h"

typedef void (*FunctionPointer) ();

const FunctionPointer PROGMEM mainGameLoop[] = {
  stateMenuIntro,
  stateMenuMain,
  stateMenuHelp,
  stateMenuPlay,
  stateMenuInfo,
  stateMenuSoundfx,
  stateGameSelectGameMode,
  stateGameSelectPlayerMode,
  stateGamePlaying,
};


void setup() {
  arduboy.begin();
  arduboy.setFrameRate(30);                                 // set the frame rate of the game at 30 fps
  arduboy.initRandomSeed();                                 // This sets the random to more random, remove this if no random is needed !
  ATM.play(themeSong);
}


void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  ((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
  arduboy.display();
  if (arduboy.everyXFrames(3)) blinking = !blinking;
}


/*
  Fantasy Rampage: https://t-arg.github.io/ID-48-Fantasy-Rampage/

  Arduboy version 2.0:  https://github.com/T-arg/ID-48-Fantasy-Rampage

  MADE by TEAM a.r.g. : https://t-arg.github.io/

  2017 - Trodoss - JO3RI

  2026 - MUSIC and SOUND EFFECTS - JO3RI

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
  stateMenuJbox,
  stateGameSelectGameMode,
  stateGameSelectPlayerMode,
  stateGamePlaying,
};


// called once per frame: the title tune in the menus, the picked tune while playing, a sting at the end of a game
void musicUpdate()
{
  if (gameState == STATE_MENU_JBOX) return;                // the jukebox decides what plays
  byte want = MUSIC_OFF;
  if (arduboy.audio.enabled())
  {
    if (gameState == STATE_GAME_PLAYING)
    {
      want = gameTune;
      if (disp_state == GAME_SHOW_WINNER)
      {
        if (PLAYER_P_score == PLAYER_C_score) want = MUSIC_OFF;
        else want = (PLAYER_C_score > PLAYER_P_score && player_mode == PLAYER_MODE_1_PLAYER) ? MUSIC_LOSE : MUSIC_WIN;
      }
    }
    else if (gameState != STATE_MENU_INTRO || introLetter > INTRO_LETTERS) want = MUSIC_TITLE;   // the title tune starts when the TEAM a.r.g. logo shows
  }
  if (want != musicId) musicPlay(want);
}

void setup() {
  arduboy.boot();
  arduboy.audio.begin();    
  arduboy.setFrameRate(30);                                 // set the frame rate of the game at 30 fps
  arduboy.initRandomSeed();                                 // This sets the random to more random, remove this if no random is needed !
}


void loop() {
  if (!(arduboy.nextFrame())) return;
  arduboy.pollButtons();
  arduboy.clear();
  musicUpdate();
  ((FunctionPointer) pgm_read_word (&mainGameLoop[gameState]))();
  arduboy.display();
  if (arduboy.everyXFrames(3)) blinking = !blinking;
}


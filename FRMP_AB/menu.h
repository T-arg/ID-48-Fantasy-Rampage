#ifndef MENU_H
#define MENU_H

#include "globals.h"
#include "font.h"
#define INTRO_SOUND() sfx(SFX_INTRO)
#include "arduboy_intro.h"

#define SMALL_MONSTERS_ON_ONE_LINE                      8

struct Monsters
{
  public:
    int x, y;
    byte frame;
};

Monsters smallMonster[(SMALL_MONSTERS_ON_ONE_LINE * 2)];

byte smallMonsterLineOne;
byte smallMonsterLineTwo;


void setSmallMonster()
{
  smallMonsterLineOne = 0;
  smallMonsterLineTwo = 0;
  for (byte i = 0; i < (SMALL_MONSTERS_ON_ONE_LINE * 2); i++)
  {
    if (i < SMALL_MONSTERS_ON_ONE_LINE)
    {
      smallMonster[i].x = i * 20;
      smallMonster[i].y = 2;
      smallMonster[i].frame = smallMonsterLineOne;
      smallMonsterLineOne++;
    }
    else
    {
      smallMonster[i].x = (i - SMALL_MONSTERS_ON_ONE_LINE) * 20;
      smallMonster[i].y = 46;
      smallMonster[i].frame = smallMonsterLineTwo;
      smallMonsterLineTwo++;
    }
  }
}


void drawTitleScreen()
{
  if (arduboy.everyXFrames(1))
  {
    for (byte i = 0; i < (SMALL_MONSTERS_ON_ONE_LINE * 2); i++)
    {
      if (i < SMALL_MONSTERS_ON_ONE_LINE)
      {
        smallMonster[i].x--;
        if (smallMonster[i].x < -16)
        {
          smallMonster[i].x = 144;
          smallMonster[i].frame = smallMonsterLineOne;
          smallMonsterLineOne = (++smallMonsterLineOne) % 24;

        }
      }
      else
      {
        smallMonster[i].x++;
        if (smallMonster[i].x > 144)
        {
          smallMonster[i].x = -16;
          smallMonster[i].frame = smallMonsterLineTwo;
          smallMonsterLineTwo = (++smallMonsterLineTwo) % 24;
        }
      }
    }

  }
  sprites.drawSelfMasked(0, 21, titleName, 0);

  for (byte i = 0; i < (SMALL_MONSTERS_ON_ONE_LINE * 2); i++) sprites.drawSelfMasked(smallMonster[i].x, smallMonster[i].y, monstersSmall, smallMonster[i].frame);
}

// twinkling stars around the TEAM a.r.g. logo: each one grows from a pixel to a small cross and shrinks again
#define TWINKLES              5
const byte PROGMEM twinkleXY[] =                              // x, y, start moment (0 - 19) of every star
{
  74, 40, 4,                                                  // on the little star at the bottom right of the logo
  45, 26, 12,   84, 15, 17,   62, 50, 8,   53, 10, 1
};

void drawTwinkles()
{
  for (byte i = 0; i < TWINKLES; i++)
  {
    byte t = (globalCounter + pgm_read_byte(&twinkleXY[i * 3 + 2])) % 20;   // 20 frames for one twinkle
    if (t > 10) t = 20 - t;                                   // grow, then shrink
    byte arm = t / 3;
    if (t)
    {
      byte x = pgm_read_byte(&twinkleXY[i * 3]);
      byte y = pgm_read_byte(&twinkleXY[i * 3 + 1]);
      arduboy.drawFastHLine(x - arm, y, arm * 2 + 1, WHITE);
      arduboy.drawFastVLine(x, y - arm, arm * 2 + 1, WHITE);
    }
  }
}

void stateMenuIntro()
{
  if (!arduboyIntro()) return;                // first the ARDUBOY letters drop in, then the TEAM a.r.g. logo
  globalCounter++;
  sprites.drawSelfMasked(49, 20, T_arg, 0);
  drawTwinkles();
  if (globalCounter > 60)
  {
    gameState = STATE_MENU_MAIN;
    setSmallMonster();
  }
}

void stateMenuMain()
{

  drawTitleScreen();
  for (byte i = 0; i < 4; i++) sprites.drawSelfMasked(19 + 25 * i, 38, menuWords, i);
  sprites.drawSelfMasked(17 + ((menuSelection - 3) * 25), 36, selectorTop, 0);
  sprites.drawPlusMask(16 + ((menuSelection - 3) * 25), 44, selectorMid_plus_mask, 0);

  if (arduboy.justPressed(RIGHT_BUTTON) && (menuSelection < STATE_MENU_JBOX))
  {
    menuSelection++;
    sfx(SFX_MOVE);
  }
  if (arduboy.justPressed(LEFT_BUTTON) && (menuSelection > 3))
  {
    menuSelection--;
    sfx(SFX_MOVE);
  }
  if (arduboy.justPressed(B_BUTTON))
  {
    gameState = menuSelection;
    sfx(SFX_SELECT);
  }
}

void stateMenuHelp()
{
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) gameState = STATE_MENU_MAIN;
}


void stateMenuInfo()
{
  drawTitleScreen();
  print_progmem(14, 38, text_info);
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    gameState = STATE_MENU_MAIN;
    sfx(SFX_BACK);
  }
}

void stateMenuSoundfx()
{
  drawTitleScreen();
  print_progmem(29, 38, text_sound);
  sprites.drawSelfMasked(59 + (arduboy.audio.enabled() * 23), 36, selectorTop, 0);
  sprites.drawPlusMask(58 + (arduboy.audio.enabled() * 23), 44, selectorMid_plus_mask, 0);
  if (arduboy.justPressed(RIGHT_BUTTON))
  {
    arduboy.audio.on();
    sfx(SFX_MOVE);
  }
  if (arduboy.justPressed(LEFT_BUTTON)) arduboy.audio.off();
  if (arduboy.justPressed(A_BUTTON | B_BUTTON))
  {
    arduboy.audio.saveOnOff();
    gameState = STATE_MENU_MAIN;
    sfx(SFX_BACK);
  }
}

// the jukebox: LEFT / RIGHT pick MUSIC or SNDFX, UP / DOWN the number, B plays, A stops (or leaves)
byte jbRow = 0;
byte jbNr[2] = {1, 1};

void stateMenuJbox()
{
  drawTitleScreen();
  byte max = jbRow ? JBOX_SFX : JBOX_MUSIC;
  if (arduboy.justPressed(LEFT_BUTTON)) jbRow = 0;
  if (arduboy.justPressed(RIGHT_BUTTON)) jbRow = 1;
  if (arduboy.justPressed(UP_BUTTON)) jbNr[jbRow] = jbNr[jbRow] % max + 1;
  if (arduboy.justPressed(DOWN_BUTTON)) jbNr[jbRow] = (jbNr[jbRow] + max - 2) % max + 1;
  if (arduboy.justPressed(B_BUTTON))
  {
    if (jbRow) sfx(jbNr[1] - 1);
    else if (arduboy.audio.enabled()) musicPlay(jbNr[0] - 1);
  }
  if (arduboy.justPressed(A_BUTTON))
  {
    if (musicId != MUSIC_OFF) musicPlay(MUSIC_OFF);
    else gameState = STATE_MENU_MAIN;      // musicUpdate restarts the title tune
  }
  sprites.drawSelfMasked(19, 38, jboxMusic, 0);   // the word MUSIC
  sprites.drawSelfMasked(69, 38, jboxSndfx, 0);   // the word SNDFX
  for (byte i = 0; i < 2; i++)
  {
    if (jbRow != i || (arduboy.frameCount & 16)) print_score(49 + 50 * i, 38, jbNr[i]);
    // a small arrow above (3 pixels + 1 on top) and below (1 + 3 pixels) the number
    byte ax = 52 + 50 * i;
    arduboy.drawPixel(ax + 1, 35, WHITE);
    arduboy.drawFastHLine(ax, 36, 3, WHITE);
    arduboy.drawFastHLine(ax, 44, 3, WHITE);
    arduboy.drawPixel(ax + 1, 45, WHITE);
  }
}


#endif

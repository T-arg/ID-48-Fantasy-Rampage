#ifndef ARDUBOY_INTRO_H
#define ARDUBOY_INTRO_H

// ARDUBOY intro: the 7 letters slide in one by one from the right and stop in a row in the middle of the screen.
// The sound effect INTRO_SOUND plays a note when a letter lands and sends a cue (1..7) at that moment (ATM_CUE),
// that cue starts the next letter. So the sound times the intro. Without sound the letters use a timer.
//
// Use:  in the loop of the intro state
//         if (arduboyIntro()) { /* done, go to the next state */ }
//       (A or B skips it. Set introLetter = 0 before you show it again.)

#define INTRO_LETTERS     7       // A R D U B O Y
#define INTRO_SLIDE       7       // frames a letter needs to slide in, then 2 frames to bounce back (together a bit less than the time between two notes)
#define INTRO_BOUNCE      2       // pixels a letter goes too far to the left (it comes back 1 pixel per frame: keep it at 2)
#define INTRO_LATE        12      // frames after which the next letter starts anyway (no sound)
#define INTRO_HOLD        30      // frames the word stays
#define INTRO_X           36      // (128 - 7 * 8) / 2
#define INTRO_Y           28

const unsigned char PROGMEM arduboyLetters[] =
{
  // width, height,
  8, 8,
  // FRAME 00 - 06 : A R D U B O Y
  0x38, 0x7d, 0x7d, 0x75, 0x75, 0x7f, 0x7e, 0x00, 0x7f, 0x7f, 0x7f, 0x0d, 0x0d, 0x7f, 0x76, 0x00, 0x7f, 0x7f, 0x7f, 0x79, 0x79, 0x7f, 0x3e, 0x00, 0x3f, 0x7f, 0x7f, 0x78, 0x78, 0x7f, 0x3f, 0x00, 0x7f, 0x7f, 0x7f, 0x75, 0x75, 0x7f, 0x3a, 0x00, 0x3e, 0x7f, 0x7f, 0x79, 0x79, 0x7f, 0x3e, 0x00, 0x4f, 0x5f, 0x5f, 0x58, 0x78, 0x7f, 0x3f, 0x00,
};

byte introLetter = 0;       // the letter that is sliding in (7 = all landed)
byte introFrame = 0;        // frames since the last letter landed (counts the hold when all landed)

// draws one frame of the intro, returns true when it is finished (or skipped)
bool arduboyIntro()
{
  if (arduboy.justPressed(A_BUTTON | B_BUTTON)) introLetter = INTRO_LETTERS + 1;
  if (introLetter > INTRO_LETTERS) return true;
  if (introLetter == 0 && introFrame == 0) INTRO_SOUND();             // start the notes
  for (byte i = 0; i <= introLetter && i < INTRO_LETTERS; i++)
  {
    int x = INTRO_X + i * 8;
    if (i == introLetter && introFrame < INTRO_SLIDE + 2)
    {
      int t = x - INTRO_BOUNCE;                                        // the new letter goes 2 pixels too far ...
      x = (introFrame < INTRO_SLIDE) ? t + ((127 - t) * (INTRO_SLIDE - introFrame)) / INTRO_SLIDE : t + introFrame - INTRO_SLIDE;
    }                                                                  // ... and comes back to its place
    sprites.drawSelfMasked(x, INTRO_Y, arduboyLetters, i);
  }
  introFrame++;
  if (introLetter < INTRO_LETTERS)
  {
    if (ATM.check() || introFrame > INTRO_LATE) { introLetter++; introFrame = 0; }   // the cue: this letter has landed
  }
  else if (introFrame > INTRO_HOLD) introLetter++;                    // all letters stay for a moment
  return false;
}

#endif

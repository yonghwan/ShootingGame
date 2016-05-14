/**
 * Controllers control what's going on.
 * One of them is the title screen, another in other files runs the game itself.
 */

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "SDL/SDL.h"
#include "FieldObject.h"
#include "Global.h"

class Controller {
      public:
             virtual int step(int input,SDL_Surface* Screen) = 0;
             virtual ~Controller();
};


/**
 * TitleController controls the title screen.
 * 
 */
class TitleController: public Controller {
      protected:
                SDL_Surface* BGImage;
                SDL_Surface* buttons;
                int downPresses;
                int upPresses;
                int escPresses;
                int mode;
                int runcounter;
                SDL_Rect spriteClip;
                SDL_Rect posRect;
                
                int* xPos;
                int* yPos;
                
                int* xOff;
                int* yOff;
                
                static const int MODE_INIT;
                static const int MODE_MENU;
                static const int MODE_EXIT;
                static const int MODE_START;
                static const int MODE_RESET;
                static const int NUM_OPTNS;
                
                int sel;
                
      public:
             TitleController();
             ~TitleController();
             int step(int input,SDL_Surface* Screen);
};


#endif


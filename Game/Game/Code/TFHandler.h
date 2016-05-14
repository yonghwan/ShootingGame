#ifndef FONTHANDL_H
#define FONTHANDL_H
#include "SDL/SDL.h"
class TFHandler {
      public:
      TFHandler(SDL_Surface* typeface,int charw,int charh);
      ~TFHandler();
      bool display_int(SDL_Surface* onto,int x,int y,int input);
      bool display_text(SDL_Surface* onto,int x,int y,const char* input,int from,int to);
      
      protected:
      SDL_Surface* typeface;
      SDL_Rect clipRect;
      SDL_Rect posRect;
      int charOffsetX(char c);
      int charOffsetY(char c);
};

#endif

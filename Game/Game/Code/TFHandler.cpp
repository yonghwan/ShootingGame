#include "TFHandler.h"
#include <cmath>

TFHandler::TFHandler(SDL_Surface* typeface,int charw,int charh) {
    this->typeface = typeface;
    clipRect.h = charh;
    clipRect.w = charw;
    posRect.y = 0;
}

//Draws an integer to a surface.
//Draws the integer backwards from the selected point btw.
bool TFHandler::display_int(SDL_Surface* onto,int x,int y,int input) {
    if (input < 0) return false;
    int len = int(floor(log(input))) + 1;
    posRect.x=x;
    posRect.y=y;
    clipRect.y = clipRect.h; //Use numeric row
    clipRect.x = 0;
    while (input >= 0) {
          int component = input % 10;
          clipRect.x = clipRect.w * component;
          posRect.x-=clipRect.w;
          SDL_BlitSurface(typeface,&clipRect,onto,&posRect);
          input /= 10;
          if (input == 0) break;
    }
    return true;
}

//Draws a char* to the screen from "from" to "to".
//Draws forwards.
bool TFHandler::display_text(SDL_Surface* onto,int x,int y,const char* input,int from,int to) {
     posRect.x=x;
     posRect.y=y;
     for (int i=from;i<to;i++) {
         clipRect.x=charOffsetX(input[i]);
         clipRect.y=charOffsetY(input[i]);
         posRect.x = x + (i-from)*clipRect.w;
         SDL_BlitSurface(typeface,&clipRect,onto,&posRect);
     }
     return true;
}

int TFHandler::charOffsetX(char c) {
    int to = (int)c;
    if (to > 64 && to <= 90) { //Uppercase letter
       return clipRect.w * (to-65);
    } else if (to>96 && to<=122) { //Lowercase letter
       return clipRect.w * (to-97);
    } else if (to >= 48 && to < 58) { //Number
       return clipRect.w * (to-48);
    } else if (to >= 33 && to <= 47) {//If we need to display anything else put it here
       return clipRect.w * (to-33+10);
       
    }
};
int TFHandler::charOffsetY(char c) {
    int to = (int)c;
    if (to > 64 && to <=90) return 0;
    if (to > 96 && to <=122) return 0;
    return clipRect.h;
};

TFHandler::~TFHandler() {
    delete typeface;
}

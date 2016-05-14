#ifndef PLAYER_H
#define PLAYER_H


#include "SDL/SDL.h"
#include <string>
#include "SDL/SDL_image.h"
#include "FieldObject.h"
#include "Global.h"
#include "PShot.h"

class Player : public FieldObject {
      
      
      public:
      //const float focusedSpeed;
      Player(float initx,float inity,SDL_Surface* inSprite);
      //Player(float initx,float inity,std::string filename);
      void step();
      void draw(SDL_Surface* onto);
      ~Player();
      void takeCommands(Uint32 input);
      bool collision(int type);
      Hitbox hitbox();
      Hitbox ihitbox(); //Item hitbox.
      float getx();
      float gety();
      int invince;
      bool bombing;
      
      protected:
      int cooldown;
      static const int spriteWidth;
      static const int spriteHeight;
      static const int offsetx;
      static const int offsety;
      static const float speed;
      static const float focusedSpeed;
      static const int dirgox[4];
      static const int dirgoy[4];
      static const int s_offx;
      static const int s_offy;
      static const float hbrad;
      static const float icrad;
      bool focused;
      SDL_Rect* spriteClip;
      SDL_Rect* posRect;
};

#endif

#ifndef HOSTILE_H
#define HOSTILE_H

#include "FieldObject.h"
#include "SDL/SDL.h"
#include "Hitbox.h"

/**
 * Hostile.
 * Intended behavior:
 *  Move automatically, destroy when offscreen.
 *  Collide with player. (Or rather, provide a hitbox,
 *   which the FieldController checks if it collides with the player.)
 */
class Hostile: public FieldObject {
      public:
      Hostile(float x, float y,float xspeed,float yspeed,
              SDL_Surface* sprite,
              int offsetx,int offsety,int spr_w,int spr_h);
      virtual ~Hostile();
      virtual void step();
      virtual void draw(SDL_Surface* onto);
      virtual bool collision(int type);
      virtual Hitbox hitbox();
      
      protected:
	  int offsetx;
	  int offsety;
      SDL_Rect* spriteClip;
      SDL_Rect* posRect;
	  float xspeed;
	  float yspeed;
      
};

#endif



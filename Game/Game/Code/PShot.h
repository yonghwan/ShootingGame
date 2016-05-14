#ifndef PSHOT_H
#define PSHOT_H

#include "FieldObject.h"
#include "SDL/SDL.h"

/**
 * Player shots.
 * Intended behavior:
 *  Move up screen, destroy when offscreen.
 */
class PShot: public FieldObject {
      public:
      PShot(float x, float y);
      virtual ~PShot();
      virtual void step();
      virtual void draw(SDL_Surface* onto);
      virtual bool collision(int type);
      virtual Hitbox hitbox();
      
      protected:
      SDL_Rect* spriteClip;
      static SDL_Rect posRect;
      
      private:
      const static int speed;
      const static int offsetx;
      const static int offsety;
      
};

class Bomb: public PShot {
      public:
      Bomb(float x,float y,float speed,float angle);
      ~Bomb();
      virtual void step();
      virtual void draw(SDL_Surface* onto);
      virtual bool collision(int type);
      virtual bool collidable();
      virtual Hitbox hitbox();
      
      protected:
      float xspeed;
      float yspeed;
      const static int offsetx;
      const static int offsety;
      int glow;
};


#endif

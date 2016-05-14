#ifndef NEUTRAL_H
#define NEUTRAL_H

#include "FieldObject.h"
#include "SDL/SDL.h"
#include "Hitbox.h"

class Neutral: public FieldObject {
      public:
      Neutral(float x, float y,float xspeed,float yspeed,
              SDL_Surface* sprite,
              int offsetx,int offsety,int spr_w,int spr_h);
      virtual ~Neutral();
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

class Explo: public Neutral {
      public:
      Explo(float x,float y, float speed, float direction);
      void step();
      protected:
      int ticker;
};

class Item:public Neutral {
      public:
      Item(float x,float y,float xspeed,float yspeed);
      virtual void step();
      virtual bool collidable();
      virtual bool collision(int type);
      
};
class PExplo:public Neutral {
      public:
      PExplo(float x,float y, float speed, float direction);
      void step();
};

//Forward declaration
class TFHandler;

class StageClear: public Neutral {
      public:
      StageClear();
      void step();
      virtual void draw(SDL_Surface* onto);
      
      protected:
      TFHandler* font;
      SDL_Rect clipRect;
      SDL_Rect posRect;
      int width;
      int height;
      int rect_minx;
      int rect_maxx;
      int rect_miny;
      int rect_maxy;
      int aminx;
      int amaxx;
      int aminy;
      int amaxy;
      int mode;
      int bombBonus;
      int lifeBonus;
      int ticker;
};


#endif

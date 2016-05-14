#include "Hostile.h"
#include "FieldController.h"
#include "Global.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include <cmath>


//Configure hostile.
//Params:
//x and y are position
//xspeed and yspeed are initial speeds
//offsetx and offsety are sprite offsets
//h and w are height and width of sprite
Hostile::Hostile(float x,float y,float xspeed,float yspeed,
                 SDL_Surface* sprite,
                 int offsetx,int offsety,int spr_w,int spr_h
                 ) {
      this->x = x;
      this->y = y;
      this->xspeed = xspeed;
      this->yspeed = yspeed;
      this->sprite = sprite;      
      this->next = fieldControl -> getHead(0);
      this->prev = NULL; //Initialize to NULL
      if (this->next!=NULL) {
         this->next->prev = this;
      }
      fieldControl -> setHead(0,this);
      spriteClip = new SDL_Rect;
      spriteClip->x = offsetx;
      spriteClip->y = offsety;
      spriteClip->w = spr_w;
      spriteClip->h = spr_h;
      this->offsetx=spr_w/2;
      this->offsety=spr_h/2;
      
      posRect = new SDL_Rect;
	  posRect->x=0;
	  posRect->y=0;
	  posRect->w=0;
	  posRect->h=0;
	  
	  constructors++;
	  
}

Hostile::~Hostile() {
      if (next != NULL) {
         next -> prev = prev;
      }
      if (prev != NULL) {
         prev -> next = next;
      } else {
         //Set shot head
         fieldControl -> setHead(0,next);
      }
      delete spriteClip;
      delete posRect;
      
      destructors++;
}

void Hostile::step() {
     x += xspeed;
     y += yspeed;
}

void Hostile::draw(SDL_Surface* onto) {
     //apply_surface(int(x),int(y),fieldControl->shotSprite,onto);
     
     posRect->x = int(x) - offsetx;
     posRect->y = int(y) - offsety;
     SDL_BlitSurface(sprite,spriteClip,onto,posRect);
}

bool Hostile::collision(int type) {
     //this->setDestroy = true;
     return false;
}

Hitbox Hostile::hitbox() {
       return Hitbox(x-offsetx,x+offsetx,y-offsety,y+offsety);
}

#include "PShot.h"
#include "FieldController.h"
#include "Global.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"


const int PShot::speed = 18;
const int PShot::offsetx = 4;
const int PShot::offsety = 12;
SDL_Rect PShot::posRect;

const int Bomb::offsetx = 24;
const int Bomb::offsety = 6;

PShot::PShot(float x,float y) {
                   
      this->x = x;
      this->y = y;
      this->sprite = fieldControl -> shotSprite;      
      this->next = fieldControl -> getHead(1);
      this->prev = NULL; //Initialize to NULL
      if (this->next!=NULL) {
         this->next->prev = this;
      }
      fieldControl -> setHead(1,this);
      spriteClip = new SDL_Rect;
      spriteClip->x = 0;
      spriteClip->y = 0;
      spriteClip->w = 8;
      spriteClip->h = 24;
}

PShot::~PShot() {
      if (next != NULL) {
         next -> prev = prev;
      }
      if (prev != NULL) {
         prev -> next = next;
      } else {
         //Set shot head
         fieldControl -> setHead(1,next);
      }
      delete spriteClip;
}

void PShot::step() {
      y -= speed;
      if (y < -speed) {
            delete this;
      }
}

void PShot::draw(SDL_Surface* onto) {
     
     posRect.x = int(x) - offsetx;
     posRect.y = int(y) - offsety;
     SDL_BlitSurface(sprite,spriteClip,onto,&posRect);
}

bool PShot::collision(int type) {
     delete this;
     return true;
}

Hitbox PShot::hitbox() {
       return Hitbox(x-offsetx,x+offsetx,y-offsety,y+offsety);
}

Bomb::Bomb(float x,float y,float speed,float angle)
     :PShot(x,y) {
    spriteClip->x=8;
    spriteClip->w=48;
    spriteClip->h=12;
    this->xspeed = speed*cos(angle*M_PI/180);
    this->yspeed = speed*sin(angle*M_PI/180);
    glow = 3;
}

Bomb::~Bomb() {
    
}

void Bomb::draw(SDL_Surface* onto) {
     posRect.x = int(x) - offsetx;
     posRect.y = int(y) - offsety;
     SDL_BlitSurface(sprite,spriteClip,onto,&posRect);
}

void Bomb::step() {
     x += xspeed;
     y += yspeed;
     glow -=1;
     if (glow <= 0) {
        spriteClip->y = (spriteClip->y+12)%24;
        glow =3;
     }
     if (y<=-offsety
         ||x>=fieldControl->FIELD_WIDTH+offsetx
         ||x<=-offsetx
         ||y>=fieldControl->FIELD_HEIGHT+offsety) {
         delete this;
     }
}

bool Bomb::collision(int type) {
     return false;
}

bool Bomb::collidable() {
     return true;
}

Hitbox Bomb::hitbox() {
       return Hitbox(x-offsetx,x+offsetx,y-offsety,y+offsety);
}

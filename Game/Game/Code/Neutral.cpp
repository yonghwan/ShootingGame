#include "Neutral.h"
#include "FieldController.h"
#include "Global.h"
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "TFHandler.h"


extern AudioController* audioControl;

Neutral::Neutral(float x,float y,float xspeed,float yspeed,
                 SDL_Surface* sprite,
                 int offsetx,int offsety,int spr_w,int spr_h
                 ) {
      this->x = x;
      this->y = y;
      this->xspeed = xspeed;
      this->yspeed = yspeed;
      this->sprite = sprite;      
      this->next = fieldControl -> getHead(2);
      this->prev = NULL; //Initialize to NULL
      if (this->next!=NULL) {
         this->next->prev = this;
      }
      fieldControl -> setHead(2,this);
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
	  
}

Neutral::~Neutral() {
      if (next != NULL) {
         next -> prev = prev;
      }
      if (prev != NULL) {
         prev -> next = next;
      } else {
         //Set neutral head
         fieldControl -> setHead(2,next);
      }
      delete spriteClip;
      delete posRect;
}

void Neutral::step() {
     x += xspeed;
     y += yspeed;        
}

void Neutral::draw(SDL_Surface* onto) {
     //apply_surface(int(x),int(y),fieldControl->shotSprite,onto);
     
     posRect->x = int(x) - offsetx;
     posRect->y = int(y) - offsety;
     SDL_BlitSurface(sprite,spriteClip,onto,posRect);
}

bool Neutral::collision(int type) {
     //this->setDestroy = true;
     return false;
}

Hitbox Neutral::hitbox() {
       return Hitbox(x-offsetx,x+offsetx,y-offsety,y+offsety);
}



Explo::Explo(float x,float y,float speed,float direction)
      :Neutral(x,y,speed*cos(direction*M_PI/180),speed*sin(direction*M_PI/180),
      fieldControl->neutralImage,
      0,0,32,32) {
      audioControl->play_sound(AudioController::SOUND_SHOT);
      ticker = 0;
}

void Explo::step() {
     x+=xspeed;
     y+=yspeed;
     ticker+=1;
     if (ticker>=3) {
         ticker = 0;
         spriteClip->x += 32;
         if (spriteClip->x >= sprite->w) delete this;
         else if (x<0-offsetx) delete this;
         else if (x>FieldController::FIELD_WIDTH+offsetx) delete this;
         else if (y<0-offsety) delete this;
         else if (y>FieldController::FIELD_HEIGHT+offsety) delete this; 
     }
}

Item::Item(float x,float y,float xspeed,float yspeed)
     :Neutral(x,y,xspeed,yspeed,fieldControl->neutralImage,32,32,24,24) {
     if (yspeed>-4) yspeed=-4;
}

void Item::step() {
     x+=xspeed;
     y+=yspeed;
     
     yspeed+=0.1;
     /*if (xspeed<0) {
        xspeed+=0.1;
        if (xspeed>0) xspeed=0;
     }
     if (xspeed>0) {
        xspeed-=0.1;
        if (xspeed<0) xspeed=0;
     }*/
     if (y>FieldController::FIELD_HEIGHT+offsety) delete this;
}
bool Item::collidable() {
     return true;
}
bool Item::collision(int type) {
     fieldControl->accScore(10*((FieldController::FIELD_HEIGHT-int(y))/10) + 200);
     delete this;
     return true;
}

PExplo::PExplo(float x,float y,float speed,float direction)
       :Neutral(x,y,speed*cos(direction*M_PI/180),speed*sin(direction*M_PI/180),
       fieldControl->neutralImage,0,32,32,32) {
//Nothing.
}

void PExplo::step() {
     x+=xspeed;
     y+=yspeed;
     if (x<0-offsetx) delete this;
     else if (x>FieldController::FIELD_WIDTH+offsetx) delete this;
     else if (y<0-offsety) delete this;
     else if (y>FieldController::FIELD_HEIGHT+offsety) delete this;    
}

StageClear::StageClear()
           :Neutral(0,0,0,0,fieldControl->frameImage,0,0,0,0) {
    this->font = fieldControl->font;
    
    clipRect.w = 16;
    clipRect.h = 16;
    rect_minx = fieldControl->FIELD_WIDTH / 2;
    rect_miny = fieldControl->FIELD_HEIGHT / 2;
    rect_maxx = rect_minx;
    rect_maxy = rect_miny;
    aminx = rect_minx - 128;
    aminy = rect_miny - 128;
    amaxx = rect_maxx + 128;
    amaxy = rect_maxy + 128;
    mode = 0;
    ticker = 0;
    lifeBonus = 3200 * fieldControl->lives;
    bombBonus = 800 * fieldControl->bombs;
}

void StageClear::step() {
     switch (mode) {
            case 0:
            if (rect_minx <=aminx && rect_miny <=aminy 
             && rect_maxx >=amaxx && rect_maxy >=amaxy) {
                mode = 1;
            } else {
              if (rect_minx > aminx) rect_minx -= 16;
              if (rect_miny > aminy) rect_miny -= 16;
              if (rect_maxx < amaxx) rect_maxx += 16;
              if (rect_maxy < amaxy) rect_maxy += 16;
            }
            break;
            
            case 1:
            ticker++;
            if (ticker >= 240) {
               fieldControl->accScore(8000+lifeBonus+bombBonus);
               mode = 2;
            }
            break;
            
            case 2:
            if (rect_minx >= rect_maxx && rect_miny >= rect_maxy) {
               fieldControl->endGame = true;
               return;
            } else {
            
              if (rect_minx < rect_maxx) {
                 rect_minx += 16;
                 rect_maxx -= 16;
              }
              if (rect_miny < rect_maxy) {
                 rect_miny += 16;
                 rect_maxy -= 16;
              }
            }
            break;
     }
}

void StageClear::draw(SDL_Surface* onto) {
  //Draw corners
  //Draw topleft
  posRect.x=rect_minx-clipRect.w;
  posRect.y=rect_miny-clipRect.h;
  clipRect.x=0;
  clipRect.y=0;
  SDL_BlitSurface(sprite,&clipRect,onto,&posRect);
  //Draw topright
  posRect.x=rect_maxx;
  clipRect.x=16;
  SDL_BlitSurface(sprite,&clipRect,onto,&posRect);
  //Draw bottomright
  posRect.y=rect_maxy;
  clipRect.y=16;
  SDL_BlitSurface(sprite,&clipRect,onto,&posRect);
  //Draw bottomleft
  posRect.x=rect_minx-clipRect.w;
  clipRect.x=0;
  SDL_BlitSurface(sprite,&clipRect,onto,&posRect);
  //Draw sides
  //Draw top
  posRect.y=rect_miny-16;
  clipRect.x=48;
  clipRect.y=0;
  for (posRect.x=rect_minx;posRect.x<rect_maxx;posRect.x+=16) {
      SDL_BlitSurface(sprite,&clipRect,onto,&posRect);
  }
  //Draw left
  posRect.x=rect_minx-16;
  clipRect.x=32;
  for (posRect.y=rect_miny;posRect.y<rect_maxy;posRect.y+=16) {
      SDL_BlitSurface(sprite,&clipRect,onto,&posRect);
  }
  //Draw right
  posRect.x=rect_maxx;
  clipRect.y=16;
  for (posRect.y=rect_miny;posRect.y<rect_maxy;posRect.y+=16) {
      SDL_BlitSurface(sprite,&clipRect,onto,&posRect);
  }
  //Draw bottom
  posRect.y=rect_maxy;
  clipRect.x=48;
  for (posRect.x=rect_minx;posRect.x<rect_maxx;posRect.x+=16) {
      SDL_BlitSurface(sprite,&clipRect,onto,&posRect);
  }
  //Draw center
  clipRect.x=64;
  clipRect.y=0;
  posRect.y=rect_miny;
  for (posRect.x=rect_minx;posRect.x<rect_maxx;posRect.x+=16) {
      for (posRect.y=rect_miny;posRect.y<rect_maxy;posRect.y+=16) {
          SDL_BlitSurface(sprite,&clipRect,onto,&posRect);
      }
  }
  
  //Draw text
  if (mode == 1) {
     font->display_text(onto,rect_minx,rect_miny,"ALL CLEAR !!",0,12);
     font->display_text(onto,rect_minx,rect_miny+16,"EARTH IS SAVED",0,14);
     
     if (ticker >= 60) {
          font->display_text(onto,rect_minx,rect_miny+48,"CLEAR BONUS",0,11);
          font->display_int(onto,rect_maxx,rect_miny+48,8000);
     }
     
     if (ticker >= 120) {
        font->display_text(onto,rect_minx,rect_miny+80,"LIVES BONUS",0,11);
        font->display_int(onto,rect_maxx,rect_miny+80,lifeBonus);
        font->display_text(onto,rect_minx,rect_miny+96,"BOMB BONUS",0,10);
        font->display_int(onto,rect_maxx,rect_miny+96,bombBonus);
     }
  }
}

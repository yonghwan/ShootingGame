#include "SDL/SDL.h"
#include <string>
#include "SDL/SDL_image.h"
#include "FieldObject.h"
#include "Player.h"
#include "Global.h"
#include "FieldController.h"
#include "PShot.h"
#include "Neutral.h"


extern AudioController* audioControl;

const float Player::speed = 5.0;
const float Player::focusedSpeed = 2.0;
const int Player::spriteWidth = 48;
const int Player::spriteHeight = 64;
const int Player::offsetx = Player::spriteWidth / 2;
const int Player::offsety = Player::spriteHeight / 2;
const int Player::s_offx = 9;
const int Player::s_offy = 0;
const float Player::hbrad = 2.0;
const float Player::icrad = 36.0;

const int Player::dirgox[4] = {0,-1,0,1};
const int Player::dirgoy[4] = {-1,0,1,0};

Player::Player(float initx, float inity, SDL_Surface* inSprite) {
       this->x = initx;
       this->y = inity;
       this -> sprite = inSprite;
       spriteClip = new SDL_Rect;
       posRect = new SDL_Rect;
       
       cooldown = 0;
       invince = 60;
       bombing = false;
       
       //Configure initial x and y for clipping rectangle
       spriteClip -> x = 0;
       spriteClip -> y = 0;
       spriteClip -> w = spriteWidth;
       spriteClip -> h = spriteHeight;
};

/*
Player::Player(float initx,float inity,std::string filename) {
       this->x = initx;
       this->y = inity;
       sprite = load_image_chromakey(filename,0,0xFF,0xFF);
       spriteClip = new SDL_Rect;
       posRect = new SDL_Rect;
       
       cooldown = 0;
       
       //Configure initial x and y for clipping rectangle
       spriteClip -> x = 0;
       spriteClip -> y = 0;
       spriteClip -> w = 48;
       spriteClip -> h = 64;
}
*/

Player::~Player() {
      delete spriteClip;
      delete posRect;
};

//Commands
void Player::takeCommands(Uint32 input) {
    //32 bit integer.
    //Bit 0 = focus
    //Bit 1-4 = movement
    //Bit 5 = shoot
    //Bit 6 = bomb

    //Move.
    focused = (input & 1 != 0);
    for (int i=0;i<4;i++) {
        input = input >> 1;
        if (input & 1 != 0) {
            x+=dirgox[i] * (focused ? focusedSpeed : speed);
            y+=dirgoy[i] * (focused ? focusedSpeed : speed);
        }
    }
    //Compensate for out of bounds.
    if (x < offsetx) {
          x=offsetx;
    } else if (x > fieldControl -> FIELD_WIDTH - offsetx) {
       x = fieldControl->FIELD_WIDTH - offsetx;
    }
    
    if (y < offsety) {
          y = offsety;
    } else if (y > fieldControl -> FIELD_HEIGHT - offsety) {
       y = fieldControl -> FIELD_HEIGHT - offsety;
    }
    
    //Test whether to shoot.
    input = input >> 1;
    if (input & 1 != 0) {
       
       if (cooldown <= 0) {
            //audioControl->play_sound(AudioController::SOUND_PSHOT);
            new PShot(x-s_offx,y+s_offy);
            new PShot(x+s_offx,y+s_offy);
            cooldown = 3;
       }
    }
    //Test whether to bomb.
    input = input >> 1;
    if (input & 1 != 0) {
       if (fieldControl -> bombs > 0 && bombing == false) {
          fieldControl -> bombs --;
          bombing = true;
          invince = 180;
          cooldown = 160;
          audioControl->play_sound(AudioController::SOUND_BOMB);
       }
    }
    
    if (cooldown > 0) {
       cooldown--;
    }
    
    if (invince != 0) {
       invince --;
       if (bombing) {
          if (cooldown%4 == 0 && cooldown % 40 > 4) {
             
                 float aimOff = 30*float(rand())/float(RAND_MAX);
                 new Bomb(x,y,12,255+aimOff);
             
          }
          if (invince == 0) bombing = false;
       }
    }
};

void Player::draw(SDL_Surface* onto) {
     if (invince%2 == 0) {
             posRect -> x = int(x) - offsetx;
             posRect -> y = int(y) - offsety;
             SDL_BlitSurface(sprite,spriteClip,onto,posRect);
     }
}

void Player::step() {
     //Player's step actions are covered in another function.
}

bool Player::collision(int type) {
     if (type == 0) {
         if (invince == 0) {
            for (float i=0.0;i<=359.0;i+=30.0) {
                new PExplo(x,y,8,i);
            }
            delete this;
            return true;
         }
         return false;
     }
     return true;
}

Hitbox Player::hitbox() {
       return Hitbox(x-hbrad,x+hbrad,y-hbrad,y+hbrad);
}

Hitbox Player::ihitbox() {
       return Hitbox(x-icrad,x+icrad,y-icrad,y+icrad);
}

float Player::getx() {
      return this->x;
}
float Player::gety() {
      return this->y;
}

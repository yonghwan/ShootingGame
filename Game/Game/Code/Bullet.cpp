#include "Bullet.h"
#include "FieldController.h"

Bullet::Bullet(float x,float y,float speed,float angle,
               int offsetx,int offsety)
       :Hostile(x,y,speed*cos(angle*M_PI/180),speed*sin(angle*M_PI/180),fieldControl->bulletsImage,
         offsetx,offsety,16,16) {
};

void Bullet::step() {
     Hostile::step();
     //Else statements to prevent duplicity
     if (x<0-offsetx) delete this;
     else if (x>FieldController::FIELD_WIDTH+offsetx) delete this;
     else if (y<0-offsety) delete this;
     else if (y>FieldController::FIELD_HEIGHT+offsety) delete this;    
}

bool Bullet::collision(int type) {
     switch (type) {
            case 0:
            case 1:
            delete this;
            return true;
     }
     return false;
}

Hitbox Bullet::hitbox() {
       return Hitbox(x-offsetx/2,x+offsetx/2,y-offsety/2,y+offsety/2);
}


RefBullet::RefBullet(float x,float y,float speed,float angle,int offsetx,int offsety)
          :Bullet(x,y,speed,angle,offsetx,offsety) {
          reflected = false;    
}

void RefBullet::step() {
    Hostile::step();
    if (!reflected) {
       if ((x<=0&&xspeed<=0)
       ||(x>=FieldController::FIELD_WIDTH&&xspeed>=0)) {
          xspeed*=-1;
          reflected = true;
       } else if ((y<=0&&yspeed<=0)||
       (y>=FieldController::FIELD_HEIGHT&&yspeed>=0)) {
         yspeed*=-1;
         reflected = true;
       }
    }
}

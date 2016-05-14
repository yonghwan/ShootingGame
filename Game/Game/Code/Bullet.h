#ifndef BULLET_H
#define BULLET_H
#include "Hostile.h"
class Bullet: public Hostile {
      public:
      Bullet(float x,float y,float speed,float angle,int offsetx,int offsety);
      virtual void step();
      virtual bool collision(int type);
      virtual Hitbox hitbox();
      
};
class RefBullet: public Bullet {
      public:
      RefBullet(float x,float y,float speed,float angle,int offsetx,int offsety);
      virtual void step();
      
      protected:
      bool reflected;
};




#endif

#ifndef MIDBOSS_H
#define MIDBOSS_H
#include "Enemy.h"

class MidbossMirror: public Enemy {
      public:
      MidbossMirror();
      virtual ~MidbossMirror();
      void step();
      int mode;
      int smode;
      void explo();
      int ticker;
      void setxspeed(float to);
      void setyspeed(float to);
      int getHP();
      void setHP(int to);
      
      protected:
      float s_aimang;
      float s_astep;
};

class MidbossSpider: public Enemy {
      public:
      MidbossSpider();
      void step();
      virtual ~MidbossSpider();
      MidbossMirror* subserv;
      void explo();
      
      protected:
      int mode;
      int smode;
      int ticker;
      float s_aimang;
};


#endif

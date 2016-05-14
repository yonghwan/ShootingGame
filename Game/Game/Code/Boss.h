#ifndef BOSS_H
#define BOSS_H
#include "Hostile.h"
#include "Global.h"

class Boss : public Hostile {
      public:
      Boss();
      ~Boss();
      void step();
      Hitbox hitbox();
      bool collision(int type);
      bool collidable();
      void burst(float offx=0,float offy=0);
      
      protected:
      int mode;
      int smode;
      int ticker;
      float** aimdir;
      float** turretx;
      float* turrety; //one-dimensional because ys are the same
      bool invinc;
      int HP;
      float speedstep;
      
      static const float HB_XWID;
      static const float HB_YWID;

};

#endif

#ifndef ENEMY_H
#define ENEMY_H
#include "Hostile.h"

class Enemy: public Hostile {
      
      protected:
      int HP;
      
      public:
      Enemy(float x, float y,float xspeed,float yspeed,
            int offsetx,int offsety,int height,int width,int starthp);
      
      virtual void step();
      bool collision(int type);
      bool collidable();
      virtual Hitbox hitbox();
      
};

class Enemy1: public Enemy {
      public:
      Enemy1(float x,float y,float xspeed);
      void step();
      
      protected:
      int cooldown;
	  
};

class Enemy2: public Enemy {
      public:
      Enemy2(float x,float y,float xspeed);
      void step();
      
      protected:
      float re_xspeed;
      int ticker;
};

class Enemy3: public Enemy {
      public:
      Enemy3(float x,float y,float xspeed,float waittime);
      void step();
      
      protected:
      int ticker;
      int waittime;
};

class Enemy4: public Enemy {
      public:
      Enemy4(float x,float y,float speed,float angle);
      void step();
      
      protected:
	  int ticker;
      int mode;
};

class Enemy5: public Enemy {
	public:
	Enemy5(float x,float y,float speed,float angle);
	void step();
	
	protected:
	int ticker;
	int mode;
	//float adir[2];
	float adir;
};

class Enemy6: public Enemy {
      public:
      Enemy6(float x,float yspeed,float angstep,float delay);
      void step();
      
      protected:
      float o_yspeed;
      float ang;
      float angstep;
      int delay;
      int mode;
      int ticker;
      
};

class Enemy7: public Enemy {
      public:
      Enemy7(float x,float y,float maxsspeed,float rof);
      void step();
      
      protected:
      int ticker;
      float maxshotspeed;
      int rof;
};

class Enemy8: public Enemy {
      public:
      Enemy8(float x,float y,float maxsspeed,float rof);
      void step();
      
      protected:
      int ticker;
      float maxshotspeed;
      int rof;
};

#endif

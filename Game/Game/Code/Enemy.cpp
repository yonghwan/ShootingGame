#include "FieldController.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Global.h"
#include "Neutral.h"

extern AudioController* audioControl;

Enemy::Enemy(float x, float y,float xspeed,float yspeed,
            int offsetx,int offsety,int height,int width,int starthp)
      :Hostile(x,y,xspeed,yspeed,fieldControl->enemiesImage,
         offsetx,offsety,height,width) {
      this->HP = starthp;
}

bool Enemy::collidable() {
    return true;
}

void Enemy::step() {
     Hostile::step();
    if (HP <= 0) {
       fieldControl ->accScore(100);
       new Item(x,y,0,-4);
       new Explo(x,y,0,0);
       delete this;
    }
}

bool Enemy::collision(int type) {
    switch (type) {
           case 1:
           HP -= 1;
           if (HP <= 0) return true;
           break;
    }
    return false;
}

Hitbox Enemy::hitbox() {
     return Hitbox(x-offsetx,x+offsetx,y-offsety,y+offsety);
     
}


Enemy1::Enemy1(float x,float y,float xspeed) 
       :Enemy(x,y,xspeed,5,
       0,0,24,24,
       10) {
       cooldown = 0;
}

void Enemy1::step() {
     yspeed -= 0.05;
     if (yspeed <= 0) {
        if (cooldown <= 0) {
            float adir = getDirection(x,y,fieldControl->px,fieldControl->py);
            for (int i=-60;i<=60;i+=30) {
                new Bullet(x,y,3,adir+i,16,0);
            }
            cooldown=20;
        } else cooldown-=1;
     }
     if (y<=0 && yspeed<=0) {
        delete this;
     } else {
       Enemy::step();
     }
     
}

Enemy2::Enemy2(float x,float y,float xspeed)
       :Enemy(x,y,0,4,
       24,0,24,24,
       5) {
       ticker=0;
       re_xspeed = xspeed;
       
       
       //xspeed=0;
       //yspeed=0;
       
       
}

void Enemy2::step() {
     ticker++;
     /*if (ticker % 20 == 0 && ticker <= 320) {
        new Enemy1(x,y,0);
        //new Hostile(x,y,0,8,fieldControl->enemiesImage,0,0,16,16);
     }
     if (ticker >= 640) {
        ticker = 0;
     }*/
     
     if (ticker == 40) {
        yspeed=yspeed/2;
        xspeed = re_xspeed/2;
     }
     if (ticker == 80) {
        yspeed=yspeed/2;
        xspeed=re_xspeed;
     }
     Hostile::step();
     if (HP<=0 || (x<=-12&&xspeed<=0) || (x>=fieldControl->FIELD_WIDTH+12&&xspeed>=0)) {
        if (HP <= 0) {
           fieldControl->accScore(50);
           new Item(x,y,0,-4);
           new Explo(x,y,0,0);
        }
        delete this;
     }
     
}



Enemy3::Enemy3(float x,float y,float xspeed,float waittime)
       :Enemy(x,y,xspeed,0,
       72,(xspeed>0 ? 24 : 0),24,24,
       5) {
       ticker=0;
       this->waittime = int(waittime);
}

void Enemy3::step() {
     ticker++;
     if (ticker%waittime == 0) {
        for (int i=-75;i<=75;i+=30) {
            new Bullet(x,y,2,90+i,16,0);
        }
     }
     if ((x<=-12&&xspeed<=0)||(x>=fieldControl->FIELD_WIDTH+12&&xspeed>=0)) {
        delete this;
        return;
     }
     Enemy::step();
}

Enemy4::Enemy4(float x,float y,float speed,float angle)
       :Enemy(x,y,speed*cos(angle*M_PI/180),speed*sin(angle*M_PI/180),
       0,0,24,24,
       10) {
	   ticker=0;
       mode=0;
}

void Enemy4::step() {
	 
     switch (mode) {
		case 0:
		if (ticker >= 30) {
			xspeed=0;
			yspeed=0;
			ticker = 0;
			mode =1;
		}
		break;
		
		case 1:
		if (ticker == 30) {
			float adir = getDirection(x,y,fieldControl->px,fieldControl->py);
			for (float i=2.0;i<=4.0;i++) {
				//for (float j=-30.0*i;j<=30.0*i;j+=20.0) {
                for (float j=-60.0;j<=60.0;j+=20.0) {
					new Bullet(x,y,(i/2)+1.0,adir+j,0,0);
				}
			}
			audioControl->play_sound(AudioController::SOUND_SHOT);
			ticker = 0;
			mode=2;
			yspeed=-0.75;
		}
		break;
		
		case 2:
		break;
	 }
	 ticker++;
     if ((x<=-12&&xspeed<=0)||(x>=fieldControl->FIELD_WIDTH+12&&xspeed>=0)
		 ||(y<=-12&&yspeed<=0)||(y>=fieldControl->FIELD_HEIGHT+12&&yspeed>=0)) {
        delete this;
        return;
     }
     Enemy::step();
}

Enemy5::Enemy5(float x,float y,float speed,float angle)
       :Enemy(x,y,speed*cos(angle*M_PI/180),speed*sin(angle*M_PI/180),
       48,0,24,24,
       20) {
	   ticker=0;
       mode=0;
}

void Enemy5::step() {
	 switch (mode) {
		case 0:
		if (ticker >= 60) {
			xspeed=0;
			yspeed=0;
			ticker=0;
			mode=1;
		}
		break;
		
		case 1:
		if (ticker >= 10) {
			/*for (int i=0;i<2;i++) {
				adir[i] = getDirection(x+(12*(i==0?-1:1)),y,fieldControl->px,fieldControl->py);
			}*/
			adir = getDirection(x,y,fieldControl->px,fieldControl->py);
			mode = 2;
			ticker=0;
		}
		break;

		case 2:
		for (int i=0;i<2;i++) {
			new Bullet(x+12*(i==0?-1:1),y,5,/*adir[i]*/adir,16,0);
		}
		if (ticker == 30 || ticker == 60) {
			for (float a=0;a<=340;a+=20) {
				new Bullet(x,y,1,adir+a,0,0);
			}
			adir = getDirection(x,y,fieldControl->px,fieldControl->py);
		}
		if (ticker >= 60) {
			ticker = 0;
			mode=3;
			yspeed=-0.75;
			break;
		}
		break;
	 }
	 ticker++;
     if ((x<=-12&&xspeed<=0)||(x>=fieldControl->FIELD_WIDTH+12&&xspeed>=0)
		 ||(y<=-12&&yspeed<=0)||(y>=fieldControl->FIELD_HEIGHT+12&&yspeed>=0)) {
        delete this;
        return;
     }
	 Enemy::step();
}

Enemy6::Enemy6(float x,float yspeed,float inAngstep,float inDelay)
       :Enemy(x,-16,0,yspeed,
       48,24,24,24,
       30) {
       mode = 0;
       ang = 0;
       o_yspeed = yspeed;
       angstep = inAngstep;
       delay = int(inDelay);
       ticker = 0;
}

void Enemy6::step() {
      switch (mode) {
             case 0:
             if (ticker >= 20) {
                ticker = 0;
                mode = 1;
             }
             ticker++;
             break;
             
             case 1:
             if (yspeed > 0.5) {
                yspeed -= 0.5;
             }
             if (ticker >= delay) {
                ang = ang + angstep;
                for (float i=-30;i<=30;i+=15) {
                    new RefBullet(x,y,2,ang+i-90,0,16);
                }
                if (ang >= 360 || ang <= -360) {
                   mode = 2;
                }
                ticker = 0;
             }
             ticker ++;
             break;
             
             case 2:
             if (yspeed < o_yspeed) {
                yspeed += 1;
             }
             if (y>=fieldControl->FIELD_HEIGHT+12&&yspeed>=0) {
                 delete this;
                 return;
             }
             break;
      }
      Hostile::step();
    if (HP <= 0) {
       fieldControl ->accScore(200);
       for (float i=-0.5;i<=0.5;i++) {
           new Item(x,y,i,-4);
       }
       new Item(x,y,0,-4.5);
       new Explo(x,y,0,0);
       delete this;
    }
}

Enemy7::Enemy7(float x,float y,float maxsspeed,float rof)
       :Enemy(x,y,x<192 ? 2 : -2,0,96,x<192?72:48,24,24,8) {
       ticker = 0;
       this->maxshotspeed = maxsspeed;
       this->rof = int(rof);
}
void Enemy7::step() {
     if (ticker >= rof) {
        float aspd = (float(rand())/float(RAND_MAX)) * maxshotspeed + 0.5;
        float adir = (float(rand())/float(RAND_MAX)) * 360;
        new Bullet(x,y,aspd,adir,0,0);
        ticker = 0;
     }
     ticker++;
     if ((x<=-12&&xspeed<=0)||(x>=fieldControl->FIELD_WIDTH+12&&xspeed>=0)) {
        delete this;
        return;
     }
     Enemy::step();
}

Enemy8::Enemy8(float x,float y,float maxsspeed,float rof)
       :Enemy(x,y,x<192 ? 4 : -4,0,96,x<192?24:0,24,24,2) {
       
       ticker = 0;
       this->maxshotspeed = maxsspeed;
       this->rof = int(rof);
}

void Enemy8::step() {
     if (ticker >= rof) {
        float adir = getDirection(x,y,fieldControl->px,fieldControl->py);
        for (float i=0.25;i<=1;i+=0.25) {
            new Bullet(x,y,i*maxshotspeed,adir,16,0);
        }
        audioControl->play_sound(AudioController::SOUND_PSHOT);
        ticker = 0;
     }
     ticker++;
     if ((x<=-12&&xspeed<=0)||(x>=fieldControl->FIELD_WIDTH+12&&xspeed>=0)) {
        delete this;
        return;
     }
     Enemy::step();
}

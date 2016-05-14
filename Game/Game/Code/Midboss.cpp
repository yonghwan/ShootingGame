#include "FieldController.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Global.h"
#include "Neutral.h"
#include "Midboss.h"


extern AudioController* audioControl;

MidbossSpider::MidbossSpider()
              :Enemy(96,-32,0,1,
              0,48,48,48,800) {
    subserv = new MidbossMirror();
    mode = 0;
    smode = 0;
    ticker = 0;
    fieldControl -> pauseTime();
}

MidbossSpider::~MidbossSpider() {
    fieldControl -> unPauseTime();
}

void MidbossSpider::step() {
    //Meter mirror's HP
    if (subserv -> getHP() < 1000) {
       HP = HP - (1000-subserv->getHP());
       subserv->setHP(1000);
    }
    switch (mode) {
           case 0:
           if (y >= fieldControl->FIELD_HEIGHT / 3) {
                 yspeed = 0;
                 subserv->setyspeed(0);
                 if (ticker >= 30) {
                    mode = 1;
                    subserv->mode = 1;
                    ticker = 0;
                    subserv->ticker = 0;
                    HP = 400;
                 }
           } else ticker = 0;
           break;
           
           case 1:
           switch (smode) {
                  case 0:
                  if (ticker >= 10) {
                     smode = 1;
                     ticker = 0;
                     audioControl -> play_sound(AudioController::SOUND_SHOT);
                  }
                  break;
                  
                  case 1:
                  if (ticker % 3 == 0) {
                     for (float i = -4; i<= 4; i+=8) {
                         float adir = getDirection(x+i,y,fieldControl->px,fieldControl->py);
                         new Bullet(x+i,y,4+float(ticker)/10,adir,16,0);
                     }
                  }
                  if (ticker == 15) {
                     float adir = getDirection(x,y,fieldControl->px,fieldControl->py);
                     for (float i = -60;i <= 60;i+=15) {
                         new Bullet(x,y,2,adir+i,0,0);
                     }
                  }
                  if (ticker >= 30) {
                     int ax = offsetx + (rand() % (fieldControl->FIELD_WIDTH-(2*offsetx)));
                     int ay = offsety + (rand() % (fieldControl->FIELD_HEIGHT / 3));
                     xspeed = (float(ax)-x)/60;
                     yspeed = (float(ay)-y)/60;
                     smode = 2;
                     ticker = 0;
                  }
                  break;
                  case 2:
                  if (ticker>=60) {
                     smode = 0;
                     xspeed = 0;
                     yspeed = 0;
                     ticker = 0;
                  }
                  break;
           }
           if (HP <=100) {
              mode = 2;
              subserv -> mode=2;
              smode = 0;
              subserv -> smode = 0;
              xspeed = 0;
              yspeed = 0;
              subserv->setxspeed(0);
              subserv->setyspeed(0);
              explo();
              subserv -> explo();
           }
           break;
           
           case 2:
           switch (smode) {
                  case 0:
                  if (ticker >= 60) {
                     smode = 1;
                     ticker = 0;
                     s_aimang = getDirection(x,y,fieldControl->px,fieldControl->py);
                     audioControl -> play_sound(AudioController::SOUND_SHOT);
                  }
                  break;
                  
                  case 1:
                  if (ticker % 4 == 0) {
                     for (float i = -15;i<= 15;i+=15) {
                         new Bullet(x,y,4,s_aimang+2*i,0,0);
                     }
                  }
                  if (ticker >= 30) {
                     float ax = x + (rand() % 100) - (rand()%100);
                     if (ax < offsetx) ax=offsetx;
                     if (ax > fieldControl->FIELD_WIDTH-offsetx) ax=fieldControl->FIELD_WIDTH-offsetx;
                     float ay;
                     if (y > fieldControl->FIELD_HEIGHT / 5) {
                        ay = offsety + rand()%(fieldControl->FIELD_HEIGHT/5 - offsety);
                     } else {
                       ay = fieldControl -> FIELD_HEIGHT /3;
                     }
                     ticker = 0;
                     yspeed = (float(ay)-y)/30;
                     xspeed = (float(ax)-x)/30;
                     smode = 2;
                  }
                  break;
                  
                  case 2:
                  if (ticker >= 30) {
                     s_aimang = getDirection(x,y,fieldControl->px,fieldControl->py);
                     xspeed = 0;
                     yspeed = 0;
                     ticker = 0;
                     smode = 1;
                     audioControl -> play_sound(AudioController::SOUND_SHOT);
                  }
                  break;
           }
           
           if (HP <= 0) {
              mode = 3;
              subserv -> mode=3;
              smode = 0;
              subserv -> smode = 0;
              xspeed = 0;
              yspeed = 0.25;
              subserv->setxspeed(0);
              subserv->setyspeed(0.25);
              explo();
              subserv -> explo();
              ticker = 0;
              subserv->ticker = 0;
              fieldControl->accScore(2000);
           }
           break;
           
           case 3:
           if (ticker%10 == 0) {
              new Item(x,y,
              2*((float(rand())/float(RAND_MAX))-0.5),
              4);
           }
           if (ticker%5 == 0) {
              new PExplo(x,y,
              (float(rand())/float(RAND_MAX))*4 + 2,
              (float(rand())/float(RAND_MAX))*180
              );
           }
           if (ticker == 30) {
              audioControl -> play_sound(AudioController::SOUND_SHOT);
           }
           if (ticker >= 60) {
              explo();
              audioControl->play_sound(AudioController::SOUND_BOSSDIE);
              delete this;
              return;
           }
           break;
    }
    Hostile::step();
    ticker++;
}



void MidbossSpider::explo() {
    audioControl -> play_sound(AudioController::SOUND_SHOT);
    for (float i=0.0;i<=359.0;i+=30.0) {
        new PExplo(x,y,8,i);
    }
}

void MidbossMirror::explo() {
    for (float i=0.0;i<=359.0;i+=30.0) {
        new PExplo(x,y,8,i);
    }
}

MidbossMirror::MidbossMirror() 
              :Enemy(288,-32,0,1,
              48,48,48,32,
              1000) {
    mode = 0;
    smode = 0;
    ticker = 0;
    s_aimang = 95.0;
    s_astep = -10.0;
}

MidbossMirror::~MidbossMirror() {
 //...Nothing, actually...                               
}

void MidbossMirror::step() {
     switch (mode) {
            case 0:
            //Handled by spider...
            break;
            
            case 1:
            switch (smode) {
                   case 0:
                   if (ticker >=10) {
                      ticker = 0;
                      smode = 1;
                   }
                   break;
                   
                   case 1:
                   if (ticker % 5 == 0) {
                      float adir = getDirection(x,y,fieldControl->px,fieldControl->py);
                      for (float i = 0;i <= 5;i+=1) {
                          new RefBullet(x,y,2,adir+i+6*float(ticker),0,16);
                      }
                   }
                   if (ticker >= 60) {
                      ticker = 0;
                      smode = 2;
                     int ax = offsetx + (rand() % (fieldControl->FIELD_WIDTH-(2*offsetx)));
                     int ay = offsety + (rand() % (fieldControl->FIELD_HEIGHT / 3));
                     xspeed = (float(ax)-x)/120;
                     yspeed = (float(ay)-y)/120;
                   }
                   break;
                   
                   case 2:
                   if (ticker >= 120) {
                      smode = 0;
                      ticker = 0;
                      xspeed = 0;
                      yspeed = 0;
                   }
                   break;
                   
                   
            }
            break;
            
            case 2:
            switch (smode) {
                   case 0:
                   if (ticker >= 30) {
                     ticker = 0;
                     smode = 1;
                     int ax = fieldControl->FIELD_WIDTH/2;
                     int ay = fieldControl->FIELD_HEIGHT/5;
                     xspeed = (float(ax)-x)/30;
                     yspeed = (float(ay)-y)/30;
                   }
                   break;
                   
                   case 1:
                   if (ticker >= 30) {
                      xspeed = 0;
                      yspeed = 0;
                      ticker = 0;
                      smode = 2;
                   }
                   break;
                   
                   case 2:
                   x = fieldControl->FIELD_WIDTH * (0.5 + 0.4*sin(float(ticker)/30));
                   if (ticker%5 == 0) {
                      float adir = getDirection(x,y,fieldControl->px,fieldControl->py);
                      for (float i = -1;i<=1;i+=2) {
                          for (float j = -5;j<=5;j+=2.5) {
                              new Bullet(x,y,6,adir+j+i*s_aimang,16,0);
                          }
                      }
                      s_aimang += s_astep;
                      if ((s_aimang <= 15 && s_astep < 0) 
                       || (s_aimang >= 95) && s_astep > 0) s_astep *= -1;
                   }
                   break;
            }
            break;
            
            case 3:
           if (ticker%10 == 0) {
              new Item(x,y,
              2*((float(rand())/float(RAND_MAX))-0.5),
              4);
           }
           if (ticker%5 == 0) {
              new PExplo(x,y,
              (float(rand())/float(RAND_MAX))*4 + 2,
              (float(rand())/float(RAND_MAX))*180
              );
           }
           if (ticker >= 40) {
              explo();
              audioControl->play_sound(AudioController::SOUND_BOSSDIE);
              delete this;
              return;
           }
            break;
     }
     Hostile::step();
     ticker++;
}

void MidbossMirror::setxspeed(float to) {
     xspeed = to;
}
void MidbossMirror::setyspeed(float to) {
     yspeed = to;
}
int MidbossMirror::getHP() {
     return HP;
}
void MidbossMirror::setHP(int to) {
     HP = to;
}

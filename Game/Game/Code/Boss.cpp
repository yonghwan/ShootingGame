#include "FieldController.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Global.h"
#include "Neutral.h"
#include "Boss.h"
#include "AudioController.h"


const float Boss::HB_XWID = 60;
const float Boss::HB_YWID = 60;

extern AudioController* audioControl;

Boss::Boss()
     :Hostile(196,-115,0,230.0/870.0,
     load_image_chromakey("Sprites/Boss.png",0,0xFF,0xFF),
     0,0,380,230) {
                  
     //Allocate arrays...
     aimdir = new float*[2];
     turretx = new float*[2];
     turrety = new float[3];
     for (int i=0;i<2;i++) {
         aimdir[i] = new float[3];
         turretx[i] = new float[3];
     }
     
     //Initialize array values...
     //Wing turrets
     turretx[0][0] = -83;
     turretx[1][0] = 83;
     turrety[0] = 83;
     //Face turrets
     turretx[0][1] = -17;
     turretx[1][1] = 17;
     turrety[1] = 115;
     //Back turrets
     turretx[0][2] = -5;
     turretx[0][2] = 5;
     turrety[2] = 29;
     
     //Initialize vars
     mode = 0;
     smode = 0;
     ticker = 0;
     HP = 2800;
     speedstep = 0;
     
     //Play music
     audioControl -> play_music(3,1);
     
     //Stop timeline
     fieldControl->pauseTime();
}

Boss::~Boss() {
     //Free the sprite
     SDL_FreeSurface(sprite);
     
     //Free arrays
     for (int i=0;i<2;i++) {
         delete[] aimdir[i];
         delete[] turretx[i];
     }
     delete[] aimdir;
     delete[] turretx;
     delete[] turrety;
     
     //Restart time
     fieldControl->unPauseTime();
}

bool Boss::collidable() {
    return (mode > 0);
}

void Boss::step() {
    switch (mode) {
           case 0:
           if (ticker > 870) {
              audioControl -> play_music(4);
              ticker = 0;
              mode = 1;
              smode = 0;
              yspeed = 0;
           }
           break;
           
           case 1:
           switch (smode) {
                  case 0:
                  if (ticker >= 10) {
                     ticker = 0;
                     for (int i=0;i<2;i++) for (int j=0;j<3;j++) {
                         aimdir[i][j] = getDirection(x+turretx[i][j],y+turrety[j],
                         fieldControl->px,fieldControl->py);
                     }
                     audioControl->play_sound(AudioController::SOUND_RAPID);
                     smode = 1;
                  }
                  break;
                  
                  case 1:
                  if (ticker % 4 == 0) {
                     for (int i=0;i<2;i++) {
                         for (float odir = -75;odir <= 75;odir +=25)
                         new Bullet(x+turretx[i][0],y+turrety[0],6,aimdir[i][0]+odir,16,0);
                     }
                  }
                  if (ticker % 30 == 0) {
                     for (int i=0;i<2;i++) {
                         aimdir[i][1] = getDirection(x+turretx[i][1],y+turrety[1],
                         fieldControl->px,fieldControl->py);
                         for (float odir = -60;odir <= 60;odir += 15) {
                             new Bullet(x+turretx[i][1],y+turrety[1],2,aimdir[i][1]+odir,0,0);
                         }
                     }
                  }
                  if (ticker >= 80) {
                     ticker = 0;
                     smode = 2;
                     float ax =float( rand() % (fieldControl->FIELD_WIDTH/4) + (fieldControl->FIELD_WIDTH*3/8));
                     float ay =float( rand() % int(y));
                     xspeed = (ax-x)/30;
                     yspeed = (ay-y)/30;
                  }
                  break;
                  
                  case 2:
                  if (ticker == 30) {
                     xspeed = 0;
                     yspeed = 0;
                     audioControl->play_sound(AudioController::SOUND_SHOT);
                     for (int i=0;i<2;i++) {
                         aimdir[i][1] = getDirection(x+turretx[i][1],y-turrety[1],
                         offsetx,offsety);
                         for (float odir = -60;odir <= 60;odir += 20) {
                             for (float ospd = 0;ospd <= 0.5;ospd+=0.5) {
                                 new Bullet(x+turretx[i][1],y+turrety[1],2.5+ospd,aimdir[i][1]+odir,0,0);
                             }
                         }
                     }
                  }
                  if (ticker == 60) {
                     float ax = offsetx;
                     float ay = offsety;
                     xspeed = (ax-x)/30;
                     yspeed = (ay-y)/30;
                  }
                  if (ticker > 90) {
                     xspeed = 0;
                     yspeed = 0;
                     smode = 0;
                  }
                  break;
           }
           if (HP <= 2300) {
              ticker = -30;
              smode = 0;
              mode = 2;
              xspeed = 0;
              yspeed = 0;
              burst();
           }
           break;
           
           case 2:
           switch (smode) {
                  case 0:
                  if (ticker >= 10) {
                     ticker = 0;
                     for (int i=0;i<2;i++) for (int j=0;j<3;j++) {
                         aimdir[i][j] = getDirection(x+turretx[i][j],y+turrety[j],
                         fieldControl->px,fieldControl->py);
                     }
                     audioControl->play_sound(AudioController::SOUND_SHOT);
                     smode = 1;
                  }
                  break;
                  
                  case 1:
                  if (ticker % 4 == 0 && ticker <= 40) {
                     for (int i=0;i<2;i++) {
                         for (float odir = -5;odir <= 5;odir +=5)
                         new Bullet(x+turretx[i][1],y+turrety[1],4,aimdir[i][1]+odir,0,0);
                     }
                  }
                  if (ticker % 30 == 0) {
                     for (int i=0;i<2;i++) {
                         aimdir[i][0] = getDirection(x+turretx[i][0],y+turrety[1],
                         fieldControl->px,fieldControl->py);
                         for (float odir = -60;odir <= 60;odir += 20) {
                             new Bullet(x+turretx[i][0],y+turrety[0],2,aimdir[i][0]+odir,16,0);
                         }
                     }
                  }
                  if (ticker >= 80) {
                     ticker = 0;
                     smode = 2;
                     float ax =float( rand() % (fieldControl->FIELD_WIDTH/4) + (fieldControl->FIELD_WIDTH*3/8));
                     float ay =float( rand() % int(y)) / 3;
                     xspeed = (ax-x)/30;
                     yspeed = (ay-y)/30;
                  }
                  break;
                  
                  case 2:
                  if (ticker == 30) {
                     xspeed = 0;
                     yspeed = 1;
                     audioControl->play_sound(AudioController::SOUND_RAPID);
                     for (int i=0;i<2;i++) {
                         aimdir[i][0] = 90;
                     }
                  }
                  if (ticker > 30 && (ticker-30)%4 == 0 && ticker <= 80) {
                      for (int i=0;i<2;i++) {
                          for (float odir = -30;odir <= 30;odir += 20) {
                              new Bullet(x+turretx[i][0],y+turrety[0],5,aimdir[i][0]+odir,16,0);
                         }
                      }
                  }
                  if (ticker == 90) {
                     float ax = offsetx;
                     float ay = offsety;
                     xspeed = (ax-x)/30;
                     yspeed = (ay-y)/30;
                  }
                  if (ticker > 90) {
                     xspeed = 0;
                     yspeed = 0;
                     smode = 0;
                  }
                  break;
           }
           if (HP <= 1800) {
              ticker = 0;
              smode = 0;
              mode = 3;
              xspeed = 0;
              yspeed = 0;
              burst();
           }
           break;
           
           case 3:
           switch (smode) {
                  case 0:
                  if (ticker >= 80) {
                     ticker = 0;
                     for (int i=0;i<2;i++) for (int j=0;j<3;j++) {
                         aimdir[i][j] = getDirection(x+turretx[i][j],y+turrety[j],
                         fieldControl->px,fieldControl->py);
                     }
                     smode = 1;
                  }
                  break;
                  
                  case 1:
                  if (ticker == 60) {
                     xspeed = 0;
                     yspeed = 0;
                  }
                  if (ticker <= 20 && ticker%2 == 0) {
                     for (int i=0;i<2;i++) {
                         new Bullet(x+turretx[i][1],y+turrety[1],3,10*(ticker-1),0,0);
                     }
                  }
                  if (ticker % 30 == 0 || ticker % 30 == 15) {
                     for (int i=0;i<2;i++) {
                         aimdir[i][0] = getDirection(x+turretx[i][0],y+turrety[1],
                         fieldControl->px,fieldControl->py);
                     }
                     audioControl->play_sound(AudioController::SOUND_SHOT);
                  }
                  if (ticker % 30 < 10 && ticker%2 == 0) {
                     new Bullet(x+turretx[0][0],y+turrety[0],4.5,aimdir[0][0],16,0);
                  }
                  if (ticker % 30 >= 15 && ticker % 30 < 25 && ticker%2 == 1) {
                     new Bullet(x+turretx[1][0],y+turrety[0],6,aimdir[1][0],16,0);
                  }
                  if (ticker == 90) {
                     float ax = float( rand() % (fieldControl->FIELD_WIDTH/4) + (fieldControl->FIELD_WIDTH*3/8));
                     float ay = float( rand() % int(y));
                     xspeed = (ax-x)/60;
                     yspeed = (ay-y)/60;
                  }
                  if (ticker == 150) {
                     xspeed = 0;
                     yspeed = 0;
                  }

                  if (ticker == 180) {
                     float ax = offsetx;
                     float ay = offsety;
                     xspeed = (ax-x)/60;
                     yspeed = (ay-y)/60;
                     ticker = 0;
                  }
                  break;
           }
           if (HP <= 1300) {
              ticker = 0;
              smode = 0;
              mode = 4;
              xspeed = 0;
              yspeed = 0;
              burst();
           }
           break;
           
           case 4:
           switch (smode) {
                  case 0:
                  if (ticker >= 10) {
                     xspeed = (offsetx-x)/30;
                     yspeed = (offsety-y)/30;
                     smode = 1;
                     ticker = 0;
                  }
                  break;
                  
                  case 1:
                  if (ticker > 30) {
                     xspeed = 0;
                     yspeed = 0;
                     smode = 2;
                     ticker = 0;
                     for (int i=0;i<2;i++) for (int j=0;j<3;j++) {
                         aimdir[i][j] = 90;
                     }
                  }
                  break;
                  
                  case 2:
                  if (ticker % 3 == 0) {
                     for (int j=0;j<2;j++) {
                         aimdir[0][j] -= 7*(j+1);
                         aimdir[1][j] += 7*(j+1);
                     }
                  }
                  if (ticker % 6 == 0) {
                     for (int i=0;i<2;i++) for (int j=0;j<2;j++) for (int k=0;k<2;k++) {
                         new Bullet(x+turretx[i][j],y+turrety[j],2-j,aimdir[i][j]+180*k,16*j,0);
                     }
                  }
                  break;
                  
           }
           if (HP <= 800) {
              ticker = 0;
              smode = 0;
              mode = 5;
              xspeed = 0;
              yspeed = 0;
              burst();
           }
           break;
           
           case 5:
           switch (smode) {
                  case 0:
                  if (ticker >= 10) {
                     xspeed = (offsetx-x)/30;
                     yspeed = (offsety-y)/30;
                     smode = 1;
                     ticker = 0;
                  }
                  break;
                  
                  case 1:
                  if (ticker > 30) {
                     xspeed = 0;
                     yspeed = 0;
                     smode = 2;
                     ticker = 0;
                     for (int i=0;i<2;i++) for (int j=0;j<3;j++) {
                         aimdir[i][j] = 90;
                     }
                  }
                  break;
                  
                  case 2:
                  if (ticker % 3 == 0) {
                     for (int j=0;j<2;j++) {
                         aimdir[0][j] -= speedstep*(j+1);
                         aimdir[1][j] += speedstep*(j+1);
                     }
                  }
                  if (ticker % 6 == 0) {
                     for (int i=0;i<2;i++) for (int j=0;j<2;j++) for (int k=0;k<2;k++) {
                         new Bullet(x+turretx[i][j],y+turrety[j],2-j,aimdir[i][j]+180*k,16*j,0);
                     }
                  }
                  speedstep += 0.14;
                  break;
                  
           }
           
           if (HP <= 0) {
              ticker = 0;
              smode = 0;
              mode = 6;
              xspeed = 0;
              yspeed = 0;
              burst();
           }
           break;

           case 6:
           switch (smode) {
                  case 0:
                  burst();
                  audioControl->play_sound(AudioController::SOUND_BOSSDIE);
                  ticker = 0;
                  yspeed = 0.25;
                  smode = 1;
                  break;
                  
                  case 1:
                  if (ticker%10 == 0) {
                     for (int i=0;i<4;i++) {
                         new Explo(x-offsetx+2*(float(rand())/float(RAND_MAX))*offsetx,y,4,
                         240+60*(float(rand())/float(RAND_MAX)));
                     }
                     audioControl->play_sound(AudioController::SOUND_BOSSDIE);
                  }
                  if (ticker % 50 == 0) {
                     float ox = (2*(float(rand())/float(RAND_MAX)) - 1)*offsetx;
                     float oy = ((float(rand())/float(RAND_MAX)) - 0.5)*offsety;
                     burst(ox,oy);
                  }
                  if (ticker >= 300) {
                     for (int i=0;i<5;i++) {
                         float ox = (2*(float(rand())/float(RAND_MAX)) - 1)*offsetx;
                         float oy = ((float(rand())/float(RAND_MAX)) - 0.5)*offsety;
                         burst(ox,oy);
                     }
                     delete this;
                     return;
                  }
                  break;
           }
           break;
    }
    ticker++;
    Hostile::step();
}

bool Boss::collision(int type) {
     if (!fieldControl->bombing) {
          HP -= 1; //Immune to bombs
     }
     return false;
}

Hitbox Boss::hitbox() {
       if (mode == 6) {
          return Hitbox(0,0,0,0);
       }
       return Hitbox(x-HB_XWID,x+HB_XWID,y-HB_YWID,y+HB_YWID);
}

void Boss::burst(float offx,float offy) {
     audioControl->play_sound(AudioController::SOUND_SHOT);
     for (int i=0;i<15;i++) {
         float spd = 3 + 3*(float(rand())/float(RAND_MAX));
         float dir = 180*(float(rand())/float(RAND_MAX));
         new PExplo(x+offx,y+offy,spd,dir);
     }
     for (int i=0;i<5;i++) {
         float xoff = 2*(float(rand())/float(RAND_MAX)) -1;
         float yoff = (float(rand())/float(RAND_MAX)) * 0.4;
         new Item(x+offx,y+offy,xoff,-4-yoff);
     }
}

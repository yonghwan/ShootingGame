#include "HiScoreController.h"
#include "Global.h"
#include "AudioController.h"
#include <iostream>
#include <fstream>


extern AudioController* audioControl;
const int HiScoreController::num_hiscores = 10;
const int HiScoreController::HSTable_minx = 64;
const int HiScoreController::HSTable_maxx = 576;
const int HiScoreController::HSTable_miny = 48;
const int HiScoreController::HSTable_maxy = 432;
const int HiScoreController::HSTable_namexpos = 192;
const int HiScoreController::HSTable_scorexpos = 448;
const int HiScoreController::HSTable_listypos = 64;
const int HiScoreController::entryX = 112;
const int HiScoreController::entryY = 256;
const char* HiScoreController::alph = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* HiScoreController::numer = "0123456789!\"#$-----------@";
const int HiScoreController::maxNameLen = 8;
//const char[26] HiScoreController::HS_ALPH = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
//const char[26] HiScoreController::HS_NUMR = "0123456789!\"#$-----------@";

HiScoreController::HiScoreController(int Score = 0) {
	this->score = Score;
	std::ifstream* loadStream = new std::ifstream("Hiscores.dat");
	scoreNames = new std::string*[num_hiscores];
	scoreValues = new int[num_hiscores];
	isHiScore = false;
	playerRank = 10; //0: off scoreboard
	for (int i=0;i<num_hiscores;i++) {
        scoreNames[i] = new std::string();
        (*loadStream) >> *scoreNames[i];
        (*loadStream) >> scoreValues[i];
    }
	delete loadStream;
	mode=0;
	//Load surfaces
	tiles = load_image_chromakey("Sprites/hs_tileset.png",0xFF,0,0);
	text = load_image_chromakey("Sprites/typeface.png",0,0xFF,0xFF);
	select = load_image_chromakey("Sprites/select.png",0,0xFF,0xFF);
	typeface = new TFHandler(
                   text,
                   16,16);
    rect_minx = (HSTable_minx+HSTable_maxx)/2;
    rect_miny = (HSTable_miny+HSTable_maxy)/2;
    rect_maxx = rect_minx;
    rect_maxy = rect_miny;
    clipRect.x=0;
    clipRect.y=0;
    clipRect.w=16;
    clipRect.h=16;
    time = 0;
    selectX=0;
    selectY=0;
    cooldown=0;
    audioControl -> stop_music();
    
    //Handle input score.
    for (int i=0;i<num_hiscores;i++) {
        if (score > scoreValues[i]) {
           isHiScore = true;
           playerRank = i;
           delete scoreNames[num_hiscores-1];
           for (int j=num_hiscores-1;j>i;j--) {
               scoreNames[j] = scoreNames[j-1];
               scoreValues[j] = scoreValues[j-1];
           }
           scoreNames[i] = new std::string();
           scoreValues[i] = score;
           break;
        }
    }
}

HiScoreController::~HiScoreController() {
  //Writeback.
  if (isHiScore) {
    std::ofstream* output = new std::ofstream("Hiscores.dat");
    for (int i=0;i<num_hiscores;i++) {
        (*output) << (*scoreNames[i]) << " " << scoreValues[i] << std::endl;
    }
    delete output;
  }
  
  //Destruction.
  for (int i=0;i<num_hiscores;i++) {
      delete scoreNames[i];
  }
  delete[] scoreNames;
  delete[] scoreValues;
  delete typeface;//Also deletes text
  SDL_FreeSurface(tiles);
  SDL_FreeSurface(select);
}

int HiScoreController::step(int input,SDL_Surface* screen) {
  //Do something or other draw the background
  for (int i=0;i<screen->w;i++) {
      for (int j=0;j<screen->h;j++) {
          Uint32 pixel = 0x00000000;
          if ((i%80) == (((time+80-((j/2)%80))))%80)  pixel |= 0x00FF0000;
          if (((i/2)%80) == (time+(j%80))%80) pixel |= 0x0000FF00;
          if ((i+time)%40 >= 20 ^ (j+40-(time%40))%40 < 20) pixel |= 0x000000FF;
          put_pixel32(screen,i,j,pixel);
      }
  }
  //Draw corners
  //Draw topleft
  posRect.x=rect_minx-clipRect.w;
  posRect.y=rect_miny-clipRect.h;
  clipRect.x=0;
  clipRect.y=0;
  SDL_BlitSurface(tiles,&clipRect,screen,&posRect);
  //Draw topright
  posRect.x=rect_maxx;
  clipRect.x=16;
  SDL_BlitSurface(tiles,&clipRect,screen,&posRect);
  //Draw bottomright
  posRect.y=rect_maxy;
  clipRect.y=16;
  SDL_BlitSurface(tiles,&clipRect,screen,&posRect);
  //Draw bottomleft
  posRect.x=rect_minx-clipRect.w;
  clipRect.x=0;
  SDL_BlitSurface(tiles,&clipRect,screen,&posRect);
  //Draw sides
  //Draw top
  posRect.y=rect_miny-16;
  clipRect.x=48;
  clipRect.y=0;
  for (posRect.x=rect_minx;posRect.x<rect_maxx;posRect.x+=16) {
      SDL_BlitSurface(tiles,&clipRect,screen,&posRect);
  }
  //Draw left
  posRect.x=rect_minx-16;
  clipRect.x=32;
  for (posRect.y=rect_miny;posRect.y<rect_maxy;posRect.y+=16) {
      SDL_BlitSurface(tiles,&clipRect,screen,&posRect);
  }
  //Draw right
  posRect.x=rect_maxx;
  clipRect.y=16;
  for (posRect.y=rect_miny;posRect.y<rect_maxy;posRect.y+=16) {
      SDL_BlitSurface(tiles,&clipRect,screen,&posRect);
  }
  //Draw bottom
  posRect.y=rect_maxy;
  clipRect.x=48;
  for (posRect.x=rect_minx;posRect.x<rect_maxx;posRect.x+=16) {
      SDL_BlitSurface(tiles,&clipRect,screen,&posRect);
  }
  //Draw center
  clipRect.x=64;
  clipRect.y=0;
  posRect.y=rect_miny;
  for (posRect.x=rect_minx;posRect.x<rect_maxx;posRect.x+=16) {
      for (posRect.y=rect_miny;posRect.y<rect_maxy;posRect.y+=16) {
          SDL_BlitSurface(tiles,&clipRect,screen,&posRect);
      }
  }
  if (mode == 1 || mode == 2) {
           for (int i=0;i<num_hiscores;i++) {
               typeface->display_text(screen,HSTable_namexpos,
                                      HSTable_listypos+16*i,scoreNames[i]->c_str(),
                                      0,scoreNames[i]->size());
               typeface->display_int(screen,HSTable_scorexpos,
                                     HSTable_listypos+16*i,(int)scoreValues[i]);
           }
           posRect.x = entryX;
           posRect.y = entryY;
           SDL_BlitSurface(text,NULL,screen,&posRect);
           if (mode==2) {
              posRect.x = entryX+16*selectX;
              posRect.y = entryY+16*selectY;
              SDL_BlitSurface(select,NULL,screen,&posRect);
           }
           
           
  }
  
  
  switch (mode) {
  
         case 0:
         if (rect_minx <= HSTable_minx && rect_miny <= HSTable_miny
             && rect_maxx >= HSTable_maxx && rect_maxy >= HSTable_maxy) {
             if (isHiScore) mode = 2;
             else mode = 1;
         } else {
           if (rect_minx > HSTable_minx) rect_minx -= 16;
           if (rect_miny > HSTable_miny) rect_miny -= 16;
           if (rect_maxx < HSTable_maxx) rect_maxx += 16;
           if (rect_maxy < HSTable_maxy) rect_maxy += 16;
         }
         break;
         
         //Just read hiscore
         case 1:
         if (input >> 5 != 0) {
           mode=3;
         }
         break;
         
         //Enter name
         case 2:
         if (cooldown == 0) {
            for (int i=0;i<=9;i++) {
                if (input & 0x01 > 0) {
                   cooldown = 1; //Minimum of 1
                   switch (i) {
                       case 0:
                       cooldown = 4;
                       break;
                       case 1:
                       case 3:
                       selectY = (selectY+1)%2;
                       break;
                       case 2:
                       selectX = (selectX+25)%26;
                       break;
                       case 4:
                       selectX = (selectX+1)%26;
                       break;
                       case 5:
                       case 7:
                       
                       if (selectX == 25 && selectY == 1) {
                         mode = 1;
                       } else {
                         if (scoreNames[playerRank]->size() < maxNameLen) {
                            if (selectY == 1) {
                               scoreNames[playerRank]->append(1,numer[selectX]);
                            } else {
                               scoreNames[playerRank]->append(1,alph[selectX]);
                            }
                          cooldown = 8;
                         }
                         if (scoreNames[playerRank]->size() >= maxNameLen) { //Deliberate. It can change.
                            selectX = 25;
                            selectY = 1;
                         }
                       }
                       break;
                       case 6:
                       //X key action: Roll back one thing
                       if (scoreNames[playerRank]->size() > 0) {
                          scoreNames[playerRank]->erase(scoreNames[playerRank]->size()-1,1);
                          cooldown=4;
                       }
                       break;
                       case 8:
                       //Escape key action
                       break;
                   }
                }
                input = input >> 1;
            }
         } else {
           cooldown -=1;
         }
         break;
         
         case 3:
         if (rect_minx >= rect_maxx && rect_miny >= rect_maxy) {
            return 3;
         } else {
           if (rect_minx < rect_maxx) {
              rect_minx +=16;
              rect_maxx -=16;
           }
           if (rect_miny < rect_maxy) {
              rect_miny +=16;
              rect_maxy -= 16;
           }
         }
         break;
  }
  time++;
  
  return 0;
}


#include "Controller.h"
#include "Global.h"
#include "SDL/SDL.h"
#include "AudioController.h"
#include <string>
//Dummy virtual destructor function for controllers.
Controller::~Controller() {
    //Don't do anything because this is actually abstract.
}

extern AudioController* audioControl;

const int TitleController::MODE_INIT = 0;
const int TitleController::MODE_MENU = 1;
const int TitleController::MODE_START = 2;
const int TitleController::MODE_EXIT = 3;
const int TitleController::MODE_RESET = 4;
const int TitleController::NUM_OPTNS = 4;

//Creates the title controller.
//Loads screen file.
TitleController::TitleController() {

      mode=MODE_INIT;
      downPresses = 0;
      upPresses = 0;
      escPresses = 0;
      
      BGImage = load_image("Backgrounds/Title.png");
      buttons = load_image_chromakey("Sprites/startmenu.PNG",0,0xFF,0);
      runcounter = 0;
      
      //Create position arrays
      xPos = new int[4];
      yPos = new int[4];
      
      xPos[0]=80;
      xPos[1]=-128;//88;
      xPos[2]=88;//96;
      xPos[3]=96;//104;
      
      yPos[0]=240;
      yPos[1]=280;
      yPos[2]=280;//320;
      yPos[3]=320;//360;
      
      //Create offset arrays
      xOff = new int[4];
      yOff = new int[4];
      
      xOff[0]=-20;
      yOff[0]=0;
      
      xOff[1]=0;//-8;
      yOff[1]=0;//14;
      
      xOff[2]=12;
      yOff[2]=4;
      
      xOff[3]=0;
      yOff[3]=-20;
      
      sel = 0;
      spriteClip.w = 128;
      spriteClip.h = 32;
      
      //Start music
      audioControl->play_music(1);
      //audioControl->play_sound(2);
}

TitleController::~TitleController() {
    SDL_FreeSurface(BGImage);
    SDL_FreeSurface(buttons);
    delete[] xPos;
    delete[] yPos;
    delete[] xOff;
    delete[] yOff;
}

//Takes input integer and draws to a screen
int TitleController::step(int input,SDL_Surface* Screen) {
    apply_surface(0,0,BGImage,Screen);
    
    //Decompose input
    switch (mode) {
        case MODE_MENU:
        for (int i=0;i<4;i++) {
            //if (i=1) i++;
            //if (i == sel) {
            spriteClip.x = (i==sel?spriteClip.w:0);
            spriteClip.y = spriteClip.h*i;
            posRect.x=xPos[i];
            posRect.y=yPos[i];
            SDL_BlitSurface(buttons,&spriteClip,Screen,&posRect);
        }
        for (int i=0;i<10;i++) {
            if ((input & 1) > 0) {
               switch (i) {
                      case 1: //Keyboard up
                      if (upPresses == 0 ||
                      (upPresses >= 30 && (upPresses%5) == 0)) { 
                            sel=(sel + NUM_OPTNS - 1)%NUM_OPTNS;
                            if (sel == 1) sel = 0; //Dummying out code
                      }
                      upPresses += 1;
                      break;
               
                      case 3:
                      if (downPresses == 0 ||
                      (downPresses >= 30 && (downPresses%5) == 0)) { 
                            sel=(sel + 1)%NUM_OPTNS;
                            if (sel == 1) sel = 2; //Dummying out code
                      }
                      downPresses++;
                      break;
               
                      case 5:
                      case 7:
                      switch (sel) {
                             case 0:
                             mode = MODE_START;
                             break;
                             
                             case 1:
                             case 2:
                             mode = MODE_RESET;
                             break;
                             
                             case 3:
                             mode = MODE_EXIT;
                             break;
                      }
                      break;
               
                      case 6:
                      case 8:
                      if (escPresses <= 0) {
                            sel = 3;
                            downPresses = 1;
                            upPresses = 1;
                      }
                      if (escPresses < 2) {
                         escPresses = 2;
                      }
                      break;
               }
            } else {
              //Actions for if a given key is NOT pressed
              switch (i) {
                     case 1:
                     upPresses = 0;
                     break;
                     
                     case 3:
                     downPresses = 0;
                     break;
                     
                     case 6:
                     case 8:
                     if (escPresses > 0) {
                           escPresses --;
                     }
                     break;
              }
            }
            input = input >> 1;
        }
        break;
        
        case MODE_INIT:
        for (int i=0;i<4;i++) {
            int j = 30-runcounter;
            spriteClip.x = 0;
            spriteClip.y = spriteClip.h*i;
            posRect.x=xPos[i]+j*xOff[i];
            posRect.y=yPos[i]+j*yOff[i];
            SDL_BlitSurface(buttons,&spriteClip,Screen,&posRect);
        }
        if (runcounter < 30) {
           runcounter++;
        } else {
           runcounter = 0;
           mode = MODE_MENU;
        }
        break;
        
        case MODE_START:        
        case MODE_EXIT:
        case MODE_RESET:
        runcounter ++;
        for (int i=0;i<4;i++) {
            int j = runcounter;
            spriteClip.x = 0;
            spriteClip.y = spriteClip.h*i;
            posRect.x=xPos[i]+j*xOff[i];
            posRect.y=yPos[i]+j*yOff[i];
            SDL_BlitSurface(buttons,&spriteClip,Screen,&posRect);
        }
        if (runcounter > 60) {
              runcounter = 0;
              if (mode == MODE_START) {
                    return 2;
              } else if (mode == MODE_EXIT) {
                    return 1;
              } else if (mode == MODE_RESET) {
                    return 4;
              }
        }
        break;
    }

    
    
    
    return 0;
}

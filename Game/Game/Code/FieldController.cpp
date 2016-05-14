#include "FieldController.h"
#include "FieldObject.h"
#include "Player.h"
#include "Hostile.h"
#include "Global.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL.h"
#include "Hitbox.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Timeline.h"
#include "TFHandler.h"
#include <string>
#include <sstream>
#include <fstream>



extern AudioController* audioControl;
extern int globalScore;

const int FieldController::FIELD_X = 16;
const int FieldController::FIELD_Y = 16;
const int FieldController::FIELD_WIDTH = 384;
const int FieldController::FIELD_HEIGHT = 448;

const int FieldController::HUDx = 416; //=FIELD_X*2 + FIELD_WIDTH
const int FieldController::HiScY = 0;
const int FieldController::ScoreY = 32;
const int FieldController::HUDIconX = 480;
const int FieldController::LivesY = 64;
const int FieldController::BombsY = 80;

FieldController::FieldController() {
    
    //Create player
    
    playerSprite = load_image_chromakey("Sprites/Player.png",0,0xFF,0xFF);
    this->player = new Player(192.0,336.0,playerSprite);
    
    heads = new FieldObject*[3];
    for (int i=0;i<3;i++) {
        heads[i] = NULL;
    }
    
    //Load sprite sheet for shots
    shotSprite = load_image_chromakey("Sprites/pshot.png",0xFF,0xFF,0xFF);
    
    //Load sprite sheet for bullets
    bulletsImage = load_image_chromakey("Sprites/bullets1.png",0,0xFF,0xFF);
    
    //Load sprite sheet for enemies
    enemiesImage = load_image_chromakey("Sprites/enemies1.png",0,0xFF,0xFF);
    
    //Load sprite sheet for neutral
    neutralImage = load_image_chromakey("Sprites/neutral.png",0,0xFF,0xFF);

    //Load sprite sheet for frames
    frameImage = load_image_chromakey("Sprites/hs_tileset.png",0xFF,0,0);
    
    //Load HUD
    HUD = load_image_chromakey("Sprites/hud.png",0,0xFF,0xFF);
    
    //Load font
    font = new TFHandler(
         load_image_chromakey("Sprites/Typeface.png",0,0xFF,0xFF),
         16,16);
    
    //Create border rectangles
    TopBorderPos.x = 0;
    TopBorderPos.y = 0;
    TopBorderPos.w = 640;
    TopBorderPos.h = FIELD_Y;
    
    LeftBorderPos.x = 0;
    LeftBorderPos.y = FIELD_Y;
    LeftBorderPos.w = FIELD_X;
    LeftBorderPos.h = FIELD_HEIGHT;
    
    RightBorderPos.x = FIELD_X+FIELD_WIDTH;
    RightBorderPos.y = FIELD_Y;
    RightBorderPos.w = 640-FIELD_X-FIELD_WIDTH;
    RightBorderPos.h = FIELD_HEIGHT;
    
    BottomBorderPos.x = 0;
    BottomBorderPos.y = FIELD_Y+FIELD_HEIGHT;
    BottomBorderPos.w = 640;
    BottomBorderPos.h = 480-FIELD_HEIGHT-FIELD_Y;
    
    FieldPos.x = FIELD_X;
    FieldPos.y = FIELD_Y;
    FieldPos.w = FIELD_WIDTH;
    FieldPos.h = FIELD_HEIGHT;
    
    //Create actual field sufrface
    Field = SDL_CreateRGBSurface(
    SDL_SWSURFACE | SDL_SRCALPHA,      //Flags
    FIELD_WIDTH,                       //Width
    FIELD_HEIGHT,                      //Height
    32,                                //Bits per pixel
    0,0,0,0);                          //Masks: Red,Blue,Green,Alpha
    
    //Create border images
    TopBorder = load_image("Backgrounds/field_top.png");
    BottomBorder = load_image("Backgrounds/field_bottom.png");
    LeftBorder = load_image("Backgrounds/field_left.png");
    RightBorder = load_image("Backgrounds/field_right.png");
    //BGImage = load_image("Backgrounds/TempBG.png");
    
    //Build HUD
    /*HSPos.x = FIELD_X+FIELD_WIDTH+48;
    HSPos.y = 0;
    
    SPos.x = FIELD_X+FIELD_WIDTH+64;
    SPos.y=64;*/
    
    HiScValPos.x = 0;
    HiScValPos.y = HiScY;
    
    scoreValPos.x = FIELD_X+FIELD_WIDTH+48;
    scoreValPos.y = ScoreY;
    
    //Initialize HUD clipping rectangles
    HSClip.x = 0;
    HSClip.y=48;
    HSClip.w=64;
    HSClip.h=16;
    
    SClip.x=16;
    SClip.y=0;
    SClip.w=48;
    SClip.h=16;
    
    LifeLabelClip.x=16;
    LifeLabelClip.y=16;
    LifeLabelClip.w=48;
    LifeLabelClip.h=16;
    
    BombLabelClip.x=0;
    BombLabelClip.y=32;
    BombLabelClip.w=64;
    BombLabelClip.h=16;
    
    LifeIconClip.x=0;
    LifeIconClip.y=0;
    LifeIconClip.w=16;
    LifeIconClip.h=16;
    
    BombIconClip.x=0;
    BombIconClip.y=16;
    BombIconClip.w=16;
    BombIconClip.h=16;
    
    //Link FieldObjects
    FieldObject::fieldControl = this;
    
    //Initialize timeline with first stage
    timeline = new Timeline(0);
    
    //Load high score    
    std::ifstream* loadStream = new std::ifstream("Hiscores.dat");

        std::string* debuf = new std::string();
        (*loadStream) >> (*debuf);
        delete debuf;
        (*loadStream) >> hiScore;  
        
	delete loadStream;
    
    //Set player-relevant and misc. variables
    score = 0;
    px = 0.0;
    py = 0.0;
    time = 0;
    lives = 2;
    defaultBombs = 3;
    bombs = defaultBombs;
    respawnTime = 0;
    endGame = false;
    timePaused = false;
    
    //Start music
    audioControl->play_music(2);

};

FieldController::~FieldController() {
    
    //Delete player
    delete this->player;
    
    //Free background image
    //SDL_FreeSurface(BGImage);

    //Free sprite images
    SDL_FreeSurface(shotSprite);
    SDL_FreeSurface(bulletsImage);
    SDL_FreeSurface(enemiesImage);
    SDL_FreeSurface(neutralImage);
    SDL_FreeSurface(playerSprite);
    SDL_FreeSurface(frameImage);    

    //Free font
    delete font;
    
    //Purge rectangles
    
    //Delete all objects.
    for (int i=0;i<3;i++) {
        for (FieldObject* headObj = heads[i];headObj != NULL;headObj = heads[i]) {
            delete headObj;
        }
    }
    delete[] heads;
    
    //Clear borders
    SDL_FreeSurface(BottomBorder);
    SDL_FreeSurface(TopBorder);
    SDL_FreeSurface(LeftBorder);
    SDL_FreeSurface(RightBorder);
    SDL_FreeSurface(Field);
    
    //Free labels/surfaces
    SDL_FreeSurface(HUD);
    
    //Delete timeline
    delete timeline;
};


int FieldController::step(int input,SDL_Surface* Screen) {
    
    //Detect escape key
    if (input >> 8 != 0) {
       return 3;
    }
    
    //Resolve end of game
    if (endGame) {
       globalScore = score;
       return 4;
       endGame = false;
    }
    
    //Tick timeline
    if (!timePaused) { //Give option of stopped time.
       timeline -> tick(time);
       time++;
    }
    //Step sequence
    for (int i=0;i<3;i++) {
        FieldObject* j = heads[i];
        while (j != NULL) {
            FieldObject* temp = j;
            j=j->next;
            temp->step();
        }
    }

    //Subset of step: Player-related stuff.
    if (this->player != NULL) {
       bombing = player->bombing;
       //Take commands, and check for collisions
       this->px = player->getx();
       this->py = player->gety();
       this -> player -> takeCommands(input);
        //Check for collisions
        Hitbox phbox = player->hitbox();
        //Set 0 is hostile head
        FieldObject* i = heads[0];
        while (i != NULL) {
            Hitbox hhbox = i->hitbox();
            //Check for collisions with player
            if (phbox.overlaps(&hhbox)) {
               FieldObject* temp = i;
               temp -> collision(0);
               if (player->collision(0)) {
                  player = NULL;
               }
               break;
            } else {
              //Check for collisions with player shots
              if (i->collidable()||player->bombing) { //Check collidable or bombing
                FieldObject* j = heads[1];
                while (j != NULL) {
                      FieldObject* temp = j;
                      j=j->next;
                      Hitbox shbox = temp->hitbox();
                      if (shbox.overlaps(&hhbox)) {
                         temp->collision(1);
                         if (i->collision(1)) break;;
                      }
                }
              }
              i=i->next;
            }   
        }
        //Test collisions with player's second hitbox
        if (player != NULL) {
           phbox = player->ihitbox();
           i = heads[2];
           while (i != NULL) {
              FieldObject* temp = i;
              i=i->next;
              if (temp->collidable()) {
                 Hitbox ihbox = temp->hitbox();
                 if (phbox.overlaps(&ihbox)) {
                    player->collision(1);
                    temp->collision(1);
                    break;
                 }
              }
           }
        }
        
    } else {
      //Things to do if player is dead.
      if (respawnTime == 0) {//If the player just now died
         respawnTime = 60;
      } else {
        respawnTime -= 1;
        if (respawnTime == 0) {
           if (lives > 0) {
              player = new Player(192.0,336.0,playerSprite);
              lives -=1;
              bombs = defaultBombs;
           } else {
             //Game Over
             globalScore = score;
             return 4;
           }
        }
      }
    }         
    
    //Draw sequence
    
    //Draw background
    
    DrawFXBG(Field,time);
    
    //apply_surface(0,0,BGImage,Field);
    
    //Draw player
    if (this->player != NULL) {
       this -> player -> draw(Field);
    }
    //Draw objects
    for (int i=0;i<3;i++) {
        for (FieldObject* j = heads[i];j != NULL;j=j->next) {
            j->draw(Field);
        }
    }
    
    //Draw screen sequence
    SDL_BlitSurface(TopBorder,NULL,Screen,&TopBorderPos);
    SDL_BlitSurface(BottomBorder,NULL,Screen,&BottomBorderPos);
    SDL_BlitSurface(LeftBorder,NULL,Screen,&LeftBorderPos);
    SDL_BlitSurface(RightBorder,NULL,Screen,&RightBorderPos);
    SDL_BlitSurface(Field,NULL,Screen,&FieldPos);
    //this -> player -> draw(Screen);
    
    //Draw HUD (except score)
    
    posRect.x=HUDx;
    
    //Draw HiScore label
    posRect.y=HiScY;
    SDL_BlitSurface(HUD,&HSClip,Screen,&posRect);
    
    
    //Draw Score label
    posRect.y=ScoreY;
    SDL_BlitSurface(HUD,&SClip,Screen,&posRect);
    
    //Draw Lives label and lives
    posRect.y=LivesY;
    SDL_BlitSurface(HUD,&LifeLabelClip,Screen,&posRect);
    posRect.x=HUDIconX;
    for (int i=0;i<lives;i++) {
        SDL_BlitSurface(HUD,&LifeIconClip,Screen,&posRect);
        posRect.x += 16;
    }
    
    posRect.x=HUDx;
    
    //Draw Bombs label and bombs
    posRect.y=BombsY;
    SDL_BlitSurface(HUD,&BombLabelClip,Screen,&posRect);
    posRect.x = HUDIconX;
    for (int i=0;i<bombs;i++) {
        SDL_BlitSurface(HUD,&BombIconClip,Screen,&posRect);
        posRect.x += 16;
    }
    
    //Draw score
    font->display_int(Screen,640,ScoreY,score);
    
    //Make sure hiscore matches score
    if (score > hiScore) hiScore = score;
    
    //Draw hiscore
    font->display_int(Screen,640,HiScY,hiScore);
    
}

/**
 * Sets one of the FieldController's three heads.
 * HeadID 0 is enemy, 1 is shot, 2 is item.
 */
void FieldController::setHead(int headID,FieldObject* newHead) {
     heads[headID] = newHead;
}

/**
 * Retrieves one of the FieldControllers three heads.
 * As above.
 */
FieldObject* FieldController::getHead(int headID) {
      return heads[headID];
}

void FieldController::accScore(int amount) {
     this->score += amount;
}

void FieldController::pauseTime() {
     timePaused = true;
}

void FieldController::unPauseTime() {
     timePaused = false;
}

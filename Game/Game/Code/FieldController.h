#ifndef FCONTROLLER_H
#define FCONTROLLER_H

#include "Controller.h"
#include "SDL/SDL.h"
#include <sstream>

class Timeline;
class FieldObject;
class Player;
class TFHandler;


class FieldController: public Controller {
      protected:
                FieldObject** heads;
                FieldObject* hostileHead;
                Player* player;
                FieldObject* shotsHead;
                FieldObject* itemsHead;
                
                //Labels.
                
                SDL_Rect TopBorderPos;
                SDL_Rect LeftBorderPos;
                SDL_Rect RightBorderPos;
                SDL_Rect BottomBorderPos;
                SDL_Rect FieldPos;
                
                SDL_Surface* TopBorder;
                SDL_Surface* LeftBorder;
                SDL_Surface* RightBorder;
                SDL_Surface* BottomBorder;
                SDL_Surface* Field;
                
                static const int HUDx;
                static const int HiScY;
                static const int ScoreY;
                static const int HUDIconX;
                static const int LivesY;
                static const int BombsY;       
                
                SDL_Rect HSClip;
                SDL_Rect SClip;
                SDL_Rect LifeLabelClip;
                SDL_Rect BombLabelClip;
                SDL_Rect LifeIconClip;
                SDL_Rect BombIconClip;
                SDL_Rect posRect;
                
                SDL_Surface* HUD;
                
                SDL_Rect HiScValPos;
                SDL_Rect scoreValPos;
                
                SDL_Surface* HiScDisp;
                
                int* xPos;
                int* yPos;
                
                Timeline* timeline;
                int time;
                
                int score;
                int hiScore;
                
                int respawnTime;
                int defaultBombs;   
                
                bool timePaused;             
                
                
      public:
             static const int FIELD_X;
             static const int FIELD_Y;
             static const int FIELD_WIDTH;
             static const int FIELD_HEIGHT;
             
             float px;
             float py;
             
             SDL_Surface* playerSprite;
             SDL_Surface* shotSprite;
             SDL_Surface* bulletsImage;
             SDL_Surface* enemiesImage;
             SDL_Surface* neutralImage;
             SDL_Surface* frameImage;
             TFHandler* font;
             
             FieldController();
             ~FieldController();
             int step(int input,SDL_Surface* Screen);
             
             void setHead(int headID,FieldObject* newHead);
             FieldObject* getHead(int headID);
             void accScore(int amount);
             
             int lives;
             int bombs;
             
             void pauseTime();
             void unPauseTime();
             
             bool bombing;
             
             bool endGame;
};

//static FieldController FieldObject::fieldControl;

#endif

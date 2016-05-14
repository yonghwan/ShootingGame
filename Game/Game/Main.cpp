#include "SDL/SDL.h"
#include <string>
#include "Code/AudioController.h"
#include "Code/Global.h"
#include "Code/FieldObject.h"
#include "Code/Player.h"
#include "Code/Timer.h"
#include "Code/Controller.h"
#include "Code/FieldController.h"
#include "Code/HiScoreController.h"
#include "SDL/SDL_image.h"



const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;
const int FRAMERATE = 60;

extern AudioController* audioControl;
extern int globalScore;

Timer fps;

//The surfaces that will be used
SDL_Surface* screen = NULL;
Controller* handler = NULL;

SDL_Event event;


int main( int argc, char* args[] )
{
    //Make sure the program waits for a quit
    bool quit = false;
    //Initialize
    screen = init(
    SCREEN_WIDTH,
    SCREEN_HEIGHT,
    SCREEN_BPP,
    SDL_SWSURFACE 
    | SDL_DOUBLEBUF 
    | SDL_FULLSCREEN
    );
    
    if (screen == NULL) {
        return 1;
    }

    //Update the screen
    if( SDL_Flip( screen ) == -1 )
    {
        //return 1;
    }
    
    //Start audio controller
    audioControl = new AudioController();
    
    //Start handler
    handler = new TitleController();
    
    //Movement handling stuff
    Uint32 movHandle = 0; //Movement integer
    Uint8* keystates;
    
    
    
    //While the user hasn't quit
    while( quit == false ) {
        
        fps.start();   
        
        //While there's an event to handle
        while(SDL_PollEvent(&event)) {
            if( event.type == SDL_QUIT ) {
                //Quit the program
                quit = true;
            }
        }
        //Get keyboard input, this could probably be function-ized
        movHandle = 0;
        keystates = SDL_GetKeyState(NULL);
        if (keystates[SDLK_LSHIFT]) {
           movHandle = movHandle | 1;
        }
        if (keystates[SDLK_UP]) {
           movHandle = movHandle | 2;
        }
        if (keystates[SDLK_DOWN]) {
           movHandle = movHandle | 8;
        }
        if (keystates[SDLK_LEFT]) {
           movHandle = movHandle | 4;
        }
        if (keystates[SDLK_RIGHT]) {
           movHandle = movHandle | 16;
        }
        //Shot/select key
        if (keystates[SDLK_z]) {
           movHandle = movHandle | 32;
        }
        //Bomb key
        if (keystates[SDLK_x]) {
           movHandle = movHandle | 64;
        }
        //Enter key
        if (keystates[SDLK_RETURN]) {
           movHandle = movHandle | 128;
        }
        if (keystates[SDLK_ESCAPE]) {
           movHandle = movHandle | 256;
        }
        
        //apply_surface( 0, 0, image, screen );
        
        //player -> takeCommands(movHandle);
        //player -> draw(screen);
        movHandle = handler->step(movHandle,screen);
        
        //Global events for handler operations.
        switch (movHandle) {
               case 1:
               delete handler;
               quit = true;
               break;
               
               //Case 2: Switch to gameplay mode
               case 2:
               delete handler;
               handler = new FieldController();
               break;
               
               //Case 3: Switch to menu mode
               case 3:
               delete handler;
               handler = new TitleController();
               break;
               
               //Case 4: Switch to High Scores mode
               case 4:
               delete handler;
               handler = new HiScoreController(globalScore);
               globalScore = 0;
               break;
                    
        }
        
        SDL_Flip(screen);
        
        if (fps.get_ticks() < 1000 / FRAMERATE) {
           SDL_Delay(1000/FRAMERATE - fps.get_ticks());
        }
    }
    
    //Cleanup
    delete[] keystates; //Eliminate the array
    delete audioControl;
    
    //Quit SDL
    cleanup();
    
    //Debug
    /*
    std::cout << "FieldObject constructors: " << FieldObject::constructors << std::endl;
    std::cout << "FieldObject destructors: " << FieldObject::destructors << std::endl;
    */
    
    return 0;
}

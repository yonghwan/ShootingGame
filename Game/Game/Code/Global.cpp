/**
 * External (global) functions go here.
 * Things like loading and blitting images that will be used a lot.
 * Put function defs in Global.h. Put implementation here.
 */
 
#include "SDL/SDL.h"
#include <string>
#include "SDL/SDL_image.h"
#include "SDL/SDL_mixer.h"

#include <cstdlib>
#include <cmath>

class AudioController;
//Extern. Global variables.
AudioController* audioControl = NULL;
int globalScore = 0;


/**
 * Initializes the screen with given width, height, and bits per pixel
 */
SDL_Surface* init(int screenWidth,int screenHeight,int screenBPP,int flags) {
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 ) {
        return NULL;
    }

    //Set up the screen
    SDL_Surface* screen = SDL_SetVideoMode(screenWidth,screenHeight,screenBPP,flags);

    //If there was an error in setting up the screen
    if (screen == NULL) {
        return NULL;
    }

    //Set the window caption
    SDL_WM_SetCaption( "Game", NULL );

    //If everything initialized fine
    return screen;
}

SDL_Surface* load_image(std::string filename) {


    //Temporary storage for the image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized image that will be used
    SDL_Surface* optimizedImage = NULL;


    //Load the image
    loadedImage = IMG_Load(filename.c_str());
    

    //If nothing went wrong in loading the image
    if( loadedImage != NULL ) {
        //Create an optimized image
        optimizedImage = SDL_DisplayFormat(loadedImage);
        //Free the old image
        SDL_FreeSurface(loadedImage);
    }

    //Return the optimized image
    return optimizedImage;
}

SDL_Surface* load_image_chromakey(std::string filename,int red,int blue,int green) {
    SDL_Surface* loadImage = load_image(filename);
    
    if (loadImage != NULL) {
        //Correct color
        Uint32 colorKey = SDL_MapRGB(loadImage -> format, red, blue, green);
        SDL_SetColorKey(loadImage,SDL_SRCCOLORKEY,colorKey);
    }
    return loadImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination ) {
    //Make a temporary rectangle to hold the offsets
    SDL_Rect offset;

    //Give the offsets to the rectangle
    offset.x = x;
    offset.y = y;

    //Blit the surface
    SDL_BlitSurface( source, NULL, destination, &offset );
}

void cleanup() {
     SDL_Quit();
}

float getDirection(float xfrom,float yfrom,float xto,float yto) {
      return (atan2(yto-yfrom,xto-xfrom)*180/M_PI);
};

Uint32 get_pixel32(SDL_Surface* surface,int x,int y) {
    //Convert the pixels to 32 bit
    Uint32* pixels = (Uint32*)surface->pixels;

    //Get the requested pixel
    return pixels[(y*surface->w)+x];
}

void put_pixel32(SDL_Surface* surface,int x,int y,Uint32 pixel) {
    //Convert the pixels to 32 bit
    Uint32* pixels = (Uint32 *)surface->pixels;

    //Set the pixel
    pixels[ ( y * surface->w ) + x ] = pixel;
}

void DrawFXBG(SDL_Surface* screen,int h) {
     if(SDL_MUSTLOCK(screen)) {
         if(SDL_LockSurface(screen) < 0) return;
     }
     for (int x=0;x<screen->w;x++) {
         int startl = 0;
         int endl = 0;
         if (rand() % 100 <= 5) {
            startl = ((rand() % 400) + 30*h) % (screen->h-20);
            endl = startl + 20;
         }
         for (int y=0;y<screen->h;y++) {
             
             Uint32 pixel = 0;//x+y;
             if (y > startl && y <= endl) {
                   pixel = 0xFFFFFF;
             }
             /*if (y>5) {
                pixel = get_pixel32(screen,x,y-5);
             } else {
               pixel = 0;
             }*/
             //pixel = pixel & 0xAAAAAAAA;
             put_pixel32(screen,x,y,pixel);
         }
     }
}




//Nathan's code.
/*
void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b) {
    Uint32 *pixmem32;
    Uint32 colour;
    
    colour = SDL_MapRGB( screen->format, r, g, b );
    
    pixmem32 = (Uint32*) screen->pixels  + y + x;
    *pixmem32 = colour;
}

void DrawFXBG(SDL_Surface* screen, int h)
{
    int x, y, ytimesw;
    
    if(SDL_MUSTLOCK(screen))
    {
        if(SDL_LockSurface(screen) < 0) return;
    }
    
    for(y = 0; y < screen->h; y++ )
    {
        ytimesw = y*screen->pitch/4; //32 is the bits-per-pixel.
        for( x = 0; x < screen->w; x++ )
        {
            // The action!
            setpixel(screen, x, ytimesw
            ,(x*y)/400+78*y+h
            ,(y*y)/256+x+h
            ,h
            );

        }
    }
    
    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
    
    SDL_Flip(screen);
}
*/











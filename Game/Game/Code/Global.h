#ifndef GLOBAL_H
#define GLOBAL_H

#include "SDL/SDL.h"
#include "AudioController.h"
#include <string>

//Initializes the screen with input width, height, and bits per pixel values.
//Also initializes used SDL subsystems.
SDL_Surface* init(int screenWidth,int screenHeight,int screenBPP,int flags);

//Loads an image from input filename.
SDL_Surface* load_image(std::string filename);

//Loads an image from input filename, with a chroma key.
//Removes the color corresponding to the three integers.
SDL_Surface* load_image_chromakey(std::string filename,int red,int blue,int green);

//Blits an SDL surface to another SDL surface with the given offsets.
void apply_surface(int x,int y,SDL_Surface* source,SDL_Surface* destination);

//Deinitializes various things.
void cleanup();

//Direction from one point to another point.
float getDirection(float xfrom,float yfrom,float xto,float yto);

//Get pixel
Uint32 get_pixel32(SDL_Surface* surface,int x,int y);

//Set pixel
void put_pixel32(SDL_Surface* surface,int x,int y,Uint32 pixel);

//Code to draw a space-type background
void DrawFXBG(SDL_Surface* screen, int h);




#endif

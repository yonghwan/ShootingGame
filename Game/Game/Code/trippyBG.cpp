void setpixel(SDL_Surface *screen, int x, int y, Uint8 r, Uint8 g, Uint8 b)
{
    Uint32 *pixmem32;
    Uint32 colour;
    
    colour = SDL_MapRGB( screen->format, r, g, b );
    
    pixmem32 = (Uint32*) screen->pixels  + y + x;
    *pixmem32 = colour;
}
void DrawTrippyBG(SDL_Surface* screen, int h)
{
    int x, y, ytimesw;
    
    if(SDL_MUSTLOCK(screen))
    {
        if(SDL_LockSurface(screen) < 0) return;
    }
    
    for(y = 0; y < screen->h; y++ )
    {
        ytimesw = y*screen->pitch/BPP;
        for( x = 0; x < screen->w; x++ )
        {
            // The action!
            setpixel(screen, x, ytimesw, (x*y)/400+78*y+h, (y*y)/256+x+h, h);

        }
    }
    
    if(SDL_MUSTLOCK(screen)) SDL_UnlockSurface(screen);
    
    SDL_Flip(screen);
}
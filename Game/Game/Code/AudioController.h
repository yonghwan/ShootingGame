#ifndef AUDIOCONT_H
#define AUDIOCONT_H
#include "SDL/SDL_Mixer.h"

/**
 * Audio controller
 * 
 *
 */
 
class AudioController {
      public:
             AudioController();
             ~AudioController();
             void play_sound(int sound);
             //void play_sound_rapid(int sound);
             void play_music(int track,int loopNum = -1);
             void stop_music();
             static const int NUM_TRACKS;
             static const int NUM_SFX;
             static const int SOUND_SHOT;
             static const int SOUND_PSHOT;
             static const int SOUND_BOMB;
             static const int SOUND_BOSSDIE;
             static const int SOUND_RAPID;
             int rChannel;
             
      //protected:
                
             Mix_Music** music;
             Mix_Chunk** SFX;
};

#endif

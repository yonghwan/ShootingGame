//Code for audio control

#include "AudioController.h"
#include "SDL/SDL.h"
#include "SDL/SDL_Mixer.h"

const int AudioController::NUM_SFX = 5;
const int AudioController::NUM_TRACKS = 4;
const int AudioController::SOUND_PSHOT = 0;
const int AudioController::SOUND_SHOT = 1;
const int AudioController::SOUND_BOMB = 2;
const int AudioController::SOUND_BOSSDIE = 3;
const int AudioController::SOUND_RAPID = 4;

AudioController::AudioController() {
    //Initialize SDL_Mixer
    if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
       //Crash and burn somehow...
    } else {
        //Load all the audio files.
        //Sounds have filenames like "SFX/Sound1.wav"
        //Musics have filenames like "Music/Track1.mid"        
        music = new Mix_Music*[NUM_TRACKS];
        music[0]=Mix_LoadMUS("Music/track1.mid");
        music[1] = Mix_LoadMUS("Music/track2.mid");
        music[2] = Mix_LoadMUS("Music/track3a.mid");
        music[3] = Mix_LoadMUS("Music/track3b.mid");
        SFX = new Mix_Chunk*[NUM_SFX];
        SFX[0] = Mix_LoadWAV("SFX/pshot.wav");
        SFX[1] = Mix_LoadWAV("SFX/shoot.wav");
        SFX[2] = Mix_LoadWAV("SFX/bomb.wav");
        SFX[3] = Mix_LoadWAV("SFX/bossdie.wav");
        SFX[4] = Mix_LoadWAV("SFX/rapidshoot.wav");
    }
    rChannel = 0;
                    
}

AudioController::~AudioController() {
    //Unload all the audio files. This is called on exit.
    stop_music();
    for (int i=0;i<NUM_SFX;i++) {
        Mix_FreeChunk(SFX[i]);
    }
    for (int i=0;i<NUM_TRACKS;i++) {
        Mix_FreeMusic(music[i]);
    }
    delete[] SFX;
    delete[] music;
    Mix_CloseAudio();

}

void AudioController::play_sound(int sound) {
    //Play a sound.
    //Specify which sound it is in the documentation.
    //Multiple sounds should be playable at once
    if (sound < 0) sound=0;
    if (sound >= NUM_SFX) sound=NUM_SFX-1;
    Mix_PlayChannel(-1,SFX[sound],0);
}

void AudioController::play_music(int track,int loopnum) {
     stop_music(); //Stop music first.
     if (track<=0) track=1;
     if (track>NUM_TRACKS) track=NUM_TRACKS;
     if( Mix_PlayingMusic() == 0 ) {
         if (Mix_PlayMusic( music[track-1], loopnum ) == -1) {
            //Fail somehow
         };
     }
     
    //Play one of the MIDI tracks.
    //Specify which one in the documentation.
    //Only one music should be playable at a time.
    //Also it should automatically loop to a starting point.
    //All the songs that need to loop to a specific point will have
    // a midi cue at that point named "loopStart" when they're done.
}

void AudioController::stop_music() {
     Mix_HaltMusic();
     
}

/***************************************************************
 *
 *
 *    ███████╗ ██████╗ ██╗   ██╗███╗   ██╗██████╗ 
 *    ██╔════╝██╔═══██╗██║   ██║████╗  ██║██╔══██╗
 *    ███████╗██║   ██║██║   ██║██╔██╗ ██║██║  ██║
 *    ╚════██║██║   ██║██║   ██║██║╚██╗██║██║  ██║
 *    ███████║╚██████╔╝╚██████╔╝██║ ╚████║██████╔╝
 *    ╚══════╝ ╚═════╝  ╚═════╝ ╚═╝  ╚═══╝╚═════╝ 
 *
 *     Sound System Header
 *     Manages game sound effects and audio playback
 *
 **************************************************************/

#ifndef SOUND_H
#define SOUND_H

#include "../common.h"

// Maximum number of loaded sounds
#define MAX_SOUNDS 50

// Sound entry structure
typedef struct {
    char name[128];
    Sound sound;
    bool loaded;
} SoundEntry;

// Sound manager structure
typedef struct {
    SoundEntry sounds[MAX_SOUNDS];
    int soundCount;
    bool initialized;
} SoundManager;

// Function declarations
void initSoundSystem(void);
void clearSoundSystem(void);
Sound* loadSoundEffect(const char* filename, const char* name);
Sound* getSoundByName(const char* name);
void playSoundEffect(const char* name);
void unloadSoundEffect(const char* name);
void setSoundVolume(float volume);

#endif // SOUND_H

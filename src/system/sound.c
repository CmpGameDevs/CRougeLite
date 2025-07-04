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
 *     Sound System Implementation
 *     Manages game sound effects and audio playback
 *
 **************************************************************/

#include "sound.h"

// Global sound manager
static SoundManager soundManager = {0};

void initSoundSystem(void) {
    if (soundManager.initialized) return;
    
    // Initialize all sound entries
    for (int i = 0; i < MAX_SOUNDS; i++) {
        soundManager.sounds[i].loaded = false;
        memset(soundManager.sounds[i].name, 0, sizeof(soundManager.sounds[i].name));
    }
    
    soundManager.soundCount = 0;
    soundManager.initialized = true;
    
    printf("Sound system initialized\n");
}

void clearSoundSystem(void) {
    if (!soundManager.initialized) return;
    
    // Unload all sounds
    for (int i = 0; i < soundManager.soundCount; i++) {
        if (soundManager.sounds[i].loaded) {
            UnloadSound(soundManager.sounds[i].sound);
            soundManager.sounds[i].loaded = false;
        }
    }
    
    soundManager.soundCount = 0;
    soundManager.initialized = false;
    
    printf("Sound system cleared\n");
}

Sound* loadSoundEffect(const char* filename, const char* name) {
    if (!soundManager.initialized) {
        initSoundSystem();
    }
    
    if (soundManager.soundCount >= MAX_SOUNDS) {
        printf("Error: Maximum number of sounds reached (%d)\n", MAX_SOUNDS);
        return NULL;
    }
    
    // Check if sound already exists
    Sound* existingSound = getSoundByName(name);
    if (existingSound != NULL) {
        return existingSound;
    }
    
    // Load the sound
    Sound sound = LoadSound(filename);
    if (!IsAudioDeviceReady()) {
        printf("Error: Audio device not ready\n");
        return NULL;
    }
    
    // Store in manager
    int index = soundManager.soundCount;
    soundManager.sounds[index].sound = sound;
    strncpy(soundManager.sounds[index].name, name, sizeof(soundManager.sounds[index].name) - 1);
    soundManager.sounds[index].loaded = true;
    soundManager.soundCount++;
    
    printf("Loaded sound: %s from %s\n", name, filename);
    return &soundManager.sounds[index].sound;
}

Sound* getSoundByName(const char* name) {
    if (!soundManager.initialized) return NULL;
    
    for (int i = 0; i < soundManager.soundCount; i++) {
        if (soundManager.sounds[i].loaded && strcmp(soundManager.sounds[i].name, name) == 0) {
            return &soundManager.sounds[i].sound;
        }
    }
    
    return NULL;
}

void playSoundEffect(const char* name) {
    Sound* sound = getSoundByName(name);
    if (sound != NULL) {
        float pitch = 1.0f + (GetRandomValue(-10, 10) / 100.0f); // Random pitch between 0.9 and 1.1
        SetSoundPitch(*sound, pitch);
        PlaySound(*sound);
    } else {
        printf("Warning: Sound '%s' not found\n", name);
    }
}

void unloadSoundEffect(const char* name) {
    if (!soundManager.initialized) return;
    
    for (int i = 0; i < soundManager.soundCount; i++) {
        if (soundManager.sounds[i].loaded && strcmp(soundManager.sounds[i].name, name) == 0) {
            UnloadSound(soundManager.sounds[i].sound);
            soundManager.sounds[i].loaded = false;
            
            // Shift remaining sounds down
            for (int j = i; j < soundManager.soundCount - 1; j++) {
                soundManager.sounds[j] = soundManager.sounds[j + 1];
            }
            soundManager.soundCount--;
            
            printf("Unloaded sound: %s\n", name);
            return;
        }
    }
    
    printf("Warning: Sound '%s' not found for unloading\n", name);
}

void setSoundVolume(float volume) {
    // Clamp volume between 0.0 and 1.0
    if (volume < 0.0f) volume = 0.0f;
    if (volume > 1.0f) volume = 1.0f;
    
    for (int i = 0; i < soundManager.soundCount; i++) {
        if (soundManager.sounds[i].loaded) {
            SetSoundVolume(soundManager.sounds[i].sound, volume);
        }
    }
}

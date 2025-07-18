#include "Music.h"
#include <iostream>
#define INCLUDE_SDL_MIXER

Music::Music() : music(nullptr) {}

Music::Music(const std::string file) : music(nullptr) {
    Open(file);
}

Music::~Music() {
    if (music != nullptr) {
        Mix_FreeMusic(music);
    }
}

void Music::Open(const std::string file) {
    if (music != nullptr) {
        Mix_FreeMusic(music);
    }

    music = Mix_LoadMUS(file.c_str());

    if (!music) {
        std::cerr << "Erro ao carregar música: " << Mix_GetError() << std::endl;
    }
}

void Music::SetVolume(int volume) {
    // Garante que o volume está no intervalo permitido
    if (volume < 0) volume = 0;
    if (volume > MIX_MAX_VOLUME) volume = MIX_MAX_VOLUME;
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        std::cerr << "Erro ao inicializar SDL_mixer: " << Mix_GetError() << std::endl;
    }

    Mix_VolumeMusic(volume);

    
}

void Music::Play(int times) {
    //std::cout<<"chamado\n";
    if (music) {
        //std::cout<<"tocando\n";
        Mix_PlayMusic(music, times);
    }
}

void Music::Stop(int msToStop) {
    Mix_FadeOutMusic(msToStop);
}

bool Music::IsOpen() {
    return music != nullptr;
}

#ifndef SOUND_H
#define SOUND_H
#define INCLUDE_SDL_MIXER

#include <string>
#include "SDL_include.h"


class Sound {
public:
    Sound();                               // Construtor padrão
    Sound(std::string file);               // Construtor com carregamento
    ~Sound();                              // Destrutor

    void SetVolume(int volume); // volume de 0 a 128
    void Play(int times = 1);              // Toca o som (1 vez por padrão)
    void Stop();                           // Para a reprodução
    void Open(std::string file);           // Carrega o som
    bool IsOpen();                         // Retorna se o som está carregado

private:
    Mix_Chunk* chunk;
    int channel;
};

#endif

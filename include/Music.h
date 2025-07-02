#ifndef MUSIC_H
#define MUSIC_H
#define INCLUDE_SDL_MIXER 

#include <string>
#include "SDL_include.h"

class Music {
private:
    Mix_Music* music;

public:
    Music();                            // Construtor padrão
    Music(const std::string file);     // Construtor com arquivo
    ~Music();                           // Destrutor
    
    void SetVolume(int volume); // volume de 0 a 128
    void Play(int times = -1);          // Reproduz a música (loop infinito por padrão)
    void Stop(int msToStop = 1500);     // Para a música com fade-out
    void Open(const std::string file); // Carrega a música do arquivo
    bool IsOpen() ;                // Retorna true se uma música estiver carregada
};

#endif // MUSIC_H

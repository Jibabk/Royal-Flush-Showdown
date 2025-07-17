#ifndef SPRITE_H
#define SPRITE_H
#define INCLUDE_SDL_IMAGE

#include <string>
#include "SDL_include.h"
#include "Vec2.h"


class Sprite {
public:
    Sprite();  // Construtor padrão
    Sprite(const std::string file, int frameCountW = 1, int frameCountH = 1); // Construtor com suporte a animação
    ~Sprite(); // Destrutor

    void Open(const std::string file);                    // Carrega uma imagem
    void SetClip(int x, int y, int w, int h);             // Define a área da textura a ser desenhada
    void SetFrame(int frame);                             // Define o frame atual
    void SetFrameCount(int frameCountW, int frameCountH); // Define número de frames horizontais e verticais

    void Render(int x, int y,float angle);                            // Renderiza com tamanho original
    void Render(int x, int y, int w, int h,float angle);              // Renderiza redimensionado

    int GetWidth();                                       // Largura de um frame
    int GetHeight();                                      // Altura de um frame
    bool IsOpen();         
    bool cameraFollower;                             // Verifica se a textura foi carregada
    void SetScale(float scaleX, float scaleY);
    Vec2 GetScale();
    void SetFlip(bool flipX, bool flipY);

private:
    SDL_Texture* texture;
    int width, height;        // Dimensões totais da imagem
    SDL_Rect clipRect;

    int frameCountW;          // Frames por linha
    int frameCountH;          // Frames por coluna
    SDL_RendererFlip flip; // Flip da textura
    Vec2 scale; // Escala da textura
};

#endif // SPRITE_H

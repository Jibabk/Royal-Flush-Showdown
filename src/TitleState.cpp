#include "TitleState.h"
#include "Game.h"
#include "InputManager.h"
#include "StageState.h"
#include "SpriteRenderer.h"
#include "Camera.h"
#include "Music.h"
#include "Timer.h"

TitleState::TitleState() : backgroundMusic("Recursos/audio/menu.wav"){
    Camera::pos = {0, 0};
    Camera::speed = {0, 0};

    // Fundo com imagem completa do menu
    GameObject* bg = new GameObject();
    SpriteRenderer* bgSprite = new SpriteRenderer(*bg, "Recursos/img/menu/Capa_2_c_borda.png");
    bgSprite->SetCameraFollower(true);

    // Ajusta escala da imagem para o tamanho da tela
    float screenW = Game::GetInstance().GetWidth();
    float screenH = Game::GetInstance().GetHeight();
    float imageW = bgSprite->GetWidth();
    float imageH = bgSprite->GetHeight();

    float scaleX = screenW / imageW;
    float scaleY = screenH / imageH;

    bgSprite->SetScale(scaleX, scaleY);
    bg->AddComponent(bgSprite);
    AddObject(bg);

    // Define áreas clicáveis com base na imagem:
    jogarArea = Rect(150, 360, 200, 75); // ajuste fino conforme a imagem
    sairArea = Rect(150, 465, 200, 75);  // ajuste fino conforme a imagem
        
    backgroundMusic.SetVolume(64); // Define o volume da música para 100%, volume de 0 a 128
    backgroundMusic.Play(); // Toca a música em loop

}

TitleState::~TitleState() {
    objectArray.clear();
}

void TitleState::Start() {
    LoadAssets();
    StartArray();
    started = true;
}

void TitleState::Pause() {}
void TitleState::Resume() {}

void TitleState::LoadAssets() {
    // Ex: backgroundMusic.Open("Recursos/audio/menuMusic.ogg");
    // backgroundMusic.Play();
}

void TitleState::Update(float dt) {
    InputManager& input = InputManager::GetInstance();
    Vec2 mouse(input.GetMouseX(), input.GetMouseY());

    if (input.MousePress(LEFT_MOUSE_BUTTON)) {
        if (jogarArea.Contains(mouse)) {
            Game::GetInstance().Push(new StageState());
        } else if (sairArea.Contains(mouse)) {
            quitRequested = true;
        }
    }

    if (input.QuitRequested() || input.KeyPress(ESCAPE_KEY)) {
        quitRequested = true;
    }

    UpdateArray(dt);
}

void TitleState::Render() {
    RenderArray();

}

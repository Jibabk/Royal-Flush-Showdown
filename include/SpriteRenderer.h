#ifndef SPRITERENDERER_H
#define SPRITERENDERER_H

#include "Component.h"
#include "Sprite.h"
#include <string>

class SpriteRenderer : public Component {
public:
    SpriteRenderer(GameObject& associated);
    SpriteRenderer(GameObject& associated, const std::string& file, int frameCountW = 1, int frameCountH = 1);

    void Open(const std::string& file);
    void SetFrameCount(int frameCountW, int frameCountH);

    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) const override;
    void SetCameraFollower(bool follow);

    int GetWidth();
    int GetHeight();
    void SetScale(float scaleX, float scaleY);
    void SetFrame(int frame, SDL_RendererFlip flip);

private:
    Sprite sprite;
};

#endif // SPRITERENDERER_H

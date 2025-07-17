#ifndef COLLIDER_H
#define COLLIDER_H


#include <memory>
#include "Component.h"
#include "Timer.h"
#include "Sound.h"
#include "Vec2.h"
#include "Rect.h"

class Collider : public Component {
public:
    Collider(GameObject& associated, Vec2 scale= Vec2(1,1), Vec2 offset = Vec2(0,0));
    Rect box;
    void Update(float dt) override;
    void Render() override;
    bool Is(std::string type) const override;
    void SetScale(Vec2 scale);
    void SetOffset(Vec2 offset);


private:
    Vec2 scale;
    Vec2 offset;
};

#endif

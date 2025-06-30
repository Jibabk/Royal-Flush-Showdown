#ifndef RECT_H
#define RECT_H

#include "Vec2.h"
class Rect {
public:
    float x, y, w, h;
    Rect() : x(0), y(0), w(0), h(0) {}
    Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

    Vec2 Center() const {
        return Vec2(x + w / 2.0f, y + h / 2.0f);
    }
};

#endif

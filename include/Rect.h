#ifndef RECT_H
#define RECT_H

#include "Vec2.h"
#include <iostream>
class Rect {
public:
    float x, y, w, h;
    Rect() : x(0), y(0), w(0), h(0) {}
    Rect(float x, float y, float w, float h) : x(x), y(y), w(w), h(h) {}

    bool Contains(const Vec2& point) const {
        return (point.x >= x && point.x <= x + w &&
                point.y >= y && point.y <= y + h);
    }

    bool OverlapsWith(const Rect& other) const {
        std::cout << "Checking overlap with Rect: (x: " 
                  << other.x << ", y: " << other.y << ", w: " 
                  << other.w << ", h: " << other.h << ")\n";
        std::cout << "Current Rect: (x: " 
                  << x << ", y: " << y << ", w: "
                  << w << ", h: " << h << ")\n";
        // Verifica se há interseção entre dois retângulos
        return x < other.x + other.w &&
               x + w > other.x &&
               y < other.y + other.h &&
               y + h > other.y;
    }



    Vec2 Center() const {
        return Vec2(x + w / 2.0f, y + h / 2.0f);
    }

    Vec2 Pos() const {
    return Vec2(x, y);
}

    void SetPos(const Vec2& pos) {
        x = pos.x;
        y = pos.y;
    }

    void SetCenter(const Vec2& center) {
        x = center.x - w / 2.0f;
        y = center.y - h / 2.0f;
}


};



#endif

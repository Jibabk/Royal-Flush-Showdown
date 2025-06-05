#ifndef VEC2_H
#define VEC2_H

#include <cmath>

class Vec2 {
public:
    float x, y;

    Vec2();
    Vec2(float x, float y);

    float Magnitude() const;
    Vec2 Normalized() const;
    Vec2 Rotate(float angleDeg) const;
    float Angle() const;
    float Distance(const Vec2& other) const;

    Vec2 operator+(const Vec2& other) const;
    Vec2 operator-(const Vec2& other) const;
    Vec2 operator*(float scalar) const;
    Vec2 operator/(float scalar) const;

    Vec2& operator+=(const Vec2& other);
    Vec2& operator-=(const Vec2& other);
    Vec2& operator*=(float scalar);
    Vec2& operator/=(float scalar);
    Vec2 GetRotated(float angleDeg);
};

#endif

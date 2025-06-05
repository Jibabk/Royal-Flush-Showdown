#include "Vec2.h"

Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(float x, float y) : x(x), y(y) {}

float Vec2::Magnitude() const {
    return std::sqrt(x * x + y * y);
}

Vec2 Vec2::Normalized() const {
    float mag = Magnitude();
    if (mag == 0) return Vec2(0, 0);
    return Vec2(x / mag, y / mag);
}

Vec2 Vec2::Rotate(float angleDeg) const {
    float rad = angleDeg;
    float cosA = cos(rad);
    float sinA = sin(rad);
    return Vec2(x * cosA - y * sinA, x * sinA + y * cosA);
}

float Vec2::Angle() const {
    return std::atan2(y, x) * 180.0f / M_PI;
}

float Vec2::Distance(const Vec2& other) const {
    float dx = x - other.x;
    float dy = y - other.y;
    return std::sqrt(dx * dx + dy * dy);
}

// Operadores

Vec2 Vec2::operator+(const Vec2& other) const {
    return Vec2(x + other.x, y + other.y);
}

Vec2 Vec2::operator-(const Vec2& other) const {
    return Vec2(x - other.x, y - other.y);
}

Vec2 Vec2::operator*(float scalar) const {
    return Vec2(x * scalar, y * scalar);
}

Vec2 Vec2::operator/(float scalar) const {
    return Vec2(x / scalar, y / scalar);
}

Vec2& Vec2::operator+=(const Vec2& other) {
    x += other.x;
    y += other.y;
    return *this;
}

Vec2& Vec2::operator-=(const Vec2& other) {
    x -= other.x;
    y -= other.y;
    return *this;
}

Vec2& Vec2::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vec2& Vec2::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

Vec2 Vec2::GetRotated(float angleDeg) {
    Vec2 result = *this;
    result.Rotate(angleDeg);
    return result;
}
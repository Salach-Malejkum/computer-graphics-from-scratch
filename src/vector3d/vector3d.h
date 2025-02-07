#pragma once

#include <math.h>
#include <SFML/Graphics.hpp>


struct Vector3D {
    float x = 0, y = 0, z = 0, length = 0;

    Vector3D(float x, float y, float z);
    Vector3D();

    float Length();

    Vector3D operator+(const Vector3D& other) const;
    Vector3D operator-(const Vector3D& other) const;
    Vector3D operator-();
    Vector3D operator*(const float num) const;
    Vector3D operator/(const float num) const;
    bool operator==(const Vector3D& other) const;

    static float dot_product(Vector3D v1, Vector3D v2);
};
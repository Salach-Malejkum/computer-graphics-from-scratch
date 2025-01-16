#pragma once

#include <math.h>
#include <SFML/Graphics.hpp>


struct Vector3D {
    float x = 0, y = 0, z = 0;

    Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}
    Vector3D() : x(0), y(0), z(0) {}

    float Length() {
        return sqrt(x * x + y * y + z * z);
    }

    Vector3D operator+(const Vector3D& other) const {
        return Vector3D(x + other.x, y + other.y, z + other.z);
    }

    Vector3D operator-(const Vector3D& other) const {
        return Vector3D(x - other.x, y - other.y, z - other.z);
    }

    Vector3D operator-() {
        this->x = -x;
        this->y = -y;
        this->z = -z;

        return *this;
    }

    Vector3D operator*(const float num) const {
        return Vector3D(x * num, y * num, z * num);
    }

    Vector3D operator/(const float num) const {
        return Vector3D(x / num, y / num, z / num);
    }

    static float dot_product(Vector3D v1, Vector3D v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }
};
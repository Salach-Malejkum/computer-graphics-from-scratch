#include "utils.h"


Vector3D::Vector3D(float x, float y, float z) : x(x), y(y), z(z), length(sqrt(x * x + y * y + z * z)) {}
Vector3D::Vector3D() : x(0), y(0), z(0), length(0) {}

float Vector3D::Length() {
    return length;
}

Vector3D Vector3D::operator+(const Vector3D& other) const {
    return Vector3D(x + other.x, y + other.y, z + other.z);
}

Vector3D Vector3D::operator-(const Vector3D& other) const {
    return Vector3D(x - other.x, y - other.y, z - other.z);
}

Vector3D Vector3D::operator-() {
    this->x = -x;
    this->y = -y;
    this->z = -z;

    return *this;
}

Vector3D Vector3D::operator*(const float num) const {
    return Vector3D(x * num, y * num, z * num);
}

Vector3D Vector3D::operator/(const float num) const {
    return Vector3D(x / num, y / num, z / num);
}

bool Vector3D::operator==(const Vector3D& other) const {
    return x == other.x && y == other.y && z == other.z;
}

float Vector3D::dot_product(Vector3D v1, Vector3D v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
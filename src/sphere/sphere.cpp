#include "sphere.h"

Sphere::Sphere() {
    center = std::make_shared<Vector3D>(Vector3D{0, 0, 0});
    radius = 0;
    color = std::make_shared<sf::Color>(sf::Color::White);
    specular = -1;
    reflective = 0;
}

Sphere::Sphere(const Vector3D& center, float radius, const sf::Color& color, float specular, float reflective) {
    this->center = std::make_shared<Vector3D>(center);
    this->radius = radius;
    this->color = std::make_shared<sf::Color>(color);
    this->specular = specular;
    this->reflective = reflective;
}

Sphere::Sphere(const Sphere& sphere) {
    if (sphere.center) {
        center = std::make_shared<Vector3D>(*sphere.center); // Deep copy of center
    }
    if (sphere.radius) {
        radius = sphere.radius;    // Deep copy of radius
    }
    if (sphere.color) {
        color = std::make_shared<sf::Color>(*sphere.color);  // Deep copy of color
    }
    if (sphere.specular) {
        specular = sphere.specular;  // Deep copy of specular
    }
    if (sphere.reflective) {
        reflective = sphere.reflective;  // Deep copy of reflective
    }
}

Sphere& Sphere::operator=(const Sphere& sphere) {
    if (this != &sphere) { // Avoid self-assignment
        // Deep copy of each member
        center = sphere.center;
        radius = sphere.radius;
        color = sphere.color;
        specular = sphere.specular;
        reflective = sphere.reflective;
    }
    return *this;
}

Sphere& Sphere::operator=(Sphere&& sphere) {
    if (this != &sphere) {
        center = std::move(sphere.center);
        radius = sphere.radius;
        color = std::move(sphere.color);
        specular = sphere.specular;
        reflective = sphere.reflective;
    }
    return *this;
}

bool Sphere::operator==(const Sphere& other) const {
    return *center == *other.center && radius == other.radius && *color == *other.color && specular == other.specular && reflective == other.reflective;
}

Vector3D Sphere::GetCenter() const {
    return *this->center;
}

float Sphere::GetRadius() const {
    return this->radius;
}

sf::Color Sphere::GetColor() const {
    return *this->color;
}

float Sphere::GetSpecular() const {
    return this->specular;
}

float Sphere::GetReflective() const {
    return this->reflective;
}
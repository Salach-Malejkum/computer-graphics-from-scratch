#include "sphere.h"

Sphere::Sphere() {
    center = std::make_unique<Vector3D>(Vector3D{0, 0, 0});
    radius = std::make_unique<float>(0);
    color = std::make_unique<sf::Color>(sf::Color::White);
    specular = std::make_unique<float>(-1);
    reflective = std::make_unique<float>(0);
}

Sphere::Sphere(const Vector3D& center, float radius, const sf::Color& color, float specular, float reflective) {
    this->center = std::make_unique<Vector3D>(center);
    this->radius = std::make_unique<float>(radius);
    this->color = std::make_unique<sf::Color>(color);
    this->specular = std::make_unique<float>(specular);
    this->reflective = std::make_unique<float>(reflective);
}

Sphere::~Sphere() {}

Sphere::Sphere(const Sphere& sphere) {
    if (sphere.center) {
        center = std::make_unique<Vector3D>(*sphere.center); // Deep copy of center
    }
    if (sphere.radius) {
        radius = std::make_unique<float>(*sphere.radius);    // Deep copy of radius
    }
    if (sphere.color) {
        color = std::make_unique<sf::Color>(*sphere.color);  // Deep copy of color
    }
    if (sphere.specular) {
        specular = std::make_unique<float>(*sphere.specular);  // Deep copy of specular
    }
    if (sphere.reflective) {
        reflective = std::make_unique<float>(*sphere.reflective);  // Deep copy of reflective
    }
}

Sphere& Sphere::operator=(const Sphere& sphere) {
    if (this != &sphere) { // Avoid self-assignment
        // Deep copy of each member
        center = sphere.center ? std::make_unique<Vector3D>(*sphere.center) : nullptr;
        radius = sphere.radius ? std::make_unique<float>(*sphere.radius) : nullptr;
        color = sphere.color ? std::make_unique<sf::Color>(*sphere.color) : nullptr;
        specular = sphere.specular ? std::make_unique<float>(*sphere.specular) : nullptr;
        reflective = sphere.reflective ? std::make_unique<float>(*sphere.reflective) : nullptr;
    }
    return *this;
}

Sphere& Sphere::operator=(Sphere&& sphere) {
    if (this != &sphere) {
        center = std::move(sphere.center);
        radius = std::move(sphere.radius);
        color = std::move(sphere.color);
        specular = std::move(sphere.specular);
        reflective = std::move(sphere.reflective);
    }
    return *this;
}

Vector3D* Sphere::GetCenter() const {
    return this->center.get();
}

float Sphere::GetRadius() const {
    return *this->radius;
}

sf::Color& Sphere::GetColor() const {
    return *this->color;
}

float Sphere::GetSpecular() const {
    return *this->specular;
}

float Sphere::GetReflective() const {
    return *this->reflective;
}
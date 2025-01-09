#include "sphere.h"

Sphere::Sphere() {
    center = std::make_unique<Vector3D>(Vector3D{0, 0, 0});
    radius = std::make_unique<float>(0);;
    color = std::make_unique<sf::Color>(sf::Color::White);
}

Sphere::Sphere(const Vector3D& center, float radius, const sf::Color& color) {
    this->center = std::make_unique<Vector3D>(center);
    this->radius = std::make_unique<float>(radius);
    this->color = std::make_unique<sf::Color>(color);
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
}

Sphere& Sphere::operator=(const Sphere& sphere) {
    if (this != &sphere) { // Avoid self-assignment
        // Deep copy of each member
        center = sphere.center ? std::make_unique<Vector3D>(*sphere.center) : nullptr;
        radius = sphere.radius ? std::make_unique<float>(*sphere.radius) : nullptr;
        color = sphere.color ? std::make_unique<sf::Color>(*sphere.color) : nullptr;
    }
    return *this;
}

Sphere& Sphere::operator=(Sphere&& sphere) {
    if (this != &sphere) {
        center = std::move(sphere.center);
        radius = std::move(sphere.radius);
        color = std::move(sphere.color);
    }
    return *this;
}
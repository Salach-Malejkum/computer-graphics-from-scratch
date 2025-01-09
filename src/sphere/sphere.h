#pragma once

#include <cstdint>
#include <memory>
#include <SFML/Graphics.hpp>

struct Vector3D {
    float x = 0, y = 0, z = 0;
};

class Sphere {
    public:
        std::unique_ptr<Vector3D> center;
        std::unique_ptr<float> radius;
        std::unique_ptr<sf::Color> color;
        Sphere();
        Sphere(const Vector3D& center, float radius, const sf::Color& color);
        ~Sphere();
        Sphere(const Sphere &sphere);
        Sphere& operator=(const Sphere &sphere);
        Sphere& operator=(Sphere&& sphere);
};
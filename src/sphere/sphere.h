#pragma once

#include <cstdint>
#include <memory>
#include <SFML/Graphics.hpp>
#include "../vector3d/vector3d.h"



class Sphere {
    private:
        std::shared_ptr<Vector3D> center;
        float radius;
        std::shared_ptr<sf::Color> color;
        float specular;
        float reflective;
    public:
        Sphere();
        Sphere(const Vector3D& center, float radius, const sf::Color& color, float specular, float reflective);
        ~Sphere() = default;
        Sphere(const Sphere &sphere);
        Sphere& operator=(const Sphere &sphere);
        Sphere& operator=(Sphere&& sphere);
        bool operator==(const Sphere& other) const;

        Vector3D GetCenter() const;
        float GetRadius() const;
        sf::Color GetColor() const;
        float GetSpecular() const;
        float GetReflective() const;
};
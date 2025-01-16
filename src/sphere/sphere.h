#pragma once

#include <cstdint>
#include <memory>
#include <SFML/Graphics.hpp>
#include "../utils.h"



class Sphere {
    private:
        std::unique_ptr<Vector3D> center;
        std::unique_ptr<float> radius;
        std::unique_ptr<sf::Color> color;
        std::unique_ptr<float> specular;
    public:
        Sphere();
        Sphere(const Vector3D& center, float radius, const sf::Color& color, float specular);
        ~Sphere();
        Sphere(const Sphere &sphere);
        Sphere& operator=(const Sphere &sphere);
        Sphere& operator=(Sphere&& sphere);

        Vector3D* GetCenter() const;
        float GetRadius() const;
        sf::Color& GetColor() const;
        float GetSpecular() const;
};
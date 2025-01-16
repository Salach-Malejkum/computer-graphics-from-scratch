#pragma once

#include "../utils.h"

enum LightType {
    AMBIENT,
    DIRECTIONAL,
    POINT
};

class ILight {   
    public:
        virtual ~ILight() = default;
        virtual float GetIntensity() const = 0;
        virtual LightType GetLightType() const = 0;
};

class AmbientLight : public ILight {
    private:
        LightType type;
        float intensity;

    public:
        AmbientLight(float intensity);
        float GetIntensity() const override;
        LightType GetLightType() const override;
};

class PointLight : public ILight {
    private:
        Vector3D position;
        LightType type;
        float intensity;

    public:
        PointLight(float intensity, Vector3D position);
        float GetIntensity() const override;
        LightType GetLightType() const override;
        Vector3D GetPosition();

};

class DirectionalLight : public ILight {
    private:
        Vector3D direction;
        LightType type;
        float intensity;

    public:
        DirectionalLight(float intensity, Vector3D direction);
        float GetIntensity() const override;
        LightType GetLightType() const override;
        Vector3D GetDirection();
};

sf::Color multiplyColorByIntensity(const sf::Color& color, float intensity);
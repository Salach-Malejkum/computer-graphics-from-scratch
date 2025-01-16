#include "light.h"

// AmbientLight
AmbientLight::AmbientLight(float intensity) {
    this->intensity = intensity;
    this->type = LightType::AMBIENT;
}
float AmbientLight::GetIntensity() const { return this->intensity; }
LightType AmbientLight::GetLightType() const { return this->type; }

// PointLight
PointLight::PointLight(float intensity, Vector3D position) {
    this->intensity = intensity;
    this->type = LightType::POINT;
    this->position = position;
}

float PointLight::GetIntensity() const { return this->intensity; }
LightType PointLight::GetLightType() const { return this->type; }
Vector3D PointLight::GetPosition() {
    return this->position;
}

// DirectionalLight
DirectionalLight::DirectionalLight(float intensity, Vector3D direction) {
    this->intensity = intensity;
    this->type = LightType::DIRECTIONAL;
    this->direction = direction;
}
float DirectionalLight::GetIntensity() const { return this->intensity; }
LightType DirectionalLight::GetLightType() const { return this->type; }
Vector3D DirectionalLight::GetDirection() {
    return this->direction;
}

sf::Color multiplyColorByIntensity(const sf::Color& color, float intensity) {
    return sf::Color(
        static_cast<std::uint8_t>(std::min(255.0f, color.r * intensity)),
        static_cast<std::uint8_t>(std::min(255.0f, color.g * intensity)),
        static_cast<std::uint8_t>(std::min(255.0f, color.b * intensity)),
        color.a
    );
}
#pragma once

#include <tuple>
#include <functional>
#include "../vector3d/vector3d.h"
#include "../sphere/sphere.h"

namespace std {
    template <>
    struct hash<Vector3D> {
        size_t operator()(const Vector3D& v) const noexcept {
            return hash<float>()(v.x) ^ (hash<float>()(v.y) << 1) ^ (hash<float>()(v.z) << 2);
        }
    };

    template <>
    struct hash<Sphere> {
        size_t operator()(const Sphere& sphere) const noexcept {
            size_t h1 = hash<Vector3D>()(sphere.GetCenter());
            size_t h2 = hash<float>()(sphere.GetRadius());
            size_t h3 = hash<int>()(sphere.GetColor().toInteger());
            size_t h4 = hash<float>()(sphere.GetSpecular());
            size_t h5 = hash<float>()(sphere.GetReflective());
            return h1 ^ (h2 << 1) ^ (h3 << 2) ^ (h4 << 3) ^ (h5 << 4);
        }
    };

    template <>
    struct hash<std::tuple<Vector3D, Vector3D, Sphere>> {
        size_t operator()(const std::tuple<Vector3D, Vector3D, Sphere>& t) const noexcept {
            const auto& [v1, v2, sphere] = t;
            size_t h1 = hash<Vector3D>()(v1);
            size_t h2 = hash<Vector3D>()(v2);
            size_t h3 = hash<Sphere>()(sphere);
            return h1 ^ (h2 << 1) ^ (h3 << 2);
        }
    };
}
#pragma once

#include <unordered_map>
#include <tuple>
#include <mutex>

#include "../sphere/sphere.h"
#include "../vector3d/vector3d.h"
#include "../common/hash_functions.h"


struct IntersectionCache {
    std::unordered_map<std::tuple<Vector3D, Vector3D, Sphere>, std::pair<float, float>> cache;
    std::mutex cache_mutex;

    bool get(const Vector3D& origin, const Vector3D& direction, const Sphere& sphere, float& t0, float& t1);
    void put(const Vector3D& origin, const Vector3D& direction, const Sphere& sphere, float t0, float t1);
};

// struct LightningCache {

// };

// struct RayCache {

// };
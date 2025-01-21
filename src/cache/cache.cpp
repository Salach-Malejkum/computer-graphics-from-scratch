#include "cache.h"


bool IntersectionCache::get(const Vector3D& origin, const Vector3D& direction, const Sphere& sphere, float& t0, float& t1) {
    std::lock_guard lock(cache_mutex);
    auto key = std::make_tuple(origin, direction, sphere);
    auto it = cache.find(key);
    if (it != cache.end()) {
        t0 = it->second.first;
        t1 = it->second.second;
        return true; // Cached result found
    }

    return false;
}

void IntersectionCache::put(const Vector3D& origin, const Vector3D& direction, const Sphere& sphere, float t0, float t1) {
    std::lock_guard lock(cache_mutex);
    auto key = std::make_tuple(origin, direction, sphere);
    cache[key] = {t0, t1};
}

// g++ -o main.exe main.cpp src/sphere/sphere.cpp src/light/light.cpp src/cache/cache.cpp src/utils/utils.cpp -I include -I C:/SFML/include -I "C:\Program Files (x86)\Intel\oneAPI\tbb\latest\include" -L "C:\Program Files (x86)\Intel\oneAPI\tbb\latest\lib\vc14_uwd" -L C:/SFML/lib -lsfml-graphics -lsfml-window -lsfml-system -ltbb -ltbbmalloc
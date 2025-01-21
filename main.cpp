#include <vector>
#include <math.h>
#include <thread>
#include <chrono>

#include "src/sphere/sphere.h"
#include "src/light/light.h"
// #include "src/cache/cache.h"


const int C_WIDTH = 600, C_HEIGHT = 600, V_WIDTH = 1, V_HEIGHT = 1, d = 1;
// IntersectionCache intersectionCache;

Vector3D CanvasToViewport(float x, float y) {
    Vector3D viewport{x * V_WIDTH / C_WIDTH, y * V_HEIGHT / C_HEIGHT, d};
    return viewport;
}

bool IntersectRaySphere(Vector3D origin, Vector3D direction, Sphere sphere, float &t0, float &t1) {
    // if (intersectionCache.get(origin, direction, sphere, t0, t1)) {
    //     return true;
    // }


    int r = sphere.GetRadius();
    Vector3D oc = origin - sphere.GetCenter();


    float a = Vector3D::dot_product(direction, direction);
    float b = 2 * Vector3D::dot_product(oc, direction);
    float c = Vector3D::dot_product(oc, oc) - r * r;

    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0) {
        t0 = INFINITY;
        t1 = INFINITY;
        return false;
    }

    t0 = (-b + sqrt(discriminant)) / (2 * a);
    t1 = (-b - sqrt(discriminant)) / (2 * a);

    // intersectionCache.put(origin, direction, sphere, t0, t1);

    return true;
}


Vector3D ReflectRay(Vector3D ray, Vector3D normal) {
    return normal * 2 * Vector3D::dot_product(normal, ray) - ray;
}


void ClosestIntersection(
    const std::vector<std::unique_ptr<ILight>>& lights,
    const std::vector<Sphere>& spheres,
    Vector3D origin, Vector3D direction,
    float t_min, float t_max,
    float& closest_t, std::shared_ptr<Sphere>& closest_sphere) {

    if (t_max < 0) {
        return;
    }

    for (const Sphere& sphere : spheres) {
        float t0, t1;
        
        if (IntersectRaySphere(origin, direction, sphere, t0, t1))
        {
            if (t0 >= t_min && t0 <= t_max && t0 < closest_t) {
                closest_t = t0;
                closest_sphere = std::make_shared<Sphere>(sphere);
            }
            if (t1 >= t_min && t1 <= t_max && t1 < closest_t) {
                closest_t = t1;
                closest_sphere = std::make_shared<Sphere>(sphere);
            }
        }
    }
}


float ComputeLighting(const std::vector<std::unique_ptr<ILight>>& lights, const std::vector<Sphere>& spheres, Vector3D point, Vector3D normal, Vector3D view, float specular) {
    float intensity = 0.0;
    float t_max = -1;

    for (const auto& light : lights) {
        if (light->GetLightType() == LightType::AMBIENT) {
            intensity += light->GetIntensity();
        } else {
            Vector3D lightVect;

            if (light->GetLightType() == LightType::POINT) {
                Vector3D light_pos = dynamic_cast<PointLight*>(light.get())->GetPosition();
                lightVect = light_pos - point;
                t_max = 1;
            } else {
                lightVect = dynamic_cast<DirectionalLight*>(light.get())->GetDirection();
                t_max = INFINITY;
            }

            // shadow check
            float shadow_t = INFINITY;
            std::shared_ptr<Sphere> shadow_sphere = nullptr;
            ClosestIntersection(lights, spheres, point, lightVect, 0.001, t_max, shadow_t, shadow_sphere);

            if (shadow_sphere != NULL) {
                continue;
            }

            // difuse light
            float n_dot_l = Vector3D::dot_product(normal, lightVect);
            if (n_dot_l > 0) {
                intensity += light->GetIntensity() * n_dot_l/(normal.Length() * lightVect.Length());
            }

            // specular light
            if (specular != -1) {
                Vector3D reflection = ReflectRay(lightVect, normal);
                float r_dot_v = Vector3D::dot_product(reflection, view);

                if (r_dot_v > 0) {
                    intensity += light->GetIntensity() * pow(r_dot_v / (reflection.Length() * view.Length()), specular);
                }
            }
        }
    }
    return std::max(0.0f, std::min(1.0f, intensity));
}


sf::Color TraceRay(
    const std::vector<std::unique_ptr<ILight>>& lights,
    const std::vector<Sphere>& spheres,
    Vector3D origin,
    Vector3D direction,
    float t_min, 
    float t_max, 
    int rec_depth
    ) {

    float closest_t = INFINITY;
    std::shared_ptr<Sphere> closest_sphere = nullptr;
    ClosestIntersection(lights, spheres, origin, direction, t_min, t_max, closest_t, closest_sphere);

    if (closest_sphere == NULL) {
        return sf::Color::Black;
    }

    Vector3D point = origin + direction * closest_t;
    Vector3D normal = point - closest_sphere->GetCenter();
    normal = normal / normal.Length();
    sf::Color localColor = multiplyColorByIntensity(closest_sphere->GetColor(), ComputeLighting(lights, spheres, point, normal, -direction, closest_sphere->GetSpecular()));
    
    float reflective = closest_sphere->GetReflective();
    if (rec_depth <= 0 || reflective <= 0) {
        return localColor;
    }

    Vector3D ray = ReflectRay(-direction, normal);
    sf::Color reflected_color = TraceRay(lights, spheres, point + normal * 0.001f, -ray, 0.001, INFINITY, rec_depth - 1);
    return multiplyColorByIntensity(localColor, (1 - reflective)) + multiplyColorByIntensity(reflected_color, reflective);
}

void CreateSpheres(std::vector<Sphere>& spheres) {
    spheres.push_back(Sphere(Vector3D{0, 1, 3}, 1, sf::Color::Red, 500, 0.2)); // red
    spheres.push_back(Sphere(Vector3D{2, 0, 4}, 1, sf::Color::Blue, 500, 0.3)); // blue
    spheres.push_back(Sphere(Vector3D{-2, 0, 4}, 1, sf::Color::Green, 10, 0.4)); // green
    spheres.push_back(Sphere(Vector3D{0, 5001, 0}, 5000, sf::Color::Yellow, 1000, 0.5)); //yellow
}

void CreateLights(std::vector<std::unique_ptr<ILight>>& lights) {
    lights.push_back(std::make_unique<AmbientLight>(0.2));
    lights.push_back(std::make_unique<PointLight>(0.6, Vector3D{2, -1, 0}));
    lights.push_back(std::make_unique<DirectionalLight>(0.2, Vector3D{1, -4, 4}));
}


void ParallelRayTracing(sf::Image& canvas, const std::vector<std::unique_ptr<ILight>>& lights, const std::vector<Sphere>& spheres, const Vector3D& origin, int width_min, int width_max, int heigth_min, int height_max, int rec_depth) {
    for (int x = width_min; x < width_max; x++) 
    {
        for (int y = heigth_min; y < height_max; y++) 
        {
            Vector3D direction = CanvasToViewport(x, y);
            sf::Color color = TraceRay(lights, spheres, origin, direction, 1, INFINITY, rec_depth);
            sf::Vector2u pixel_pos(x + C_WIDTH / 2, y + C_HEIGHT / 2);
            canvas.setPixel(pixel_pos, color);
        }
    }
}


int main()
{
    Vector3D origin(0, 0, 0);
    std::vector<Sphere> spheres;
    std::vector<std::unique_ptr<ILight>> lights;

    CreateSpheres(spheres);
    CreateLights(lights);

    sf::RenderWindow window(sf::VideoMode({C_WIDTH, C_HEIGHT}), "Computer Graphics - intro!");
    sf::Image canvas;
    sf::Vector2u canvas_size(C_WIDTH, C_HEIGHT);
    canvas.resize(canvas_size, sf::Color::White);
    int rec_depth = 1;

    auto start = std::chrono::high_resolution_clock::now();

    std::thread top_left(ParallelRayTracing, std::ref(canvas), std::ref(lights), std::ref(spheres), std::ref(origin), -C_WIDTH / 2, 0, -C_HEIGHT / 2, 0, rec_depth);
    std::thread top_right(ParallelRayTracing, std::ref(canvas), std::ref(lights), std::ref(spheres), std::ref(origin), 0, C_WIDTH / 2, -C_HEIGHT / 2, 0, rec_depth);
    std::thread bot_left(ParallelRayTracing, std::ref(canvas), std::ref(lights), std::ref(spheres), std::ref(origin), -C_WIDTH / 2, 0, 0, C_HEIGHT / 2, rec_depth);
    std::thread bot_right(ParallelRayTracing, std::ref(canvas), std::ref(lights), std::ref(spheres), std::ref(origin), 0, C_WIDTH / 2, 0, C_HEIGHT / 2, rec_depth);

    top_left.join();
    top_right.join();
    bot_left.join();
    bot_right.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    printf("Execution time: %f", duration.count());

    const sf::Texture texture(canvas);
    sf::Sprite sprite(texture);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
}
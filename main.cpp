#include <vector>
#include <math.h>
#include "src/sphere/sphere.h"
#include "src/light/light.h"

const int C_WIDTH = 600, C_HEIGHT = 600, V_WIDTH = 1, V_HEIGHT = 1, d = 1;

Vector3D CanvasToViewport(float x, float y) {
    Vector3D viewport{x * V_WIDTH / C_WIDTH, y * V_HEIGHT / C_HEIGHT, d};
    return viewport;
}

bool IntersectRaySphere(Vector3D origin, Vector3D direction, Sphere sphere, float &t0, float &t1) {
    int r = sphere.GetRadius();
    Vector3D oc = origin - *dynamic_cast<Vector3D*>(sphere.GetCenter());


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
    return true;
}


float ComputeLighting(const std::vector<std::unique_ptr<ILight>>& lights, Vector3D point, Vector3D normal, Vector3D view, float specular) {
    float intensity = 0.0;

    for (const auto& light : lights) {
        if (light->GetLightType() == LightType::AMBIENT) {
            intensity += light->GetIntensity();
        } else {
            Vector3D lightVect;

            if (light->GetLightType() == LightType::POINT) {
                Vector3D light_pos = dynamic_cast<PointLight*>(light.get())->GetPosition();
                lightVect = light_pos - point;
            } else {
                lightVect = dynamic_cast<DirectionalLight*>(light.get())->GetDirection();
            }

            // difuse light
            float n_dot_l = Vector3D::dot_product(normal, lightVect);
            if (n_dot_l > 0) {
                intensity += light->GetIntensity() * n_dot_l/(normal.Length() * lightVect.Length());
            }

            // specular light
            if (specular != -1) {
                Vector3D reflection = normal * 2  * n_dot_l - lightVect;
                float r_dot_v = Vector3D::dot_product(reflection, view);

                if (r_dot_v > 0) {
                    intensity += light->GetIntensity() * pow(r_dot_v / (reflection.Length() * view.Length()), specular);
                }
            }
        }
    }
    return intensity;
}

sf::Color TraceRay(
    const std::vector<std::unique_ptr<ILight>>& lights,
    const std::vector<Sphere>& spheres,
    Vector3D origin, Vector3D direction,
    float tMin, float tMax
    ) {
    float closest_t = INFINITY;
    const Sphere* closest_sphere = nullptr;

    for (const Sphere& sphere : spheres) {
        float t0, t1;
        
        if (IntersectRaySphere(origin, direction, sphere, t0, t1))
        {
            if (t0 >= tMin && t0 <= tMax && t0 < closest_t) {
                closest_t = t0;
                closest_sphere = &sphere;
            }
            if (t1 >= tMin && t1 <= tMax && t1 < closest_t) {
                closest_t = t1;
                closest_sphere = &sphere;
            }
        }
    }

    if (closest_sphere == NULL) {
        return sf::Color::White;
    }

    Vector3D point = origin + direction * closest_t;
    Vector3D normal = point - *dynamic_cast<Vector3D*>(closest_sphere->GetCenter());
    normal = normal / normal.Length();
    return multiplyColorByIntensity(closest_sphere->GetColor(), ComputeLighting(lights, point, normal, -direction, closest_sphere->GetSpecular()));
}

void CreateSpheres(std::vector<Sphere>& spheres) {
    spheres.push_back(Sphere(Vector3D{0, 1, 3}, 1, sf::Color::Red, 500)); // red
    spheres.push_back(Sphere(Vector3D{2, 0, 4}, 1, sf::Color::Blue, 500)); // blue
    spheres.push_back(Sphere(Vector3D{-2, 0, 4}, 1, sf::Color::Green, 10)); // green
    spheres.push_back(Sphere(Vector3D{0, 5001, 0}, 5000, sf::Color::Yellow, 5000)); //yellow
}

void CreateLights(std::vector<std::unique_ptr<ILight>>& lights) {
    lights.push_back(std::make_unique<AmbientLight>(0.2));
    lights.push_back(std::make_unique<PointLight>(0.6, Vector3D{2, -1, 0}));
    lights.push_back(std::make_unique<DirectionalLight>(0.2, Vector3D{1, -4, 4}));
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

    for (int x = -C_WIDTH / 2; x < C_WIDTH / 2; x++) // 
    {
        for (int y = -C_HEIGHT / 2; y < C_HEIGHT / 2; y++) // 
        {
            Vector3D direction = CanvasToViewport(x, y);
            sf::Color color = TraceRay(lights, spheres, origin, direction, 1, INFINITY);
            sf::Vector2u pixel_pos(x + C_WIDTH / 2, y + C_HEIGHT / 2);
            canvas.setPixel(pixel_pos, color);
        }
    }

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
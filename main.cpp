#include <vector>
#include <math.h>
#include "src/sphere/sphere.h"

const int C_WIDTH = 600, C_HEIGHT = 600, V_WIDTH = 1, V_HEIGHT = 1, d = 1;

Vector3D CanvasToViewport(float x, float y) {
    Vector3D viewport{x * V_WIDTH / C_WIDTH, y * V_HEIGHT / C_HEIGHT, d};
    return viewport;
}

bool IntersectRaySphere(Vector3D origin, Vector3D direction, Sphere sphere, float &t0, float &t1) {
    int r = *sphere.radius;
    Vector3D oc = {origin.x - sphere.center->x, origin.y - sphere.center->y, origin.z - sphere.center->z};

    float a = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;
    float b = 2 * (oc.x * direction.x + oc.y * direction.y + oc.z * direction.z);
    float c = oc.x * oc.x + oc.y * oc.y + oc.z * oc.z - r * r;

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

sf::Color TraceRay(const std::vector<Sphere>& spheres, Vector3D origin, Vector3D direction, float tMin, float tMax) {
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

    if (closest_sphere != NULL) {
        return *closest_sphere->color;
    } else {
        return sf::Color::White;
    }
}

int main()
{
    Vector3D origin;
    std::vector<Sphere> spheres;
    spheres.push_back(Sphere(Vector3D{0, 1, 3}, 1, sf::Color::Red)); // red
    spheres.push_back(Sphere(Vector3D{2, 0, 4}, 1, sf::Color::Blue)); // blue
    spheres.push_back(Sphere(Vector3D{-2, 0, 4}, 1, sf::Color::Green)); // green

    sf::RenderWindow window(sf::VideoMode({C_WIDTH, C_HEIGHT}), "Computer Graphics - intro!");
    sf::Image canvas;
    sf::Vector2u canvas_size(C_WIDTH, C_HEIGHT);
    canvas.resize(canvas_size, sf::Color::White);

    for (int x = -C_WIDTH / 2; x < C_WIDTH / 2; x++) // 
    {
        for (int y = -C_HEIGHT / 2; y < C_HEIGHT / 2; y++) // 
        {
            Vector3D direction;
            direction = CanvasToViewport(x, y);
            sf::Color color = TraceRay(spheres, origin, direction, 1, INFINITY);
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
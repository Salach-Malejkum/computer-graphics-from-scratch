#include <vector>
#include <math.h>
#include <thread>
#include <chrono>

#include "src/sphere/sphere.h"
#include "src/light/light.h"
#include "src/common/operations.h"
// #include "src/cache/cache.h"

// IntersectionCache intersectionCache;


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
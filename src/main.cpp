
#include <string>

#include "SPGL/SPGL/SPGL.hpp"
#include "constants.hpp"
#include "light.hpp"
#include "camera.hpp"

#include <chrono>
#include <thread>

using namespace sb;

const Light lights[] = {
    Light(Vec3d(6, 1, -2), SPGL::Color(255, 16, 8), 16),
    Light(Vec3d(-2, 6, -1), SPGL::Color(8, 255, 16), 16),
    Light(Vec3d(-1, 2, 6), SPGL::Color(16, 8, 255), 16),
    Light(Vec3d(-32, 8, -16), SPGL::Color(255, 255, 192), 256),
    Light(Vec3d(24, 8, 24), SPGL::Color(16, 255, 192), 96),
};

SPGL::Color march(SDF sdf, Ray r) {
    SPGL::Color out = SPGL::Color::Black;

    int hits = 0;
    double distance = 0;
    double step = 0;

    for(int i = 0; i < MAX_MARCH_ITER; ++i) {
        step = sdf(r.pos());
        distance += step;
        
        if(MAX_STEP < step) {
            break;
        }

        if(MAX_HITS < hits) {
            break;
        }

        if(step < EPS) {
            hits += 1;
            double darkness = std::pow(3, hits - 1);
            for(const Light& l : lights) {
                out += l.getColor(sdf, r) / darkness;
            }
            r = r.reflect(sdf).fix(sdf, FIXING_RATIO * EPS);
        } else {
            r = r.step(step);
        }
    }

    return out;
}

int main() {
    const SDF scene = (SDF::Plane(Vec3d(-1, 0, -1), 48))
            | (SDF::Plane(Vec3d(1, 0, -1), 48))
            | (SDF::Plane(Vec3d(-1, 0, 1), 48))
            | (SDF::Plane(Vec3d(1, 0, 1), 48))
            | (SDF::Plane(Vec3d(0, 1, 0), 5))
            | (SDF::Sphere(16) + Vec3d(-0, -0, -32))
            | ((0.5 + SDF::Box(Vec3d(2, 2, 2))) ^ SDF::Sphere(3.3) | SDF::Sphere(1));

    SPGL::Window<> window(WIDTH, HEIGHT, "Sam Marcher");
    SPGL::Image img(WIDTH, HEIGHT);

    Camera cam(WIDTH, HEIGHT);

    std::thread threads[THREADS];
    for(int g = 0; g < THREADS; ++g) {
        threads[g] = std::thread([=,&img,&cam](){
            const int T = WIDTH*HEIGHT;
            const int gaps = T / THREADS;
            for(;;) {
                for(int i = gaps * g; i < std::min(gaps * (g + 1), T); ++i) {
                    img(i) = march(scene, cam(i % WIDTH, i / WIDTH));
                }
            }
        });
    }

    double t = 1;
    while(window.isRunning()) {
        t += 0.0005;

        cam.setPos(Vec3d(10*std::cos(t), 5*pow(sin(t), 2), 10*std::sin(t)));    

        window.renderImage(img);
        window.update();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 10));
    }


}
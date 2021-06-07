
#include <string>

#include "SPGL/SPGL/SPGL.hpp"
#include "light.hpp"
#include "camera.hpp"

#include <chrono>
#include <thread>

using namespace sb;

const std::size_t width = 1024;
const std::size_t height = 640;

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

SPGL::Window<> window(width, height, "Sam Marcher");
SPGL::Image img(width, height);

Camera cam(width, height);

void row(int group, int groups) {
    const SDF scene = (SDF::Plane(Vec3d(-1, 0, -1), 48))
            | (SDF::Plane(Vec3d(1, 0, -1), 48))
            | (SDF::Plane(Vec3d(-1, 0, 1), 48))
            | (SDF::Plane(Vec3d(1, 0, 1), 48))
            | (SDF::Plane(Vec3d(0, 1, 0), 5))
            | (SDF::Sphere(16) + Vec3d(-0, -0, -32))
            | ((0.5 + SDF::Box(Vec3d(2, 2, 2))) ^ SDF::Sphere(3.3) | SDF::Sphere(1));
    for(;;){
        int total = width*height;
        for(int i = group; i < total; i += groups) {
            img(i) = march(scene, cam(i % width, i / width));
        }
    }
}

int main() {


    double t = 1;
    const int groups = 16;
    std::thread threads[groups];
    for(int i = 0; i < groups; ++i) {
        threads[i] = std::thread(row, i, groups);
    }

    while(window.isRunning()) {
        t += 0.01;

        cam.setPos(Vec3d(10*std::cos(t), 5*pow(sin(t), 2), 10*std::sin(t)));    

        window.renderImage(img);
        window.update();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 4));
    }


}
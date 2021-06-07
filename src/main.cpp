
#include <string>

#include "SPGL/SPGL/SPGL.hpp"
#include "light.hpp"
#include "camera.hpp"
#include <thread>

using namespace sb;

const std::size_t width = 1024;
const std::size_t height = 640;

const Light lights[] = {
    Light(Vec3d(6, 1, -2), SPGL::Color(255, 16, 8), 16),
    Light(Vec3d(-2, 6, -1), SPGL::Color(8, 255, 16), 16),
    Light(Vec3d(-1, 2, 6), SPGL::Color(16, 8, 255), 16),
    Light(Vec3d(-32, 8, -16), SPGL::Color(255, 255, 192), 256)
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
            r = r.reflect(sdf).fix(sdf, FIX_EPS);
        } else {
            r = r.step(step);
        }
    }

    return out;
}

SPGL::Window<> window(width, height, "Myles Marcher");
SPGL::Image img(width, height);

Camera cam(width, height);

void row(int y) {
    const SDF scene = (SDF::Plane(Vec3d(0, 1, 0), 5))
            | (SDF::Sphere(16) + Vec3d(-0, -0, -32))
            | ((0.5 + SDF::Box(Vec3d(2, 2, 2))) ^ SDF::Sphere(3.3) | SDF::Sphere(1));
    for(int x = 0; x < width; ++x) {
        img(x, y) = march(scene, cam(x, y));
    }
}

int main() {


    double t = 1;

    std::thread threads[height];

    while(window.isRunning()) {
        t += 0.1;

        cam.setPos(Vec3d(10*std::cos(t), 10, 10*std::sin(t)));
        for(int y = 0; y < height; ++y) {
            threads[y] = std::thread(row, y);
        }

        for(auto& t : threads) {
            t.join();
        }

        window.renderImage(img);
        window.update();
    }


}
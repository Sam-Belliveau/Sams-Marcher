
#include <string>

#include "SPGL/SPGL/SPGL.hpp"
#include "light.hpp"
#include "camera.hpp"
#include <thread>

using namespace sb;

const std::size_t width = 1024;
const std::size_t height = 640;

const Light lights[] = {
    Light(Vec3d(6, 1, -2), SPGL::Color(224, 64, 64), 16),
    Light(Vec3d(-2, 6, -1), SPGL::Color(64, 224, 64), 16),
    Light(Vec3d(-1, 2, 6), SPGL::Color(64, 64, 244), 16),
    Light(Vec3d(-32, 4, -16), SPGL::Color(224, 224, 224), 128)
};

SPGL::Color march(SDF sdf, Ray r) {
    SPGL::Color out = SPGL::Color::Black;

    int hits = 0;
    double distance = 0;
    double step = 0;

    for(int i = 0; i < 128; ++i) {
        step = sdf(r.pos());
        distance += step;
        
        if(256 < step) {
            break;
        }

        if(2 < hits) {
            break;
        }

        if(step < EPS) {
            hits += 1;
            double darkness = std::pow(std::max(1.0, distance / 4.0), hits - 1);
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

int main() {

    SPGL::Window<> window(width, height, "Myles Marcher");
    SPGL::Image img(width, height);

    SDF scene = SDF::Plane(Vec3d(0, 1, 0), 4)
                | (SDF::Sphere(6) + Vec3d(-16, -4, -32))
                | ((0.5 + SDF::Box(Vec3d(2, 2, 2))) ^ SDF::Sphere(3.3) | SDF::Sphere(1));
    Camera cam(width, height);

    double t = 1;

    while(window.isRunning()) {
        t += 0.1;

        cam.setPos(Vec3d(10*std::cos(t), 4, 10*std::sin(t)));
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                img(x, y) = march(scene, cam(x, y));
            }
            if(y % 80 == 0)
            window.update();
        }

        window.renderImage(img);
        window.update();
    }


}
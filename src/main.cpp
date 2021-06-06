
#include <string>

#include "SPGL/SPGL/SPGL.hpp"
#include "camera.hpp"

using namespace sb;

const std::size_t width = 640;
const std::size_t height = 480;

int march(SDF sdf, Ray r) {
    const double EPS = 1e-2;

    int hits = 0;
    double step = 0;

    for(int i = 0; i < 256 && hits < 9; ++i) {
        step = sdf(r.pos());
        
        if(20 < step) {
            break;
        }

        if(9 < hits) {
            break;
        }

        if(step < EPS) {
            hits += 1;
            r = r.reflect(sdf).fix(sdf);
        } else {
            r = r.step(step);
        }
    }

    return hits;
}

int main() {

    SPGL::Window<> window(width, height, "Myles Marcher");
    SPGL::Image img(width, height);

    SDF scene = 2 * ((SDF::Sphere(0.5) - Vec3d(1, 0, 0)) | (SDF::Sphere(0.5) + Vec3d(1, 0, 0)));
    Camera cam(width, height);

    double t = 0;

    while(window.isRunning()) {
        t += 0.25;
        cam.set_pos(Vec3d(5*std::cos(t), 0, 5*std::sin(t)));
        for(int y = 0; y < height; ++y) {
            for(int x = 0; x < width; ++x) {
                img(x, y) = SPGL::Color(SPGL::UInt8(20 + 20 * march(scene, cam(x, y))));
            }   
            window.renderImage(img);
            window.update();
        }
        window.update();
    }


}
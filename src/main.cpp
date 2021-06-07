
#include <string>

#include "SPGL/SPGL/SPGL.hpp"
#include "constants.hpp"
#include "light.hpp"
#include "camera.hpp"
#include "scene.hpp"

#include <thread>

using namespace sb;

int main() {
    std::thread threads[THREADS];

    SPGL::Window<> window(WIDTH, HEIGHT, "Sam Marcher");

    const SDF sdf =  (~SDF::Box(Vec3d(32, 32, 32)) + Vec3d(0, 28, 0))
            | (SDF::Sphere(16) + Vec3d(-0, -0, -32))
            | (((0.5 + SDF::Box(Vec3d(2, 2, 2))) - SDF::Sphere(3.3)) | SDF::Sphere(1));
    
    const std::vector<Light> lights = {
        Light(Vec3d(+8, +4, -2), SPGL::Color(255, 64, 16), 24),
        Light(Vec3d(-2, +8, -4), SPGL::Color(16, 255, 64), 24),
        Light(Vec3d(-4, +2, +8), SPGL::Color(64, 16, 255), 24),
        Light(Vec3d(-20, 16, -20), SPGL::Color(255, 192, 64), 48),
        Light(Vec3d(+20, 16, +20), SPGL::Color(64, 255, 192), 48),
    };

    Scene scene(sdf, lights, SPGL::Image(WIDTH, HEIGHT));


    double t = 1.5;
    while(window.isRunning()) {
        t += 0.6;
        scene.camera.setPos(Vec3d(10*std::cos(t), 10, 10*std::sin(t)));

        for(int g = 0; g < THREADS; ++g) {
            threads[g] = std::thread([=,&scene]() {
                int gaps = PIXELS / THREADS;
                for(int i = gaps * g; i < std::min(gaps * (g + 1), PIXELS); ++i) {
                    scene.updatePixel(i);
                }
            });
        }
        for(auto& t : threads) {
            t.join();
        }

        window.renderImage(scene.image);
        window.update();

    }


}

#include <string>

#include "SPGL/SPGL/SPGL.hpp"
#include "constants.hpp"
#include "light.hpp"
#include "camera.hpp"
#include "scene.hpp"
#include "mat3.hpp"

#include <thread>

using namespace sb;


int main() {
    std::thread threads[THREADS];

    SPGL::Window<> window(WIDTH, HEIGHT, "Sam Marcher");

    const FloatT PI = SPGL::Math::Pi;

    const SDF sdf = 
        (~(Mat3d::Roll(PI / 2) * SDF::Cylinder(12) | Mat3d::Yaw(PI / 2) * SDF::Cylinder(12) | SDF::Box(Vec3d(24, 24, 24))) + Vec3d(0, 0, 0))
        | (SDF::Sphere(4) + Vec3d(-16, -0, -16))
        | (SDF::Sphere(4) + Vec3d(-16, -0, -16))
        | (SDF::Sphere(4) + Vec3d(-16, -0, -16))
        | ((SDF::Squircle(3) - SDF::Sphere(3.5)) | SDF::Sphere(1));

    const std::vector<Light> lights = {
        Light(Vec3d(0, 20, 0), SPGL::Color(224, 224, 192), 480),
        Light(Vec3d(-48, 4, 4), SPGL::Color(255, 16, 64), 320),
        Light(Vec3d(48, -4, -4), SPGL::Color(64, 16, 255), 320),
        Light(Vec3d(4, -4, -48), SPGL::Color(64, 255, 16), 320),
        Light(Vec3d(-4, 4, 48), SPGL::Color(64, 128, 255), 320)
    };

    Scene scene(sdf, lights, SPGL::Image(WIDTH, HEIGHT));


    FloatT t = 1.5;
    while(window.isRunning()) {
        t += 0.1;
        scene.camera.setFov(90);
        scene.camera.setPos(Vec3d(20*std::cos(t), 10, 20*std::sin(t)));

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
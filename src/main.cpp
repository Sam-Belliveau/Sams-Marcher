
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
        Vec3(5, 0, 0) + Mat3d::Pitch(PI / 4) * Mat3d::Roll(PI / 4) * (
            (
                Mat3d::Identity() * SDF::Cylinder(1) |
                Mat3d::Pitch(PI/2) * SDF::Cylinder(1) |
                Mat3d::Roll(PI/2) * SDF::Cylinder(1)
            ) 
            & SDF::Sphere(6)
        )
        | Vec3(-5, 0, 0) + Mat3d::Pitch(PI / 6) * Mat3d::Roll(PI / 6) * (
            SDF::Squircle(2)
        )
        | SDF::Plane(Vec3d(0, 1, 0), 6);
    
    const std::vector<Light> lights = {
        Light(Vec3d(16, 16, 0), SPGL::Color(255, 64, 16), 320),
        Light(Vec3d(0, 16, 16), SPGL::Color(16, 255, 64), 320),
        Light(Vec3d(-16, 16, 0), SPGL::Color(64, 16, 255), 320),
        Light(Vec3d(0, 16, -16), SPGL::Color(255, 255, 64), 240)
    };

    Scene scene(sdf, lights, SPGL::Image(WIDTH, HEIGHT));


    FloatT t = 1.5;
    while(window.isRunning()) {
        t += 0.2;
        scene.camera.setFov(75);
        scene.camera.setPos(Vec3d(16*std::cos(t), 10, 16*std::sin(t)));

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
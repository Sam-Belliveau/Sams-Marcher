#include <iostream>
#include <string>
#include "camera.hpp"

using namespace sb;


const std::size_t width = 80;
const std::size_t height = 80;

int march(SDF sdf, Ray r) {
    const double EPS = 1e-3;

    int hits = 0;
    double step = 0;

    for(int i = 0; i < 256 && hits < 9; ++i) {
        step = sdf(r.pos());
        
        if(step < EPS) {
            hits += 1;
            r = r.reflect(sdf).step(sdf).step(sdf).step(sdf);
        } else {
            r = r.step(step);
        }
    }

    return hits;
}

int main() {

    SDF scene = Vec3d(1, 2, 1) * (SDF::Sphere(1) | (SDF::Sphere(1) + Vec3d(0, 1, 0)));

    Camera cam(width, height);

    for(int y = 0; y < height; ++y) {
        for(int x = 0; x < width; ++x) {
            std::cout << march(scene, cam(x, y));
        }   
        std::cout << '\n';
    }


    std::cout << std::endl;
    

}
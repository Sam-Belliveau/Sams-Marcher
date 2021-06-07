#ifndef SAM_B_SCENE_HPP
#define SAM_B_SCENE_HPP 1

#include "SPGL/SPGL/SPGL.hpp"
#include "camera.hpp"
#include "light.hpp"
#include "sdf.hpp"

#include <vector>

namespace sb {

    class Scene {
    public: // Variables
        SDF scene;
        std::vector<Light> lights;
        SPGL::Image image;
        Camera camera;
    
    public: // Constructor
        Scene(const SDF& scene, const std::vector<Light>& lights, const SPGL::Image& image) 
            : scene{scene}, lights{lights}, image{image}, camera{Camera(image.width(), image.height())} {}

    public: // Functions
        SPGL::Color getPixel(SPGL::Size x, SPGL::Size y) const {
            SPGL::Color out = SPGL::Color::Black;

            Ray ray = Ray(camera(x, y));

            int hits = 0;
            double distance = 0;
            double step = 0;

            for(int i = 0; i < MAX_MARCH_ITER; ++i) {
                step = scene(ray.pos());
                distance += step;
                
                if(MAX_STEP < step) {
                    break;
                }

                if(MAX_HITS < hits) {
                    break;
                }

                if(step < EPS) {
                    double darkness = std::pow(REFLECTIVE_DAMPENING, hits - 1);

                    for(const Light& l : lights) {
                        out += l.getColor(scene, ray) / darkness;
                    }

                    ray = ray.reflect(scene, FIXING_RATIO * EPS);
                    hits += 1;
                } else {
                    ray = ray.step(step);
                }
            }

            return out;
        }

        void updatePixel(SPGL::Size x, SPGL::Size y) {
            image(x, y) = getPixel(x, y);
        }

        void updatePixel(SPGL::Size i) {
            updatePixel(i % image.width(), i / image.width());
        }
    };

}

#endif
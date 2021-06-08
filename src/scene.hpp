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

    private: // Helper Functions
        SPGL::Color march(Ray ray, std::size_t hits) const {
            
            for(int i = 0; i < MAX_MARCH_ITER; ++i) {
                double step = scene(ray.pos());
                
                if(MAX_STEP < step) {
                    break; 
                }

                if(step < EPS) {
                    SPGL::Color out = SPGL::Color::Black;
                    
                    for(const auto& l : lights) 
                        out += l.getColor(scene, ray);
                    
                    if(0 < hits) {
                        out += march(ray.reflect(scene, i * FIXING_RATIO * EPS), hits - 1) / REFLECTIVE_DAMPENING;
                    }

                    return out; 
                }

                ray = ray.step(step);
            }

            return 0.25 * SPGL::Color::Cyan;
        }

    public: // Functions
        SPGL::Color getPixel(SPGL::Size x, SPGL::Size y) const {
            return march(Ray(camera(x, y)), MAX_HITS);
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
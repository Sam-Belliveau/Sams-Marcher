#ifndef SAM_B_LIGHT_HPP
#define SAM_B_LIGHT_HPP 1

#include "SPGL/SPGL/SPGL.hpp"

#include "constants.hpp"
#include "vec3.hpp"
#include "sdf.hpp"
#include "ray.hpp"

namespace sb {

    class Material {
    public:
        // Specularity
        const double k_s;
        
        // Diffusion
        const double k_d;

        // Ambiance 
        const double k_a;

        // Specularity Amount
        const double a;

    public:
        Material(double k_s, double k_d, double k_a, double a)
            : k_s{k_s}, k_d{k_d}, k_a{k_a}, a{a} {}
    
    };

    const Material DEFAULT_MATERIAL = Material(4, 1, 0.1, 96);
    
    class Light {
    private:
        Vec3d _pos;
        SPGL::Color _color;
        double _bright;

    public: // Constructor
        Light(const Vec3d& pos, const SPGL::Color& color = SPGL::Color(224,224,64), double bright=1.0)
        : _pos{pos}, _color{color}, _bright{bright} {}

    private: // Helper Functions
        bool getDirectLight(const SDF& sdf, const Vec3d& pos) const {
            // Get ray from point towards light
            Ray ray = Ray(pos, _pos - pos).fix(sdf, FIXING_RATIO * LIGHTING_EPS);

            for(int i = 0; i < MAX_MARCH_ITER_LIGHTING; ++i) {
                double distance = (_pos - ray.pos()).mag();
                double step = sdf(ray.pos());
                
                // If you hit something, there is no light
                if(step < LIGHTING_EPS) return false;

                // If the light is closer to you than the scene, there is light
                if(distance < step) return true;

                // If you do not know, just step
                ray = ray.step(step);
            }

            return false;
        }

    public: // Functions
        SPGL::Color getColor(const SDF& sdf, const Ray& ray, const Material& mat = DEFAULT_MATERIAL) const {

            // Relative Position / Distance
            const Vec3d rel_pos = _pos - ray.pos();
            const double dist = rel_pos.mag();
            const double dist_sqr = dist * dist;

            // Ambient Lighting
            double brightness = mat.k_a;

            // If there is direct light, do some more lighting
            if(getDirectLight(sdf, ray.pos())) {
                const Vec3d normal = sdf.normal(ray.pos());
                const Vec3d light_dir = Ray(ray.pos(), -rel_pos).reflect(sdf).dir();

                brightness += mat.k_d * std::max(0.0, (normal.dot(rel_pos.norm())));
                brightness += mat.k_s * std::pow(std::max(0.0, -(ray.dir().dot(light_dir))), mat.a);
            }

            // Return Color Multiplied by Brightness
            return (_bright * brightness / dist_sqr) * _color;
        }
    };

}

#endif

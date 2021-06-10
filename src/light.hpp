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
        const FloatT k_s;
        
        // Diffusion
        const FloatT k_d;

        // Ambiance 
        const FloatT k_a;

        // Specularity Amount
        const FloatT a;

    public:
        constexpr Material(FloatT k_s, FloatT k_d, FloatT k_a, FloatT a)
            : k_s{k_s}, k_d{k_d}, k_a{k_a}, a{a} {}
    
    };

    constexpr Material DEFAULT_MATERIAL = Material(0.3, 1, 0.00, 96);
    
    FloatT fresnel(const FloatT& ks, const Vec3d& h, const Vec3d& l) {
        return ks + (FloatT(1.0) - ks) * std::pow(std::clamp(FloatT(1.0) - h.dot(l), FloatT(0.0), FloatT(1.0)), 5);
    }

    class Light {
    private:
        Vec3d _pos;
        SPGL::Color _color;
        FloatT _bright;

    public: // Constructor
        constexpr Light(const Vec3d& pos, const SPGL::Color& color = SPGL::Color(224,224,64), FloatT bright=1.0)
        : _pos{pos}, _color{color}, _bright{bright} {}

    private: // Helper Functions
        bool getDirectLight(const SDF& sdf, const Vec3d& pos) const {
            // Get ray from point towards light
            Ray ray = Ray(pos, _pos - pos).fix(sdf, FIXING_RATIO * LIGHTING_EPS);

            for(int i = 0; i < MAX_MARCH_ITER_LIGHTING; ++i) {
                FloatT distance = (_pos - ray.pos()).mag();
                FloatT step = sdf(ray.pos());
                
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
            const FloatT dist = rel_pos.mag();
            const FloatT dist_sqr = dist * dist;

            // Ambient Lighting
            FloatT brightness = mat.k_a;

            // If there is direct light, do some more lighting
            if(getDirectLight(sdf, ray.pos())) {
                const Vec3d normal = sdf.normal(ray.pos());
                const Vec3d light_dir = Ray(ray.pos(), -rel_pos).reflect(sdf).dir();

                const Vec3d h = rel_pos.norm();
                const Vec3d l = (h - ray.dir()).norm();

                const FloatT f = fresnel(mat.k_s, l, h);

                brightness += (FloatT(1.0) - f) * mat.k_d * std::max(FloatT(0), (normal.dot(rel_pos.norm())));
                brightness += (FloatT(0.0) + f) * std::pow(std::max(FloatT(0), -(ray.dir().dot(light_dir))), mat.a);
            }

            // Return Color Multiplied by Brightness
            return (_bright * brightness / dist_sqr) * _color;
        }
    };

}

#endif

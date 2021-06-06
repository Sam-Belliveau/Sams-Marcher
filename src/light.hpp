#ifndef SAM_B_LIGHT_HPP
#define SAM_B_LIGHT_HPP 1

#include "SPGL/SPGL/SPGL.hpp"

#include "vec3.hpp"
#include "sdf.hpp"
#include "ray.hpp"

namespace sb {

    class Material {
    public:
        const double k_s, k_d, k_a, a;

    public:
        Material(double k_s, double k_d, double k_a, double a)
            : k_s{k_s}, k_d{k_d}, k_a{k_a}, a{a} {}
    
    public:
        static const Material DEFAULT(0.2, 0.4, 0.4, 2);
    }

    class Light {
    private:
        SPGL::Color _color;
        Vec3d _pos;
        double _bright;

    public: // Constructor
        Light(const Vec3d& pos, const SPGL::Color& color = SPGL::Color(255,255,0), bright=1.0)
        : _pos{pos}, _color{color}, _bright{bright} {}


    public: // Functions
        SPGL::Color getColor(const SDF& sdf, const Ray& ray, const Material& mat = Material::DEFAULT) {
            const Vec3d normal = sdf.normal(ray.pos());
            const vec3d rel_pos = ray.pos() - _pos;

            double brightness = mat.k_a;
            brightness = mat.k_d * (normal.dot(rel_pos));

            double r = std::min(255.0, std::max(0.0, (_color.r * brightness)));
            double g = std::min(255.0, std::max(0.0, (_color.r * brightness)));
            double b = std::min(255.0, std::max(0.0, (_color.r * brightness)));
            
            return SPGL::Color(SPGL::UInt8(r), SPGL::UInt8(g), SPGL::UInt8(b));

        }
    }

}

#endif

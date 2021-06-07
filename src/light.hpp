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
        const double k_s, k_d, k_a, a;

    public:
        Material(double k_s, double k_d, double k_a, double a)
            : k_s{k_s}, k_d{k_d}, k_a{k_a}, a{a} {}
    
    };

    const Material DEFAULT_MATERIAL = Material(4, 1, 0.1, 48);
    
    class Light {
    private:
        Vec3d _pos;
        SPGL::Color _color;
        double _bright;

    public: // Constructor
        Light(const Vec3d& pos, const SPGL::Color& color = SPGL::Color(224,224,64), double bright=1.0)
        : _pos{pos}, _color{color}, _bright{bright} {}

    private: // Helper Functions
        double getMarchClearance(const SDF& sdf, const Vec3d& pos) const {
            SDF lightSDF([=](const Vec3d& pos){ return (_pos - pos).mag(); });
            Ray ray = Ray(pos, _pos - pos).fix(sdf, FIXING_RATIO * LIGHTING_EPS);

            for(int i = 0; i < MAX_MARCH_ITER_LIGHTING; ++i) {
                double step = sdf(ray.pos());

                if(step < LIGHTING_EPS) {
                    return 0.0;
                }

                if(lightSDF(ray.pos()) < step) {
                    break;
                }

                ray = ray.step(step);
            }

            return 1.0;
        }

    public: // Functions
        SPGL::Color getColor(const SDF& sdf, const Ray& ray, const Material& mat = DEFAULT_MATERIAL) const {
            const Vec3d ray_pos = ray.pos();
            const Vec3d ray_dir = ray.dir();
            const Vec3d normal = sdf.normal(ray_pos);
            const Vec3d rel_pos = _pos - ray_pos;
            const Ray light_ray = Ray(ray_pos, -rel_pos);
            const Vec3d light_dir = light_ray.reflect(sdf).dir();
            const double dist = rel_pos.mag();

            const double clear = getMarchClearance(sdf, ray_pos);
            double brightness = mat.k_a;
            brightness += clear * mat.k_d * std::max(0.0, (normal.dot(rel_pos.norm())));
            brightness += clear * mat.k_s * std::pow(std::max(0.0, ((-ray_dir).dot(light_dir))), mat.a);

            return (_bright * brightness / (dist * dist)) * _color;
        }
    };

}

#endif

#ifndef SAM_B_RAY_HPP 
#define SAM_B_RAY_HPP 1

#include "sdf.hpp"
#include "vec3.hpp"

namespace sb {

    class Ray {

    private: // Variables
        Vec3d _pos;
        Vec3d _dir;

    public: // Constructors 
        constexpr Ray(const Vec3d& pos, const Vec3d& dir) : _pos{pos}, _dir{dir.norm()} {}
        constexpr Ray(const Ray&) = default;
        constexpr Ray& operator=(const Ray&) = default;

    public: // Getters
        constexpr const Vec3d pos() const {
            return _pos;
        }
        
        constexpr const Vec3d dir() const {
            return _dir;
        }
        
    public: // Functions
        constexpr Ray step(FloatT distance) const {
            const Vec3d d = dir();
            return Ray(pos() + distance * d, d);
        }

        Ray step(SDF sdf) const {
            return step(sdf(pos()));
        }

        Ray reflect(SDF sdf, FloatT eps = 0.0) const {
            const Vec3d d = dir();
            const Vec3d n = sdf.normal(pos());
            return Ray(pos() + eps * n, d - 2 * n * (d.dot(n)));
        }

        Ray fix(SDF sdf, FloatT eps) {
            const Vec3d n = sdf.normal(pos());
            return Ray(pos() + eps * n, dir());
        }
    };

}

#endif
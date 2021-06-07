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
        Ray(const Vec3d& pos, const Vec3d& dir) : _pos{pos}, _dir{dir.norm()} {}
        Ray(const Ray&) = default;
        Ray& operator=(const Ray&) = default;

    public: // Getters
        const Vec3d pos() const {
            return _pos;
        }
        
        const Vec3d dir() const {
            return _dir.norm();
        }
        
    public: // Functions
        Ray step(double distance) const {
            const Vec3d d = dir();
            return Ray(pos() + distance * d, d);
        }

        Ray step(SDF sdf) const {
            return step(sdf(pos()));
        }

        Ray reflect(SDF sdf) const {
            const Vec3d d = dir();
            const Vec3d n = sdf.normal(pos());
            return Ray(pos(), d - 2 * n * (d.dot(n)));
        }

        Ray fix(SDF sdf, double eps) {
            const Vec3d n = sdf.normal(pos());
            return Ray(pos() + eps * n, dir());
        }
    };

}

#endif
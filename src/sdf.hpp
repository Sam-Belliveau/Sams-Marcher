#ifndef SAM_B_SDF_HPP
#define SAM_B_SDF_HPP 1

#include <cmath>
#include <functional>
#include "vec3.hpp"

namespace sb {

    using SDFBase = std::function<double(const Vec3d&)>;


    class SDF {
    public: // Static Constructors
        static SDF Sphere(const double radius) {
            return SDF([=](const Vec3d& pos) {
                return pos.mag() - radius;
            });
        }

        static SDF Box(const Vec3d size) {
            return SDF([=](const Vec3d& pos) {
                Vec3d q = pos.abs() - size;
                double a = Vec3d(std::max(q.x, 0.0), std::max(q.y, 0.0), std::max(q.z, 0.0)).mag();
                double b = std::min(0.0, std::max(q.x, std::max(q.y, q.z)));
                return a + b;
            });
        }

        static SDF Plane(const Vec3d normal, const double h) {
            const Vec3d normed_normal = normal.norm();
            return SDF([=](const Vec3d& pos) {
                return normed_normal.dot(pos) + h;
            });
        }

    private: // Variables
        SDFBase _func;

    public: // Constructors
        SDF(const SDFBase& func) : _func{func} {}

        SDF(const SDF&) = default;
        SDF& operator=(const SDF&) = default;

    public: // Operator Overloading (Passthrough)
        double operator()(const Vec3d& pos) const {
            return _func(pos);
        }
    
    public: // Functions
        Vec3d normal(const Vec3d& pos) const {
            static const Vec3d xyy(+1, -1, -1);
            static const Vec3d yyx(-1, -1, +1);
            static const Vec3d yxy(-1, +1, -1);
            static const Vec3d xxx(+1, +1, +1);
            
            return (
                xyy * operator()(pos + NORM_EPS * xyy) += 
                yyx * operator()(pos + NORM_EPS * yyx) += 
                yxy * operator()(pos + NORM_EPS * yxy) += 
                xxx * operator()(pos + NORM_EPS * xxx)).normalize();
        }

    public: // Operator Overloading (Construction)

        // Invert the shape
        friend SDF operator~(const SDF& a) {
            return SDF([=](const Vec3d& pos) {
                return -a(pos);
            });
        }

        // Union two shapes together
        friend SDF operator|(const SDF& lhs, const SDF& rhs) {
            return SDF([=](const Vec3d& pos) {
                return std::min(lhs(pos), rhs(pos));
            });
        }

        // Get the intersection of two shapes
        friend SDF operator&(const SDF& lhs, const SDF& rhs) {
            return SDF([=](const Vec3d& pos) {
                return std::max(lhs(pos), rhs(pos));
            });
        }

        // Subtract one shape from another
        friend SDF operator-(const SDF& lhs, const SDF& rhs) {
            return SDF([=](const Vec3d& pos) {
                return std::max(lhs(pos), -rhs(pos));
            });
        }
        
        // Add to the position of a shape
        friend SDF operator+(const Vec3d& lhs, const SDF& rhs) { return rhs + lhs; }
        friend SDF operator+(const SDF& lhs, const Vec3d& rhs) {
            return SDF([=](const Vec3d& pos) {
                return lhs(pos - rhs);
            });
        }

        // Add radius to the SDF function
        friend SDF operator+(const double lhs, const SDF& rhs) { return rhs + lhs; }
        friend SDF operator+(const SDF& lhs, const double rhs) {
            return SDF([=](const Vec3d& pos) {
                return lhs(pos) - rhs;
            });
        }

        // Subtract position from the shape
        friend SDF operator-(const SDF& lhs, const Vec3d& rhs) {
            return SDF([=](const Vec3d& pos) {
                return lhs(pos + rhs);
            });
        }

        // Remove radius from the SDF function
        friend SDF operator-(const SDF& lhs, const double rhs) {
            return SDF([=](const Vec3d& pos) {
                return lhs(pos) + rhs;
            });
        }

        // Stretch Shape by a certain vector
        friend SDF operator*(const Vec3d& lhs, const SDF& rhs) { return rhs * lhs; }
        friend SDF operator*(const SDF& lhs, const Vec3d& rhs) {
            return SDF([=](const Vec3d& pos) {
                return lhs(pos / rhs) * std::min(rhs.x, std::min(rhs.y, rhs.z));
            });
        }

        // Scale a Shape
        friend SDF operator*(const double& lhs, const SDF& rhs) { return rhs * lhs; }
        friend SDF operator*(const SDF& lhs, const double& rhs) {
            return SDF([=](const Vec3d& pos) {
                return lhs(pos / rhs) * rhs;
            });
        }
    };

}

#endif
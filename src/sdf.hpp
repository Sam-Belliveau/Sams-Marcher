#ifndef SAM_B_SDF_HPP
#define SAM_B_SDF_HPP 1

#include <cmath>
#include <functional>
#include "vec3.hpp"
#include "mat3.hpp"

namespace sb {

    using SDFBase = std::function<FloatT(const Vec3d&)>;


    class SDF {
    public: // Static Constructors
        static SDF Sphere(const FloatT radius) {
            return SDF([=](const Vec3d& pos) {
                return pos.mag() - radius;
            });
        }

        static SDF Box(const Vec3d size) {
            return SDF([=](const Vec3d& pos) {
                Vec3d q = pos.abs() - size;
                FloatT a = Vec3d(std::max(q.x, FloatT(0)), std::max(q.y, FloatT(0)), std::max(q.z, FloatT(0))).mag();
                FloatT b = std::min(FloatT(0), std::max(q.x, std::max(q.y, q.z)));
                return a + b;
            });
        }

        static SDF Plane(const Vec3d normal, const FloatT h) {
            const Vec3d normed_normal = normal.norm();
            return SDF([=](const Vec3d& pos) {
                return normed_normal.dot(pos) + h;
            });
        }

        static SDF Cylinder(const FloatT radius) {
            return SDF([=](const Vec3d& pos) {
                return std::hypot(pos.x, pos.y) - radius;
            });
        }

    private: // Variables
        SDFBase _func;

    public: // Constructors
        SDF(const SDFBase& func) : _func{func} {}

        SDF(const SDF&) = default;
        SDF& operator=(const SDF&) = default;

    public: // Operator Overloading (Passthrough)
        FloatT operator()(const Vec3d& pos) const {
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

        // Subtract position from the shape
        friend SDF operator-(const SDF& lhs, const Vec3d& rhs) {
            return SDF([=](const Vec3d& pos) {
                return lhs(pos + rhs);
            });
        }

        friend SDF operator*(const Mat3d& lhs, const SDF& rhs) {
            const Mat3d mat = lhs.inv();
            const FloatT x_dist = std::sqrt(mat[0][0] * mat[0][0] + mat[1][0] * mat[1][0] + mat[2][0] * mat[2][0]);
            const FloatT y_dist = std::sqrt(mat[0][1] * mat[0][1] + mat[1][1] * mat[1][1] + mat[2][1] * mat[2][1]);
            const FloatT z_dist = std::sqrt(mat[0][2] * mat[0][2] + mat[1][2] * mat[1][2] + mat[2][2] * mat[2][2]);
            const FloatT max_dist = std::max(x_dist, std::max(y_dist, z_dist));
            return SDF([=](const Vec3d& pos) {
                return rhs(mat * pos) / max_dist;
            });
        }

        // Add radius to the SDF function
        friend SDF operator+(const FloatT lhs, const SDF& rhs) { return rhs + lhs; }
        friend SDF operator+(const SDF& lhs, const FloatT rhs) {
            return SDF([=](const Vec3d& pos) {
                return lhs(pos) - rhs;
            });
        }

        // Remove radius from the SDF function
        friend SDF operator-(const SDF& lhs, const FloatT rhs) {
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
        friend SDF operator*(const FloatT& lhs, const SDF& rhs) { return rhs * lhs; }
        friend SDF operator*(const SDF& lhs, const FloatT& rhs) {
            return SDF([=](const Vec3d& pos) {
                return lhs(pos / rhs) * rhs;
            });
        }
    };

}

#endif
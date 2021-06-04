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
        Vec3d normal(const Vec3d& pos, const double eps = 1e-3) const {
            const double dist = operator()(pos);
            return Vec3d(
                operator()(pos + Vec3d(eps, 0, 0)) - operator()(pos - Vec3d(eps, 0, 0)),
                operator()(pos + Vec3d(0, eps, 0)) - operator()(pos - Vec3d(0, eps, 0)),
                operator()(pos + Vec3d(0, 0, eps)) - operator()(pos - Vec3d(0, 0, eps))
            ).normalize();
        }

    public: // Operator Overloading (Construction)

        // Constructing SDFs
        SDF operator~() const {
            return SDF([=](const Vec3d& pos) {
                return -operator()(pos);
            });
        }

        friend SDF operator|(const SDF& lhs, const SDF& rhs) {
            return SDF([=](const Vec3d& pos) {
                return std::min(lhs(pos), rhs(pos));
            });
        }

        friend SDF operator&(const SDF& lhs, const SDF& rhs) {
            return SDF([=](const Vec3d& pos) {
                return std::max(lhs(pos), rhs(pos));
            });
        }

        friend SDF operator^(const SDF& lhs, const SDF& rhs) {
            return lhs & ~rhs;
        }
        
        // Shifting SDFs Around
        friend SDF operator+(const SDF& lhs, const Vec3d& rhs) {
            return SDF([=](const Vec3d& pos) {
                return lhs(pos - rhs);
            });
        }

        friend SDF operator+(const Vec3d& lhs, const SDF& rhs) {
            return rhs + lhs;
        }

        friend SDF operator-(const SDF& lhs, const Vec3d& rhs) {
            return SDF([=](const Vec3d& pos) {
                return lhs(pos + rhs);
            });
        }

        friend SDF operator*(const SDF& lhs, const Vec3d& rhs) {
            return SDF([=](const Vec3d& pos) {
                return lhs(pos / rhs) * std::min(rhs.x, std::min(rhs.y, rhs.z));
            });
        }

        friend SDF operator*(const Vec3d& lhs, const SDF& rhs) {
            return rhs * lhs;
        }

        friend SDF operator*(const SDF& lhs, const double& rhs) {
            return SDF([=](const Vec3d& pos) {
                return lhs(pos / rhs) * rhs;
            });
        }

        friend SDF operator*(const double& lhs, const SDF& rhs) {
            return rhs * lhs;
        }
    };

}

#endif
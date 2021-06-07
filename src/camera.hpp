#ifndef SAM_B_CAMERA_HPP
#define SAM_B_CAMERA_HPP 1

#include <cmath>
#include <cstdint>

#include "ray.hpp"
#include "vec3.hpp"
#include "sdf.hpp"

namespace sb {

    class Camera {

    private: // Variables
        Vec3d _pos;
        Vec3d _dir;
        
        double _fov_mul;
        std::size_t _width, _height;

    public: // Constructors
        Camera(const std::size_t width, const std::size_t height, double fov = 90.0) {
            _width = width;
            _height = height;

            setFov(fov);
            setPos(Vec3d(1,0,0));
        }
    
    public: // Functions
        void setFov(double fov) {
            _fov_mul = std::tan(3.1416 * fov / 360.0);
        }

        void setPos(Vec3d pos) {
            _pos = pos;
            _dir = (-pos).norm();
        }

        Ray operator()(const std::size_t x, const std::size_t y) const {
            const Vec3d x_dir = Vec3d(_dir.z, 0, -_dir.x).normalize();
            const Vec3d y_dir = Vec3d(_dir.x*_dir.y, _dir.y - std::hypot(_dir.x, _dir.z), _dir.z*_dir.y).normalize(); 

            double dx = _fov_mul * 2.0 * ((double(x) / double(_width)) - 0.5) * double(_width) / double(_height);
            double dy = _fov_mul * 2.0 * ((double(y) / double(_height)) - 0.5);

            
            return Ray(
                _pos, (_dir + dx * x_dir + dy * y_dir).norm()
            );
        }

    };

}

#endif

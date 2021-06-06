#ifndef SAM_B_CAMERA_HPP
#define SAM_B_CAMERA_HPP 1

#include <cstdint>

#include "ray.hpp"
#include "vec3.hpp"
#include "sdf.hpp"


namespace sb {

    class Camera {

    private: // Variables
        Vec3d _pos;
        Vec3d _dir;
        
        std::size_t _width, _height;

    public: // Constructors
        Camera(const std::size_t width, const std::size_t height) {
            _width = width;
            _height = height;

            _pos = Vec3d(0, 0, -5);
            _dir = Vec3d(0, 0, 1).normalize();
        }
    
    public: // Functions
        void set_pos(Vec3d pos) {
            _pos = pos;
            _dir = (-pos).norm();
        }

        Ray operator()(const std::size_t x, const std::size_t y) {
            _dir.normalize();
            const Vec3d x_dir = Vec3d(_dir.z, 0, -_dir.x).normalize();
            const Vec3d y_dir = Vec3d(0, _dir.z, -_dir.y).normalize(); 

            double dx = 2.0 * ((double(x) / double(_width)) - 0.5) * double(_width) / double(_height);
            double dy = 2.0 * ((double(y) / double(_height)) - 0.5);

            return Ray(
                _pos, (_dir + dx * x_dir + dy * y_dir).norm()
            );
        }

    };

}

#endif

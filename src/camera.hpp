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

            _pos = Vec3d(-5, 0, 0);
            _dir = Vec3d(1, 0, 0).normalize();
        }
    
    public: // Functions

        Ray operator()(const std::size_t x, const std::size_t y) {
            double dx = 2.0 * (double(x) / double(_width) - 0.5) * double(_width) / double(_height);
            double dy = 2.0 * (double(y) / double(_height) - 0.5);

            return Ray(
                _pos, _dir + Vec3d(0, dx, dy)
            );
        }

    };

}

#endif

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
            _fov_mul = std::tan(SPGL::Math::Pi * fov / 360.0);
        }

        void setPos(Vec3d pos) {
            _pos = pos;
            _dir = (-pos).norm();
        }

        Ray operator()(const std::size_t x, const std::size_t y) const {
            // These vectors are all at 90 degrees from eachother
            // We can prove this by taking the dot product between theme    
            //
            // DOT PRODUCT FOR X_DIR:
            // dot = dir.x * (dir.z) 
            //     + dir.y * (0) 
            //     + dir.z * (-dir.x) 
            //     ==================
            //       0
            //
            // DOT PRODUCT FOR Y_DIR:
            // dot = dir.x * (dir.y * dir.x) 
            //     + dir.y * (-(dir.z * dir.z + dir.x * dir.x)) 
            //     + dir.z * (dir.y * dir.z) 
            //     ==================
            //       0
            //
            // Because of this we know that our projection is being done correctly
            
            const Vec3d dir = _dir;
            const Vec3d x_dir = Vec3d( dir.z, 0, -dir.x ).norm();
            const Vec3d y_dir = Vec3d( dir.y * dir.x, -(dir.z * dir.z + dir.x * dir.x), dir.y * dir.z).norm(); 

            double dx = _fov_mul * 2.0 * ((double(x) / double(_width)) - 0.5) * double(_width) / double(_height);
            double dy = _fov_mul * 2.0 * ((double(y) / double(_height)) - 0.5);

            return Ray(
                _pos, (_dir + dx * x_dir + dy * y_dir).norm()
            );
        }

    };

}

#endif

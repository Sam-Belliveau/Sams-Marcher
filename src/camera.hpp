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
        std::size_t _width, _height;
        FloatT _fov_mul;

        Vec3d _pos;
        Vec3d _dir;
        
    public: // Constructors
        constexpr Camera(const std::size_t width, const std::size_t height, FloatT fov = 90.0) 
            : _width{width}
            , _height{height}
            , _fov_mul{std::tan(FloatT(SPGL::Math::Pi) * fov / 360.0)}
            , _pos{Vec3d(1, 1, 1)}
            , _dir{(-_pos).norm()} {}
    
    public: // Functions
        constexpr void setFov(FloatT fov) {
            _fov_mul = std::tan(SPGL::Math::Pi * fov / 360.0);
        }

        constexpr void setPos(Vec3d pos) {
            _pos = pos;
            _dir = (-pos).norm();
        }

        constexpr Ray operator()(const std::size_t x, const std::size_t y) const {
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

            FloatT dx = _fov_mul * 2.0 * ((FloatT(x) / FloatT(_width)) - 0.5) * FloatT(_width) / FloatT(_height);
            FloatT dy = _fov_mul * 2.0 * ((FloatT(y) / FloatT(_height)) - 0.5);

            return Ray(
                _pos, (_dir + dx * x_dir + dy * y_dir).norm()
            );
        }

    };

}

#endif

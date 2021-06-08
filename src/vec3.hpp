#ifndef SAM_B_VEC_3_HPP
#define SAM_B_VEC_3_HPP 1

#include <cmath>
#include <array>

namespace sb {

    template<typename T>
    class Vec3 {

    public: // Variables
        T x, y, z;

    public: // Constructors
        constexpr Vec3() : x(0), y(0), z(0) {}
        constexpr Vec3(T x, T y, T z) : x(x), y(y), z(z) {} 

        template<typename OT> 
        constexpr Vec3(const Vec3<OT>& other) : x(T(other.x)), y(T(other.y)), z(T(other.z)) {}
        
        template<typename OT>
        constexpr Vec3& operator=(const Vec3<OT>& other) {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        // Casting
        constexpr operator std::array<T, 3>() const {
            return std::array<T, 3>({x, y, z});
        }

    public: // Functions
        constexpr T mag() const {
            return std::sqrt(x*x + y*y + z*z);
        }

        constexpr T dot(const Vec3& rhs) const {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        constexpr Vec3 norm() const {
            return *this / mag();
        }

        constexpr Vec3& normalize() { 
            operator/=(mag());
            return *this;
        }

        constexpr Vec3 abs() const {
            return Vec3(std::abs(x), std::abs(y), std::abs(z));
        }

    public: // Operators
        constexpr Vec3& operator+=(const Vec3& rhs) {
            x += rhs.x; y += rhs.y; z += rhs.z;
            return *this;
        }

        constexpr Vec3& operator-=(const Vec3& rhs) {
            x -= rhs.x; y -= rhs.y; z -= rhs.z;
            return *this;
        }

        constexpr Vec3& operator*=(const Vec3& rhs) {
            x *= rhs.x; y *= rhs.y; z *= rhs.z;
            return *this;
        }

        constexpr Vec3& operator/=(const Vec3& rhs) {
            x /= rhs.x; y /= rhs.y; z /= rhs.z;
            return *this;
        }

        constexpr Vec3& operator*=(const T rhs) {
            x *= rhs; y *= rhs; z *= rhs;
            return *this;
        }

        constexpr Vec3& operator/=(const T rhs) {
            x /= rhs; y /= rhs; z /= rhs;
            return *this;
        }

        constexpr Vec3 operator-() const {
            return Vec3(-x, -y, -z);
        }

        constexpr friend Vec3 operator+(Vec3 lhs, const Vec3& rhs) {
            return lhs += rhs;
        }

        constexpr friend Vec3 operator-(Vec3 lhs, const Vec3& rhs) {
            return lhs -= rhs;
        }

        constexpr friend Vec3 operator*(Vec3 lhs, const Vec3& rhs) {
            return lhs *= rhs;
        }

        constexpr friend Vec3 operator/(Vec3 lhs, const Vec3& rhs) {
            return lhs /= rhs;
        }

        constexpr friend Vec3 operator*(Vec3 lhs, const T rhs) {
            return lhs *= rhs;
        }

        constexpr friend Vec3 operator*(const T lhs, Vec3 rhs) {
            return rhs *= lhs;
        }

        constexpr friend Vec3 operator/(Vec3 lhs, const T rhs) {
            return lhs /= rhs;
        }
    };

    using Vec3d = Vec3<FloatT>;
    using Vec3i = Vec3<int>;

}

#endif
#ifndef SAM_B_VEC_3_HPP
#define SAM_B_VEC_3_HPP 1

#include <cmath>

namespace sb {

    template<typename T>
    class Vec3 {

    public: // Variables
        T x, y, z;

    public: // Constructors
        Vec3() : x(0), y(0), z(0) {}
        Vec3(T x, T y, T z) : x(x), y(y), z(z) {} 

        template<typename OT> 
        Vec3(const Vec3<OT>& other) : x(T(other.x)), y(T(other.y)), z(T(other.z)) {}
        
        template<typename OT>
        Vec3& operator=(const Vec3<OT>& other) {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

    public: // Functions
        T mag() const {
            return std::sqrt(x*x + y*y + z*z);
        }

        T dot(const Vec3& rhs) const {
            return x * rhs.x + y * rhs.y + z * rhs.z;
        }

        Vec3 norm() const {
            return *this / mag();
        }

        Vec3& normalize() { 
            operator/=(mag());
            return *this;
        }

        Vec3 abs() const {
            return Vec3(std::abs(x), std::abs(y), std::abs(z));
        }

    public: // Operators
        Vec3& operator+=(const Vec3& rhs) {
            x += rhs.x; y += rhs.y; z += rhs.z;
            return *this;
        }

        Vec3& operator-=(const Vec3& rhs) {
            x -= rhs.x; y -= rhs.y; z -= rhs.z;
            return *this;
        }

        Vec3& operator*=(const Vec3& rhs) {
            x *= rhs.x; y *= rhs.y; z *= rhs.z;
            return *this;
        }

        Vec3& operator/=(const Vec3& rhs) {
            x /= rhs.x; y /= rhs.y; z /= rhs.z;
            return *this;
        }

        Vec3& operator*=(const T rhs) {
            x *= rhs; y *= rhs; z *= rhs;
            return *this;
        }

        Vec3& operator/=(const T rhs) {
            x /= rhs; y /= rhs; z /= rhs;
            return *this;
        }

        Vec3& operator-() {
            return Vec3(-x, -y, -z);
        }

        friend Vec3 operator+(Vec3 lhs, const Vec3& rhs) {
            return lhs += rhs;
        }

        friend Vec3 operator-(Vec3 lhs, const Vec3& rhs) {
            return lhs -= rhs;
        }

        friend Vec3 operator*(Vec3 lhs, const Vec3& rhs) {
            return lhs *= rhs;
        }

        friend Vec3 operator/(Vec3 lhs, const Vec3& rhs) {
            return lhs /= rhs;
        }

        friend Vec3 operator*(Vec3 lhs, const double rhs) {
            return lhs *= rhs;
        }

        friend Vec3 operator*(const double lhs, Vec3 rhs) {
            return rhs *= lhs;
        }

        friend Vec3 operator/(Vec3 lhs, const double rhs) {
            return lhs /= rhs;
        }
    };

    using Vec3d = Vec3<double>;
    using Vec3i = Vec3<int>;

}

#endif
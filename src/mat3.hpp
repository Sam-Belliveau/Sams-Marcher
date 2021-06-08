#ifndef SAM_B_MATRIX_HPP
#define SAM_B_MATRIX_HPP 1

#include "vec3.hpp"

#include <cmath>
#include <array>

namespace sb {

    template<typename T>
    class Mat3 {
    public: // Typedefs
        using InternalSubRep = std::array<T, 3>;
        using InternalRep = std::array<InternalSubRep, 3>;

    public: // Factories
        
        constexpr static Mat3 Identity() {
            return Mat3(
                {T(1),T(0),T(0)},
                {T(0),T(1),T(0)},
                {T(0),T(0),T(1)}
            );
        }

        constexpr static Mat3 Pitch(const T& t) {
            const T S = std::sin(t);
            const T C = std::cos(t);
            return Mat3{
                { +C, +0, +S },
                { +0, +1, +0 },
                { -S, +0, +C }
            };
        }

        constexpr static Mat3 Yaw(const T& t) {
            const T S = std::sin(t);
            const T C = std::cos(t);
            return Mat3{
                { +C, -S, +0 },
                { +S, +C, +0 },
                { +0, +0, +1 }
            };
        }

        constexpr static Mat3 Roll(const T& t) {
            const T S = std::sin(t);
            const T C = std::cos(t);
            return Mat3{
                { +1, +0, +0 },
                { +0, +C, -S },
                { +0, +S, +C }
            };
        }
 

    private: // Variables
        InternalRep _data;

    public: // Constructors
        constexpr Mat3(const InternalRep& data) : _data{data} {}
        constexpr Mat3(const InternalSubRep& a, const InternalSubRep& b, const InternalSubRep& c) 
            : _data{{a, b, c}} {}
        
        constexpr Mat3(const Mat3& other) = default;
        constexpr Mat3& operator=(const Mat3& other) = default;

    public: // Getters
        constexpr InternalSubRep& operator[](std::size_t column) {
            return _data[column];
        }

        constexpr const InternalSubRep& operator[](std::size_t column) const {
            return _data[column];
        }

        constexpr T& operator()(std::size_t column, std::size_t row) {
            return _data[column][row];
        }

        constexpr const T& operator()(std::size_t column, std::size_t row) const {
            return _data[column][row];
        }

    public: // Math Operators
        
        constexpr T det() const {
            return ( (_data[0][0] * (_data[1][1] * _data[2][2] - _data[1][2] * _data[2][1]))
                   + (_data[0][1] * (_data[1][2] * _data[2][0] - _data[1][0] * _data[2][2]))
                   + (_data[0][2] * (_data[1][0] * _data[2][1] - _data[1][1] * _data[2][0])));
        }

        constexpr Mat3 adj() const {
            InternalRep data = {};
            
            data[0][0] = +(_data[1][1] * _data[2][2] - _data[1][2] * _data[2][1]);
            data[1][0] = -(_data[0][1] * _data[2][2] - _data[0][2] * _data[2][1]);
            data[2][0] = +(_data[0][1] * _data[1][2] - _data[0][2] * _data[1][1]);

            data[0][1] = -(_data[1][0] * _data[2][2] - _data[1][2] * _data[2][0]);
            data[1][1] = +(_data[0][0] * _data[2][2] - _data[0][2] * _data[2][0]);
            data[2][1] = -(_data[0][0] * _data[1][2] - _data[0][2] * _data[1][0]);

            data[0][2] = +(_data[1][0] * _data[2][1] - _data[1][1] * _data[2][0]);
            data[1][2] = -(_data[0][0] * _data[2][1] - _data[0][1] * _data[2][0]);
            data[2][2] = +(_data[0][0] * _data[1][1] - _data[0][1] * _data[1][0]);

            return Mat3(data);
        }

        constexpr Mat3 inv() const {
            return adj() /= det();
        }

        constexpr friend Mat3 operator+(Mat3 lhs, const Mat3& rhs) { return lhs += rhs; }
        constexpr Mat3& operator+=(const Mat3& rhs) {
            _data[0][0] += rhs[0][0]; _data[0][1] += rhs[0][1]; _data[0][2] += rhs[0][2];
            _data[1][0] += rhs[1][0]; _data[1][1] += rhs[1][1]; _data[1][2] += rhs[1][2];
            _data[2][0] += rhs[2][0]; _data[2][1] += rhs[2][1]; _data[2][2] += rhs[2][2];
            return *this;
        }

        constexpr friend Mat3 operator*(Mat3 lhs, const T& rhs) { return lhs *= rhs; }
        constexpr friend Mat3 operator*(const T& lhs, Mat3 rhs) { return rhs *= lhs; }
        constexpr Mat3& operator*=(const T& rhs) {
            _data[0][0] *= rhs; _data[0][1] *= rhs; _data[0][2] *= rhs;
            _data[1][0] *= rhs; _data[1][1] *= rhs; _data[1][2] *= rhs;
            _data[2][0] *= rhs; _data[2][1] *= rhs; _data[2][2] *= rhs;
            return *this;
        }    
        
        constexpr friend Mat3 operator/(Mat3 lhs, const T& rhs) { return lhs /= rhs; }
        constexpr Mat3& operator/=(const T& rhs) {
            _data[0][0] /= rhs; _data[0][1] /= rhs; _data[0][2] /= rhs;
            _data[1][0] /= rhs; _data[1][1] /= rhs; _data[1][2] /= rhs;
            _data[2][0] /= rhs; _data[2][1] /= rhs; _data[2][2] /= rhs;
            return *this;
        }

        constexpr friend Mat3 operator*(const Mat3& lhs, const Mat3& rhs) {
            InternalRep data = {};

            data[0][0] = lhs[0][0] * rhs[0][0] + lhs[0][1] * rhs[1][0] + lhs[0][2] * rhs[2][0];
            data[0][1] = lhs[0][0] * rhs[0][1] + lhs[0][1] * rhs[1][1] + lhs[0][2] * rhs[2][1];
            data[0][2] = lhs[0][0] * rhs[0][2] + lhs[0][1] * rhs[1][2] + lhs[0][2] * rhs[2][2];

            data[1][0] = lhs[1][0] * rhs[0][0] + lhs[1][1] * rhs[1][0] + lhs[1][2] * rhs[2][0];
            data[1][1] = lhs[1][0] * rhs[0][1] + lhs[1][1] * rhs[1][1] + lhs[1][2] * rhs[2][1];
            data[1][2] = lhs[1][0] * rhs[0][2] + lhs[1][1] * rhs[1][2] + lhs[1][2] * rhs[2][2];

            data[2][0] = lhs[2][0] * rhs[0][0] + lhs[2][1] * rhs[1][0] + lhs[2][2] * rhs[2][0];
            data[2][1] = lhs[2][0] * rhs[0][1] + lhs[2][1] * rhs[1][1] + lhs[2][2] * rhs[2][1];
            data[2][2] = lhs[2][0] * rhs[0][2] + lhs[2][1] * rhs[1][2] + lhs[2][2] * rhs[2][2];

            return Mat3(data);
        }

        constexpr friend Vec3<T> operator*(const Mat3& lhs, const Vec3<T>& rhs) {
            const T x = lhs[0][0] * rhs.x + lhs[0][1] * rhs.y + lhs[0][2] * rhs.z;
            const T y = lhs[1][0] * rhs.x + lhs[1][1] * rhs.y + lhs[1][2] * rhs.z;
            const T z = lhs[2][0] * rhs.x + lhs[2][1] * rhs.y + lhs[2][2] * rhs.z;
            
            return Vec3<T>(x, y, z);
        }

    };

    using Mat3d = Mat3<FloatT>;
    using Mat3i = Mat3<int>;

}

#endif
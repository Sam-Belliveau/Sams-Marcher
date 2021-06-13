#ifndef SAM_B_CONSTANTS_HPP
#define SAM_B_CONSTANTS_HPP

#include "SPGL/SPGL/SPGL.hpp"

namespace sb {

    // Data type we are using for rendering
    using FloatT = double;

    // Max Number of Reflections
    constexpr int MAX_HITS = 4;

    // Max Number of Steps allowed when Marching
    constexpr int MAX_MARCH_ITER = 320;
    constexpr int MAX_MARCH_ITER_LIGHTING = 128;

    // The EPS allowed for different situations
    constexpr FloatT EPS = 1e-2;
    constexpr FloatT NORM_EPS = 1e-4;
    constexpr FloatT LIGHTING_EPS = 1e-2;

    // How far away ray should be placed when reflections happen
    constexpr FloatT FIXING_RATIO = 4;

    // The color of the sky
    constexpr SPGL::Color AMBIENT_COLOR = SPGL::Color(24, 16, 64);

    // Max amount of distance Ray Marching should go without assuming it will tend to infinity
    constexpr FloatT MAX_DISTANCE = 256;

    // Number of threads the CPU should use
    constexpr int THREADS = 24;

    // Resolution
    constexpr int WIDTH = 720;
    constexpr int HEIGHT = 480;

    constexpr int PIXELS = WIDTH*HEIGHT;
}

#endif
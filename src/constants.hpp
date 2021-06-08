#ifndef SAM_B_constexprANTS_HPP
#define SAM_B_constexprANTS_HPP

namespace sb {

    // Data type we are using for rendering
    using FloatT = float;

    // Max Number of Reflections
    constexpr int MAX_HITS = 2;

    // Max Number of Steps allowed when Marching
    constexpr int MAX_MARCH_ITER = 128;
    constexpr int MAX_MARCH_ITER_LIGHTING = 128;

    // The EPS allowed for different situations
    constexpr FloatT EPS = 1e-2;
    constexpr FloatT NORM_EPS = 1e-4;
    constexpr FloatT LIGHTING_EPS = 1e-2;

    // How far away ray should be placed when reflections happen
    constexpr FloatT FIXING_RATIO = 4;

    // How much darker the reflections should be
    constexpr FloatT REFLECTIVE_DAMPENING = 2.5;

    // Max amount of distance Ray Marching should go without assuming it will tend to infinity
    constexpr FloatT MAX_STEP = 32;

    // Number of threads the CPU should use
    constexpr int THREADS = 24;

    // Resolution
    constexpr int WIDTH = 1280;
    constexpr int HEIGHT = 720;

    constexpr int PIXELS = WIDTH*HEIGHT;
}

#endif
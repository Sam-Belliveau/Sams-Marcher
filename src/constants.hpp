#ifndef SAM_B_CONSTANTS_HPP
#define SAM_B_CONSTANTS_HPP

namespace sb {
    // Max Number of Reflections
    const int MAX_HITS = 4;

    // Max Number of Steps allowed when Marching
    const int MAX_MARCH_ITER = 128;
    const int MAX_MARCH_ITER_LIGHTING = 128;

    // The EPS allowed for different situations
    const double EPS = 1e-2;
    const double NORM_EPS = 1e-6;
    const double LIGHTING_EPS = 1e-2;

    // How far away ray should be placed when reflections happen
    const double FIXING_RATIO = 4;

    // How much darker the reflections should be
    const double REFLECTIVE_DAMPENING = 1.25;

    // Max amount of distance Ray Marching should go without assuming it will tend to infinity
    const double MAX_STEP = 32;

    // Number of threads the CPU should use
    const int THREADS = 24;

    // Resolution
    const int WIDTH = 1920;
    const int HEIGHT = 1080;

    const int PIXELS = WIDTH*HEIGHT;
}

#endif
#ifndef SAM_B_CONSTANTS_HPP
#define SAM_B_CONSTANTS_HPP

namespace sb {
    const int MAX_HITS = 2;
    const int MAX_MARCH_ITER = 96;
    const int MAX_MARCH_ITER_LIGHTING = 96;

    const double EPS = 1e-2;
    const double NORM_EPS = 1e-4;
    const double LIGHTING_EPS = 1e-2;

    const double FIXING_RATIO = 4;

    const double MAX_STEP = 64;

    const int THREADS = 16;

    const int WIDTH = 720;
    const int HEIGHT = 480;
}

#endif
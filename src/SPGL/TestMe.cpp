#include <iostream>
#include <complex>

#include "SPGL/SPGL.hpp"

int main()
{
    // Define Width and Height
    SPGL::Size width = 640;
    SPGL::Size height = 480;

    // Make Window and Image Buffer
    SPGL::Window<> win(width, height, "Mandelbrot Set");
    SPGL::Image img(width, height);
    
    for(SPGL::Size y = 0; y < height; ++y)
        for(SPGL::Size x = 0; x < width; ++x)
        {
            // Mandelbrot Algorithm
            const std::complex<double> C((x - width*0.75) / (width/4.0),
                                            (y - height*0.5) / (height/3.0));
            std::complex<double> Z(C);
            SPGL::UInt8 iter = 0;

            while(++iter < 255 && Z.real()*Z.real() + Z.imag()*Z.imag() < 4)
              Z = Z*Z + C;

            // Use built in HSV to RGB conversion to get colors
            if(iter != 255) img(x, y) = SPGL::Color::HSV(iter);
        }
    
    // While the window is open, render the image and check for events.
    while(win.isRunning()) 
    {
        win.renderImage(img); 
        win.update(); 
    }
}
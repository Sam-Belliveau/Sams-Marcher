#ifndef SPGL_WINDOW_HPP
#define SPGL_WINDOW_HPP 1

#include <SDL2/SDL.h> // Graphics

#include "TypeNames.hpp"
#include "Vector2D.hpp"
#include "Color.hpp"
#include "Image.hpp"

namespace SPGL // Definitions
{
    // Main window should be made first and destroyed last
    template<bool main_window = true>
    class Window
    {
    private: // non-copyable
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;
        Window(Window&&) = delete;
        Window& operator=(Window&&) = delete;

    public: /* Functions */
        /* Initalization */
        Window(Size x, Size y, const char* name = "SPGL Window")
        {
            if(main_window)
            { SDL_Init(SDL_INIT_VIDEO); }

            window = SDL_CreateWindow(name, SDL_WINDOWPOS_UNDEFINED,
                SDL_WINDOWPOS_UNDEFINED, x, y, 0);
            original_window_size = Vector2s(x, y);

            SDL_SetWindowResizable(window, SDL_TRUE);

            renderer = SDL_CreateRenderer(window, -1, 0);

            texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                SDL_TEXTUREACCESS_STATIC, x, y);

            image_size = Vector2s(x, y);

            running = true;
        }

        ~Window() { close(); }

        /* Running Stuff */
        void close()
        {
            if(running)
            {
                running = false;
                SDL_DestroyTexture(texture);
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);

                if(main_window)
                { SDL_Quit(); }
            }
        }

        bool isRunning() const
        { return running; }

        /* Events */
        void update()
        {
            if(!running) { return; }

            while(SDL_PollEvent(&event))
            {
                switch (event.type)
                {
                case SDL_QUIT:
                    close();
                    break;

                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT)
                        mouseDown = false;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT)
                        mouseDown = true;
                    break;

                case SDL_MOUSEMOTION:
                    mousePixel.x = event.motion.x;
                    mousePixel.y = event.motion.y;
                    break;
                }
            }
        }

        /* Information */
        bool isMouseDown() const { return mouseDown; }
        Vector2s getMousePixel() const { return mousePixel; }
        Vector2s getMouseImagePixel() const 
        { 
            auto window_size = getWindowSize();
            if(window_size.x == 0 || window_size.y == 0) { return Vector2s(0,0); }
            else { return (mousePixel*image_size)/window_size; }
        }

        Vector2s getWindowSize() const 
        { 
            Vector2D<int> temp;
            SDL_GetWindowSize(window, &temp.x, &temp.y);
            return temp; 
        }

        Vector2s getImageSize() const { return image_size; }

        /* Rendering */
        void renderImage(const Image &in)
        {
            if(!running) { return; }

            if(in.vecsize() != image_size) 
            {
                image_size = in.vecsize();
                SDL_DestroyTexture(texture);
                texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888,
                    SDL_TEXTUREACCESS_STATIC, image_size.x, image_size.y);

                SDL_RenderSetScale(renderer
                    , float(original_window_size.x)/float(image_size.x)
                    , float(original_window_size.y)/float(image_size.y));
            }

            SDL_RenderClear(renderer);
            SDL_UpdateTexture(texture, NULL, in.data(), image_size.x*4);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }

    private: /* Variables */
        bool running = false;
        bool mouseDown = false;
        Vector2s mousePixel;

        Vector2s original_window_size;
        Vector2s image_size;

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_Texture* texture;
        SDL_Event event;
    };
}


#endif // SPGL_WINDOW_HPP

#ifndef SPGL_IMAGE_HPP
#define SPGL_IMAGE_HPP 1

#include <vector> // std::vector
#include <algorithm> // std::copy
#include <stdexcept> // std::out_of_range
#include <iterator> // std::reverse_iterator

#include "TypeNames.hpp"
#include "Vector2D.hpp"
#include "Color.hpp"

namespace SPGL // Definitions
{
    class Image
    {
    public: /* Container Information */
        using value_type = Color;

        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;

        using reference = value_type&;
        using const_reference = const value_type&;

        using pointer = value_type*;
        using const_pointer = const value_type*;

        using iterator = std::vector<value_type>::iterator;
        using const_iterator = std::vector<value_type>::const_iterator;

        using reverse_iterator = std::vector<value_type>::reverse_iterator;
        using const_reverse_iterator = std::vector<value_type>::const_reverse_iterator;

    public: /* Information */
        Vector2D<Size> vecsize() const noexcept { return img_size; }
        Size height() const noexcept { return img_size.y; }
        Size width()  const noexcept { return img_size.x; }
        bool empty()  const noexcept { return (img_size.x | img_size.y) == 0; }
        Size size()   const noexcept { return img_size.x * img_size.y; }

        const value_type* data() const noexcept { return img_data.data(); }
        value_type* data() noexcept { return img_data.data(); }

    public: /* Constructors */
        // Default Constructor
        Image() noexcept {}

        // Copy/Move Constructors
        Image(const Image& in) = default;
        Image(Image&& in) = default;
        Image& operator=(const Image& in) = default;
        Image& operator=(Image&& in) = default;

        // Create Functions
        Image(Size x, Size y, Color color = Color(0,0,0)) { create(x, y, color); }
        Image& create(Size x, Size y, Color color = Color(0,0,0)) 
        {
            img_size = Vector2s(x, y);
            img_data.resize(x*y);
            std::fill(img_data.begin(), img_data.end(), color);
            return *this;
        }

    public: /* Functions */
        value_type& operator[](Size i) noexcept
        { return img_data[i]; }

        const value_type& operator[](Size i) const noexcept 
        { return img_data[i]; }

        value_type& operator()(Size i) noexcept 
        { return img_data[i]; }

        const value_type& operator()(Size i) const noexcept 
        { return img_data[i]; }

        value_type& operator()(Size inX, Size inY) noexcept 
        { return img_data[inY*width() + inX]; }

        const value_type& operator()(Size inX, Size inY) const noexcept 
        { return img_data[inY*width() + inX]; }

        value_type& getPixel(Size inX, Size inY)
        {
            if(inX > width())  throw std::out_of_range("X Too Large!");
            if(inY > height()) throw std::out_of_range("Y Too Large!");
            return img_data[inY*width() + inX];
        }

        const value_type& getPixel(Size inX, Size inY) const
        {
            if(inX > width())  throw std::out_of_range("X Too Large!");
            if(inY > height()) throw std::out_of_range("Y Too Large!");
            return img_data[inY*width() + inX];
        }

    private: /* Raw Data */
        std::vector<value_type> img_data;
        Vector2D<Size> img_size;

    public: /* Iterators */
        auto begin() noexcept { return std::begin(img_data); }
        auto begin() const noexcept { return std::cbegin(img_data); }
        auto cbegin() const noexcept { return std::cbegin(img_data); }

        auto end() noexcept { return std::end(img_data); }
        auto end() const noexcept { return std::cend(img_data); }
        auto cend() const noexcept { return std::cend(img_data); }

        auto rbegin() noexcept { return std::end(img_data); }
        auto rbegin() const noexcept { return std::cend(img_data); }
        auto crbegin() const noexcept { return std::cend(img_data); }

        auto rend() noexcept { return std::begin(img_data); }
        auto rend() const noexcept { return std::cbegin(img_data); }
        auto crend() const noexcept { return std::cbegin(img_data); }
    };
}

#endif // SPGL_IMAGE_HPP

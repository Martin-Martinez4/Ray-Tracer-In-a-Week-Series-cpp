#ifndef COLOR_H
#define COLOR_H

#include "interval.h"
#include "vec3.h"

using color = vec3;

inline double linear_to_gamma(double linear_component){
    if (linear_component > 0){
        return std::sqrt(linear_component);
    }

    return 0;
}

void write_color(std::ostream& out, const color& pixel_color){
    auto r = pixel_color.x();
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    // Replace NaN components with zero. To try to cover up surface acne
    if (r != r) r = 0.0;
    if (g != g) g = 0.0;
    if (b != b) b = 0.0;


    // apply a linear to gamma transform
    r = linear_to_gamma(r);
    g = linear_to_gamma(g);
    b = linear_to_gamma(b);

    // translate the [0,1] component values to the range [0,255].  
    static const interval intensity(0.000, 0.999);
    int rbyte = int(255.99 * intensity.clamp(r));
    int gbyte = int(255.99 * intensity.clamp(g));
    int bbyte = int(255.99 * intensity.clamp(b));

    // write out the pixel color components.  
    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}

#endif
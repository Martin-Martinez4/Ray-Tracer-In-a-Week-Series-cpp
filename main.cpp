
#include "color.h"
#include "ray.h"
#include "vec3.h"

#include <iostream>

// color ray_color(const ray& r){
//     return color(0, 0, 0);
// }

// color ray_color_lerp(const ray& r){
//     // blendedValue=(1−a)⋅startValue+a⋅endValue,
//     vec3 unit_direction = unit_vector(r.direction());
//     auto a = 0.5 * (unit_direction.y() + 1.0);
//     return (1.0-a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
// }

double hit_sphere(const point3& center, double radius, const ray& r){
    vec3 oc = center - r.origin();
    // double a = dot(r.direction(), r.direction());
    // double b = -2.0 * dot(r.direction(), oc);
    // double c = dot(oc, oc) - radius*radius;
    // double discriminant = b*b - 4*a*c;

    double a = r.direction().length_squared();
    double h = dot(r.direction(), oc);
    double c = oc.length_squared() - radius*radius;
    double discriminant = h*h - a*c;
    
    if(discriminant < 0){
        return -1.0;
    }else{
        return (h - std::sqrt(discriminant) ) / a;
    }
}

color ray_color(const ray& r){

    double t = hit_sphere(point3(0,0,-1), 0.5, r);

    if(t > 0.0){
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5*color(N.x()+1, N.y()+1, N.z()+1);
    }

    vec3 unit_direction = unit_vector(r.direction());
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}


int main(){

    auto aspect_ratio = 16.0 / 9.0;
    int image_width = 400;

    int image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    auto focal_length = 1.0;
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width) / image_height);
    auto camera_center = point3(0,0,0);

    // calculate the vectors around the horizontal and down the vertical viewport edges
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // calculate the horizontal and vertical delta vectors from pixel to pixel.  
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calculate the location of the upper left pixel.
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u/2 - viewport_v/2;
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    std::cout << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int h = 0; h < image_height; ++h){
        std::clog << "\rScanlines remaining : " << (image_height - h -1) << ' '<< std::flush;
        for (int w = 0; w < image_width; ++w){
            auto pixel_center = pixel00_loc + (w * pixel_delta_u) + (h * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center;
            ray r(camera_center, ray_direction);

            color pixel_color = ray_color(r);
            write_color(std::cout, pixel_color);
        }
    }
    std::clog << std::flush;
    std::clog << "\n\rDone.\n";

}
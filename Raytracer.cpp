// Raytracer.cpp : Defines the entry point for the application.
//

#include "Raytracer.h"
#include "vec3.h"
#include "colour.h"
#include "ray.h"
#include <iostream>



bool hit_sphere(const point3& centre, double radius, const ray& r) {
	double a = dot(r.direction(), r.direction());
	double b = dot(-2 * r.direction(), centre - r.origin());
	double c = dot(centre - r.origin(), centre - r.origin()) - pow(radius, 2);
	double discriminant = pow(b, 2) - 4 * a * c;
	return (discriminant >= 0);
}

colour ray_colour(const ray& r) {
	if (hit_sphere(point3(0, 0, -1), 0.5, r)) {
		return colour(1, 0, 0);
	}
	vec3 unit_direction = unit_vector(r.direction());
	//auto a = 0.5 * (unit_direction.y() + 1.0);
	auto a = 0.5 * (unit_direction.y()+1.0);
	return (1.0 - a) * colour(1.0, 1.0, 1.0) + a * colour(0.5, 0.7, 1.0);
}

int main(){
	// image
	auto aspect_ratio = 16.0 / 9.0;
	int image_width = 400;

	// Calculate image height, make it at least 1
	int image_height = int(image_width / aspect_ratio);
	if (image_height < 1) {
		image_height = 1;
	}

	// Camera
	auto focal_length = 1.0;
	auto viewport_height = 2.0;
	auto viewport_width = viewport_height * (double(image_width) / image_height);
	auto camera_centre = point3(0, 0, 0);

	// Calculate vectors for horizontal and down the vertical vp edges
	vec3 viewport_u = vec3(viewport_width, 0, 0);
	vec3 viewport_v = vec3(0, -viewport_height, 0);

	// Calculate delta vectors from pixel to pixel
	auto pixel_delta_u = viewport_u / image_width;
	auto pixel_delta_v = viewport_v / image_height;

	// Calculate location of upper left pixel
	auto viewport_upper_left = camera_centre - vec3(0, 0, focal_length) -
		viewport_u / 2 - viewport_v / 2;
	auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
	
	// render
	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	for (int j = 0; j < image_height; j++) {
		std::clog << "\rScanlines remaining: " << (image_height - j) << '\n';
		for (int i = 0; i < image_width; i++) {
			auto pixel_centre = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
			auto ray_direction = pixel_centre - camera_centre;
			ray r(camera_centre, ray_direction);
			colour pixel_colour = ray_colour(r);
			write_colour(std::cout, pixel_colour);
		}
	}

	std::clog << "\rDone.       \n";
	return 0;
}

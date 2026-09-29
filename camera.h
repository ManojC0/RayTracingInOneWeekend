#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"

class camera {
public:
	/* Public camera parameters here*/
	// image
	double aspect_ratio = 1.0; // ratio of image width over height
	int image_width = 100; // rendered image width in pixel count
	void render(const hittable& world) {
		initialise();

		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
		for (int j = 0; j < image_height; j++) {
			std::clog << "\rScanlines remaining: " << (image_height - j) << '\n';
			for (int i = 0; i < image_width; i++) {
				auto pixel_centre = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
				auto ray_direction = pixel_centre - centre;
				ray r(centre, ray_direction);
				colour pixel_colour = ray_colour(r, world);
				write_colour(std::cout, pixel_colour);
			}
		}
	}


private:
	/* Private camera variables here */
	int image_height; // rendered image height
	point3 centre; // Camera centre
	point3 pixel00_loc; // location of pixel (0,0)
	vec3 pixel_delta_u; // offset to pixel to the right
	vec3 pixel_delta_v; // offset to pixel below

	void initialise() {
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;
		centre = point3(0, 0, 0);

		// Determine viewport dimensions
		auto focal_length = 1.0;
		auto viewport_height = 2.0;
		auto viewport_width = viewport_height * (double(image_width) / image_height);

		// Calculate vectors for horizontal and down the vertical vp edges
		vec3 viewport_u = vec3(viewport_width, 0, 0);
		vec3 viewport_v = vec3(0, -viewport_height, 0);

		// Calculate delta vectors from pixel to pixel
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		// Calculate location of upper left pixel
		auto viewport_upper_left = centre - vec3(0, 0, focal_length) -
			viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

	}

	colour ray_colour(const ray& r, const hittable& world) const {
		hit_record rec;
		if (world.hit(r, interval(0, infinity), rec)) {
			return rec.normal; // 0.5 * (rec.normal + colour(1, 1, 1));
		}

		vec3 unit_direction = unit_vector(r.direction());
		double a = 0.5 * (unit_direction.y() + 1);
		// linear interpolation between white and blue
		return (1.0 - a) * colour(1.0, 1.0, 1.0) + a * colour(0, 0, 1);

	}
};

#endif
#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "material.h"

class camera {
public:
	/* Public camera parameters here */
	// image
	double aspect_ratio = 1.0; // ratio of image width over height
	int image_width = 100; // rendered image width in pixel count
	int samples_per_pixel = 100; // Count of random samples for each pixel
	int max_depth = 10;
	double vfov = 90; // Vertical view angle (field of view)
	point3 lookfrom = point3(0, 0, 0);// point camera is looking from
	point3 lookat = point3(0, 0, -1); // point camera is looking at
	vec3 vup = vec3(0, 1, 0); // camera-relative "up" direction

	double defocus_angle = 0; // Variation angle of rays through each pixel
	double focus_dist = 10; // distance from camera lookfrom point to plane of perfect focus


	void render(const hittable& world) {
		initialise();

		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
		for (int j = 0; j < image_height; j++) {
			std::clog << "\rScanlines remaining: " << (image_height - j) << '\n';
			for (int i = 0; i < image_width; i++) {
				colour pixel_colour(0, 0, 0);
				for (int sample = 0;sample < samples_per_pixel; sample++) {
					ray r = get_ray(i, j);
					pixel_colour += ray_colour(r, max_depth, world);
				}
				write_colour(std::cout, pixel_samples_scale * pixel_colour);
			}
		}
	}


private:
	/* Private camera variables here */
	int image_height; // rendered image height
	double pixel_samples_scale;
	point3 centre; // Camera centre
	point3 pixel00_loc; // location of pixel (0,0)
	vec3 pixel_delta_u; // offset to pixel to the right
	vec3 pixel_delta_v; // offset to pixel below
	vec3 defocus_disk_u; // Defocus disk horizontal radius
	vec3 defocus_disk_v; // defocus disk vertical radius
	vec3 u, v, w; //camera frame basis vectors
	void initialise() {
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		pixel_samples_scale = 1.0 / samples_per_pixel;
		centre = lookfrom;

		// Determine viewport dimensions
		auto theta = degrees_to_radians(vfov);
		auto h = std::tan(theta / 2);
		auto viewport_height = 2 * h * focus_dist;
		//		auto viewport_height = 2.0;
		auto viewport_width = viewport_height * (double(image_width) / image_height);

		// Calculate the u,v,w unit basis vectors for the camera cooridnate frame
		w = unit_vector(lookfrom - lookat);
		u = unit_vector(cross(vup, w));
		v = cross(w, u);


		// Calculate vectors for horizontal and down the vertical vp edges
		vec3 viewport_u = viewport_width * u; // Vector across vp horizontal edge
		vec3 viewport_v = viewport_height * -v; // Vector down vp vertical edge

		// Calculate delta vectors from pixel to pixel
		pixel_delta_u = viewport_u / image_width;
		pixel_delta_v = viewport_v / image_height;

		// Calculate location of upper left pixel
		auto viewport_upper_left = centre - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

		auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
		defocus_disk_u = u * defocus_radius;
		defocus_disk_v = v * defocus_radius;

	}

	colour ray_colour(const ray& r, int depth, const hittable& world) const {
		// if we've exceeded the ray bounce limit, no more light gathered
		if (depth <= 0)
			return colour(0, 0, 0);
		
		hit_record rec;
		// prevents generated light rays from under the surface from intersecting the surface
		if (world.hit(r, interval(0.001, infinity), rec)) {
			ray scattered;
			colour attenuation;
			if (rec.mat->scatter(r, rec, attenuation, scattered))
				return attenuation * ray_colour(scattered, depth - 1, world);
			return colour(0, 0, 0);
			//vec3 direction = rec.normal + random_on_hemisphere(rec.normal);
			// 0.5 here is the reflectance and can be modified between 0 <= reflectance <= 1
			//return 0.5 * (ray_colour(ray(rec.p, direction), depth-1, world));
			//return rec.normal; // 0.5 * (rec.normal + colour(1, 1, 1));
			//return 0.5 * (rec.normal + colour(1, 1, 1));
		}

		vec3 unit_direction = unit_vector(r.direction());
		double a = 0.5 * (unit_direction.y() + 1.0);
		// linear interpolation between white and blue
		return (1.0 - a) * colour(1.0, 1.0, 1.0) + a * colour(0.5, 0.7, 1);

	}
	ray get_ray(int i, int j) const {
		// construct a camera ray originating from the defocus disk directed at a randomly sampled 
		// point around the pixel location (i,j)

		auto offset = sample_square();
		auto pixel_sample = pixel00_loc
			+ ((i + offset.x()) * pixel_delta_u)
			+ ((j + offset.y()) * pixel_delta_v);
		auto ray_origin = (defocus_angle <= 0) ? centre : defocus_disk_sample();
		auto ray_direction = pixel_sample - ray_origin;
		return ray(ray_origin, ray_direction);
	}

	vec3 sample_square() const {
		// returns the vector to a random point in
		// [-0.5,-0.5] to [0.5,0.5] unit square
		//return vec3(random_double() - 0.5, random_double() - 0.5, 0);
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	point3 defocus_disk_sample() const {
		// returns a random point in the camera defocus disk
		auto p = random_in_unit_disk();
		return centre + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);

	}
};



#endif
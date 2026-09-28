#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public: 
	sphere(const point3& centre, double radius) : centre(centre), radius(std::fmax(0, radius)) {}

	bool hit(const ray& r, double ray_tmin, double ray_tmax, hit_record& rec) const override {
		vec3 co = centre - r.origin();
		double a = dot(r.direction(), r.direction());
		double b = dot(-2 * r.direction(), centre - r.origin());
		double c = dot(co, co) - pow(radius, 2);
		double discriminant = pow(b, 2) - 4 * a * c;

		if (discriminant < 0)
			return false; 

		double sqrtd = std::sqrt(discriminant);

		// Find the nearest root that lies in the acceptable range
		double root = (-b - sqrtd) / (2.0*a);
		if (root <= ray_tmin || ray_tmax <= root) {
			root = (-b + sqrtd) / (2.0*a);
			if (root <= ray_tmin || ray_tmax <= root)
				return false;

		}

		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - centre) / radius;
		rec.set_face_normal(r, outward_normal);

		return true;


	}

private:
	point3 centre;
	double radius;

};

#endif
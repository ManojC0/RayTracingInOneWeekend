#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"

class material {
public: 
	virtual ~material() = default;
	
	virtual bool scatter(
		const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered)
		const {
		return false;
	}

};

class metal : public material {
public:
	metal(const colour& albedo) : albedo(albedo){}

	bool scatter(const ray& r_in, const hit_record& rec, colour& attentuation, ray& scattered)
		const override {
		auto reflection = reflect(r_in.direction(), rec.normal);

		scattered = ray(rec.p, reflection);
		attentuation = albedo;
		return true;
	}


private:
	colour albedo;
};

class lambertian : public material {
public: 
	lambertian(const colour& albedo) : albedo(albedo){}
	bool scatter(const ray& r_in, const hit_record& rec, colour& attentuation, ray& scattered)
		const override {
		auto scatter_direction = rec.normal + random_unit_vector();
		// catch degenerate scatter direction
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;
		scattered = ray(rec.p, scatter_direction);
		attentuation = albedo;
		return true;
	}

private:
	colour albedo;
};
#endif
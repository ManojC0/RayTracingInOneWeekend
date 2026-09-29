#ifndef VEC3_H
#define VEC3_H


class vec3 {
public: 
	double e[3];

	vec3() : e{0,0,0} 
	{
	}
	vec3(double e0, double e1, double e2) : e{ e0, e1, e2 }
	{
	}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
	double operator[](int i) const { return e[i]; }
	double& operator[](int i) { return e[i]; }

	vec3 operator+=(const vec3& v) {
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	vec3& operator*=(double t) {
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	vec3& operator/=(double t) {
		return *this *= 1 / t;
 	}

	double length() const {
		return std::sqrt(length_squared());
	}
	double length_squared() const {
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	static vec3 random() {
		return vec3(random_double(), random_double(), random_double());
	}

	static vec3 random(double min, double max) {
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

};

using point3 = vec3;

// Vector utility functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator+(const vec3& u, const vec3& v) {
	return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator*(const vec3& v, double t ) {
	return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline vec3 operator*(double t, const vec3& v) {
	return v * t;
}

inline vec3 operator/(const vec3 v, double t) {
	return v * (1 / t);
}

inline double dot(const vec3 u, const vec3 v) {
	return u.e[0] * v.e[0] + u.e[1] * v.e[1] + u.e[2] * v.e[2];
}

inline vec3 cross(const vec3 u, const vec3 v) {
	//[i,j,k
	//u0,u1,u2
	//v0,v1,v2]
	double component_1 = u.e[1] * v.e[2] - u.e[2] * v.e[1];
	double component_2 = u.e[2] * v.e[0] -  u.e[0] * v.e[2];
	double component_3 = u.e[0] * v.e[1] - u.e[1] * v.e[0];
	return vec3(component_1, component_2, component_3);
}

inline vec3 unit_vector(const vec3& v) {
	return v / v.length();

}

inline vec3 random_unit_vector() {
	// rejection sampling
	while (true) {
		vec3 p = vec3::random(-1, 1);
		double lensq = p.length_squared();
		// handles points very close to centre of sphere (len approx 0)
		// floating-points are annoying...
		if (1e-160 < lensq && lensq <= 1)
			return p / sqrt(lensq);
	}
}

inline vec3 random_on_hemisphere(const vec3& normal) {
	vec3 on_unit_sphere = random_unit_vector();
	if (dot(on_unit_sphere, normal) > 0.0) // in same hemisphere as normal
		return on_unit_sphere;
	else
		return -on_unit_sphere;
}


#endif
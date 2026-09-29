#ifndef UTILITIES_H
#define UTILITIES_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <cstdlib>


// constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility functions

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180;
}

inline double random_double() {
	// Returns a random real in [0,1)
	return std::rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
	// returns a random real in [min,max)
	return min + (max - min) * random_double();
}

// common Headers
#include "colour.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif

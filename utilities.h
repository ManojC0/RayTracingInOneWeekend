#ifndef UTILITIES_H
#define UTILITIES_H

#include <cmath>
#include <iostream>
#include <limits>
#include <memory>

// constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility functions

inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180;
}

// common Headers
#include "colour.h"
#include "interval.h"
#include "ray.h"
#include "vec3.h"

#endif

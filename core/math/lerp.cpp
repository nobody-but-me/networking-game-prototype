
#include <iostream>
#include <math/lerp.hpp>

namespace Math {
	float lerp(float a, float b, float t) {
		return a + t * (b - a);
	}
	float lerp_precise(float a, float b, float t) {
		return (1 - t) * a + t * b;
	}
}

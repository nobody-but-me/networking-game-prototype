
#include <iostream>
#include <math/lerp.hpp>

namespace Math {
	float lerp(float x, float y, float t) {
		return x + t * (y - x);
	}
	float lerp_precise(float x, float y, float t) {
		return (1 - t) * x + t * y;
	}
}

#include "twiddle.h"

#define PI 3.14159265

// lookup table for values of sin from 0 to pi/2 in pi/80 increments
const float lut[41] = {
	0.        , 0.03925982, 0.0784591 , 0.1175374 , 0.15643447,
	0.19509032, 0.23344536, 0.27144045, 0.30901699, 0.34611706,
	0.38268343, 0.41865974, 0.4539905 , 0.48862124, 0.52249856,
	0.55557023, 0.58778525, 0.61909395, 0.64944805, 0.67880075,
	0.70710678, 0.73432251, 0.76040597, 0.78531693, 0.80901699,
	0.83146961, 0.85264016, 0.87249601, 0.89100652, 0.90814317,
	0.92387953, 0.93819134, 0.95105652, 0.96245524, 0.97236992,
	0.98078528, 0.98768834, 0.99306846, 0.99691733, 0.99922904,
	1.
};

// use angle addition formulas to approximate
#define sin(x) lut[]


struct cnum twiddle(float real, float im, float angle) {
	// regularize argument to 0-2pi range
	angle -= ((int) (angle / (2 * PI))) * (2 * PI);
	if(angle < 0) angle += PI * 2;					// Correct for negative angle
	int q = (int) (2 * angle / PI);					// determine + save quadrant
	angle -= q * (PI / 2);							// normalize angle to first quadrant

	int index = ((int) (angle * 80 / PI));			// index to lut
	float alpha = index * (PI / 80);				// angle in lut
	float beta = angle - alpha;						// angle to be approximated

	float sin_a = lut[index];						// lookup large angle in lut
	float cos_a = lut[41 - index];					// lookup large angle in lut
	float sin_b = beta - (beta * beta * beta) / 6;	// taylor approbetaimate small angle
	float cos_b = 1 - (beta * beta) / 2;			// taylor approbetaimate small angle

	// Angle addition to calculate finals
	float sin = (q < 1 ? 1 : -1) * (sin_a * cos_b + cos_a * sin_b);
	float cos = (q % 3 ? 1 : -1) * (cos_a * cos_b - sin_a * sin_b);

	return (struct cnum) {real * cos - im * sin, real * sin + cos * im};
}

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

#define TAYLOR 1 // Degree of taylor expansion to use; 0, 1, or 2
#define PI2 1.57079632679

struct cnum twiddle(float real, float im, int k, int b) {
	k = k % (2 * b); // regularize to within 2 pi
	k = (2 * b) - k; // make it a positive angle
	q = (k * 2) / b; // determine quadrant
	// flip based on quadrant
	if (q == 1) k = b - k;
	else if (q == 2) k -= b;
	else if (q == 3) k = (2 * b) - k;

	int idx = (k * 80) / b; // find index in lut
	if((k % b) > (b / 2)) idx++; // round up

#if TAYLOR
	float sin_a = lut[idx]; // lookup in lut
	float cos_a = lut[40-idx];
	k = (80 * k) % (b * index);
	b *= 80;
	float angle = ((PI2 * k) / b);
#endif /* TAYLOR */
#if TAYLOR == 1
	float sin = sin_a + cos_a * angle;
	float cos = cos_a - sin_a * angle;
#elif TAYLOR == 2
	float sin_b = angle - (angle * angle * angle / 6);
	float cos_b = 1 - (angle * angle / 2);
	float sin = sin_a * cos_b + cos_a * sin_b;
	float cos = cos_a * cos_b - sin_a * sin_b;
#else /* TAYLOR == .*/
	float sin = lut[idx]; // lookup in lut
	float cos = lut[40-idx];
#endif /* TAYLOR == .*/
	return (struct cnum) {real * cos, real * sin};
}

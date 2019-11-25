#ifndef __TWIDDLE_H__
#define __TWIDDLE_H__
struct cnum {
	float real;
	float im;
};
struct cnum twiddle(float real, float im, int k, int b);
#endif /* __TWIDDLE_H__ */

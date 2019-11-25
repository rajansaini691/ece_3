#include "fft.h"
#include "twiddle.h"
#include <xil_types.h>
#include <xil_assert.h>
#include <xio.h>
#include <stdio.h>


static float new_[512];
static float new_im[512];

#define SAMPLE_F 48828.125 // (100 * 1000 * 1000 / 2048.0)
#define SAMPLES 512
#define BIN_SPACING 95.467431640625 // SAMPLE_F / SAMPLES
#define M 9


// n - 512 (buffer size)
float fft(int* q) {
	int w[SAMPLES];
	int a,b,r,d,e,c;
	int k,place;
	a=SAMPLES/2;
	b=1;
	int i,j;
	float real=0, imagine=0;
	float max,frequency;

	// Ordering algorithm
	for(i=0; i<(M-1); i++){
		d=0;
		for (j=0; j<b; j++){
			for (c=0; c<a; c++){	
				e=c+d;
				new_[e]=q[(c*2)+d];
				new_[e+a]=q[2*c+1+d];
			}
			d+=(SAMPLES/b);
		}
		for (r=0; r<SAMPLES;r++){
			q[r]=new_[r];
		}
		b*=2;
		a=SAMPLES/(2*b);
	}
	//end ordering algorithm

	b=1;
	k=0;
	for (j=0; j<M; j++){
	//MATH
		for(i=0; i<SAMPLES; i+=2){
			if (i%(SAMPLES/b)==0 && i!=0)
				k++;
			struct cnum tw = twiddle(q[i+1], k, b);
			new_[i] = q[i] + tw.real;
			new_im[i] = tw.im;
			new_[i+1] = q[i] - tw.real;
			new_im[i+1] = -tw.im;

		}
		for (i=0; i<SAMPLES; i++){
			q[i]=new_[i];
			w[i] = new_im[i];
		}
	//END MATH

	//REORDER
		for (i=0; i<SAMPLES/2; i++){
			new_[i]=q[2*i];
			new_[i+(SAMPLES/2)]=q[2*i+1];
			new_im[i] = w[2*i];
			new_im[i+(n/2)]=w[2*i+1];
		}
		for (i=0; i<SAMPLES; i++){
			q[i]=new_[i];
			w[i] = new_im[i];
		}
	//END REORDER	
		b*=2;
		k=0;		
	}

	//find magnitudes
	max=0;
	place=1;
	for(i=1;i<(SAMPLES/2);i++) {
		new_[i]=q[i]*q[i]+w[i]*w[i];
		if(max < new_[i]) {
			max=new_[i];
			place=i;
		}
	}

	//curve fitting for more accuarcy
	//assumes parabolic shape and uses three point to find the shift in the parabola
	//using the equation y=A(x-x0)^2+C
	float y1=new_[place-1],y2=new_[place],y3=new_[place+1];
	float x0 =+ (2 * BIN_SPACING * (y2-y1))/(2 * y2 - y1 - y3);
	x0 = x0 / BIN_SPACING - 1;
	
	if(x0 <0 || x0 > 2) { //error
		return 0;
	}
	if(x0 <= 1)  {
		frequency=frequency-(1-x0)*BIN_SPACING;
	}
	else {
		frequency=frequency+(x0-1)*BIN_SPACING;
	}
	
	return frequency;
}

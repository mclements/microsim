#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "dist.h"

double dist_unif(void)
{
	return ((double) rand()) / ((double) RAND_MAX);
}


static double exp_rand(void)
{
	/*implementation based on exp_rand in Rmath*/

	/* q[k-1] = sum(log(2)^k / k!)  k=1,..,n, */
	/* The highest n (here 16) is determined by q[n-1] = 1.0 */
	/* within standard precision */
	const static double q[] =
	{
		0.6931471805599453,
		0.9333736875190459,
		0.9888777961838675,
		0.9984959252914960,
		0.9998292811061389,
		0.9999833164100727,
		0.9999985691438767,
		0.9999998906925558,
		0.9999999924734159,
		0.9999999995283275,
		0.9999999999728814,
		0.9999999999985598,
		0.9999999999999289,
		0.9999999999999968,
		0.9999999999999999,
		1.0000000000000000
	};

	double a = 0.;
	double u = dist_unif();	/* precaution if u = 0 is ever returned */
	while(u <= 0. || u >= 1.) u = dist_unif();
	for (;;) {
		u += u;
		if (u > 1.)
			break;
		a += q[0];
	}
	u -= 1.;

	if (u <= q[0])
		return a + u;

	int i = 0;
	double ustar = dist_unif(), umin = ustar;
	do {
		ustar = dist_unif();
		if (umin > ustar)
			umin = ustar;
		i++;
	} while (u > q[i]);
	return a + umin * q[0];
}


double dist_exp(double scale)
{
	/*implementation based on rexp in Rmath*/

	double result;
	
	if (isfinite(scale) && (scale > 0.0)) {
		result = scale * exp_rand();
	} else {
		if(scale == 0.0) {
			result = 0.0;
		} else {
			fprintf(stderr, "argument out of domain in function weibull\n");
			result = NAN;
		}
	}
	return result;
	return 0.0;
}


double dist_weibull(double shape, double scale)
{
	/*implementation based on rweibull in Rmath*/

	double result;
	
	if (isfinite(shape) && isfinite(scale) && (shape > 0.0) && (scale > 0.0)) {
		result = scale * pow(-log(dist_unif()), 1.0 / shape);
	} else {
		if(scale == 0.0) {
			result = 0.0;
		} else {
			fprintf(stderr, "argument out of domain in function weibull\n");
			result = NAN;
		}
	}
	return result;
}

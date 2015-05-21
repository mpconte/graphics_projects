/*
 * Misc. vector functions
 *
 *	John Amanatides, Sept. 2007
 */


#include <math.h>
#include "vector.h"


/*
 * compute cross product
 */

void
Cross(Vector result, Vector a, Vector b)
{
	result[0]= a[1]*b[2] - a[2]*b[1];
	result[1]= a[2]*b[0] - a[0]*b[2];
	result[2]= a[0]*b[1] - a[1]*b[0];
}

/*
 * make vector unit length;  also, return length of original vector.
 */

double
Normalize(Vector a)
{
	double length, scale;

	length= DOT(a, a);
	length= sqrt(length);
	if(length != 0.0) {
		scale= 1.0/length;
		a[0] *= scale;
		a[1] *= scale;
		a[2] *= scale;
	}
	return length;
}

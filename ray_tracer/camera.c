/*
 * 
 * implements art's camera model with a lens that shoots 5 extra rays
 *
 *	Matthew Conte
 	Victoria Kirnos
	
	Dec 3, 2007
 */


#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "artInternal.h"

extern Vector	Cross(Vector, Vector);
extern double	Normalize(Vector *);

static Point camera;
Vector vx, vy, vz;
static double scaleX, scaleY;
double focusDistance, lensRadius;

Ray lens_rays[NUM_LENS_RAYS];


/*get 5 new rays 4 a lense */
Ray *
lens(Ray ray)
{
	Ray *rays;
	Color c1,c2,c3,c4,c5,c6,colorFinal;
	Ray ray2,ray3,ray4, ray5, ray6;
	double ran;
	ran =(double)rand() /(double)RAND_MAX;
	
	int j = 0;
	
	Point convP;
	Vector u;
	Vector v1;
	
	/*for ray */
	double t;
	Vector normal;
	Material material;
	TIMES(u,vz,focusDistance); 
	
	double  d= DOT(u,ray.direction);
	TIMES (v1, ray.direction,d);
	PLUS(convP, ray.origin, v1);
	
	lens_rays[0] = ray;

	
	/*for ray2 */
	Vector vyr;
	Vector vyr2;
	TIMES(vyr, vy, lensRadius);
	TIMES(vyr2, vyr, (double)rand()/(double)RAND_MAX);
	//ray2.origin = ray->origin + vyr2;
	PLUS(ray2.origin, ray.origin,vyr)
	MINUS(ray2.direction, convP, ray2.origin);
	
	lens_rays[1] = ray2;


	/*for ray3 */
	Vector vyr3 ;
	TIMES(vyr3, vyr, (double)rand()/(double)RAND_MAX);
	MINUS(ray3.origin, ray2.origin,vyr3)
	MINUS(ray3.direction, convP, ray3.origin);
	
	lens_rays[2] = ray3;	


	/*for ray4 */
	Vector vxr ;
	Vector vxr4 ;
	TIMES(vxr, vx, lensRadius);
	TIMES(vxr4, vxr, (double)rand()/(double)RAND_MAX);
	MINUS(ray4.origin, ray3.origin,vxr4)
	MINUS(ray4.direction, convP, ray4.origin);
	
	lens_rays[3] = ray4;


	/*for ray5*/
	Vector vxr5 ;
	TIMES(vxr5, vxr, (double)rand()/(double)RAND_MAX);
	PLUS(ray5.origin, ray4.origin,vxr5)
	MINUS(ray5.direction, convP, ray5.origin);
	
	lens_rays[4] = ray5;


	/*for ray6*/
	Vector vxr6 ;
	TIMES(vxr6, vxr, (double)rand()/(double)RAND_MAX);
	PLUS(ray6.origin, ray5.origin,vxr6)
	MINUS(ray6.direction, convP, ray6.origin);

	lens_rays[5] = ray6;


	//assign ray pointer to first element of array of rays
	rays = &lens_rays[0];

	
	return rays;

}


char *
art_Camera(Point lookFrom, Point lookAt, Vector lookUp)
{
	Vector x, z;

	MINUS(z, lookAt, lookFrom);
	if(Normalize(&z) == 0.0)
		return "art_Camera: domain error";
	x= Cross(z, lookUp);
	if(Normalize(&x) == 0.0)
		return "art_Camera: domain error";

	camera= lookFrom;
	vx= x;
	vy= Cross(x, z);
	vz= z;
	return NULL;
}


char *
art_Lens(double focus, double radius)
{
	if(focus <= 0.0 || radius < 0.0)
		return "art_Lens: domain error";
	focusDistance= focus;
	lensRadius= radius;
	return NULL;
}


char *
art_Perspective(double width, double height, double distance)
{
	if(distance <= 0.0 || width <= 0.0 || height <= 0.0)
		return "art_Perspective: domain error";
	scaleX= width/distance;
	scaleY= height/distance;
	return NULL;
}


void InitCamera(void)
{
	Vector from, to, up;

	from.v[0]= 0.0;
	from.v[1]= 0.0;
	from.v[2]= 10.0;

	to.v[0]= 0.0;
	to.v[1]= 0.0;
	to.v[2]= 0.0;

	up.v[0]= 0.0;
	up.v[1]= 1.0;
	up.v[2]= 0.0;

	(void) art_Camera(from, to, up);
	(void) art_Lens(1.0, 0.0);
	(void) art_Perspective(1.0, 1.0, 1.0);
}



/*
 * given sample point on unit viewing square (0.0 <= u, v <= 1.0)
 * return 6 rays that travels in that direction from the eye
 * 
 */

Ray *
ShootRays(double u, double v)
{
	Ray ray;
	Vector tmp;	
	
	
	ray.origin= camera;
	ray.direction= vz;

	if(u < 0.0 || u > 1.0 || v < 0.0 || v > 1.0) {
		(void) fprintf(stderr, "ShootRay: domain error!!\n");
		exit(1);
	} else {
		TIMES(tmp, vx, (u-0.5)*scaleX);
		PLUS(ray.direction, tmp, ray.direction);
		TIMES(tmp, vy, (v-0.5)*scaleY);
		PLUS(ray.direction, tmp, ray.direction);
		(void) Normalize(&ray.direction);
	}	
	
	return lens(ray);		
}

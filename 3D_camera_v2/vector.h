typedef float Point[3];
typedef float Vector[3];

#define DOT(a,b) (a[0]*b[0] + a[1]*b[1] + a[2]*b[2])
#define PLUS(r,a,b)	{ \
			r[0] = a[0] + b[0]; \
			r[1] = a[1] + b[1]; \
			r[2] = a[2] + b[2]; \
			}

#define MINUS(r,a,b)	{ \
			r[0] = a[0] - b[0]; \
			r[1] = a[1] - b[1]; \
			r[2] = a[2] - b[2]; \
			}

#define TIMES(r,a,s)	{ \
			r[0] = (s)*a[0]; \
			r[1] = (s)*a[1]; \
			r[2] = (s)*a[2]; \
			}

extern void Cross(Vector, Vector, Vector);
extern double Normalize(Vector);


/*
 * the scene data structure is created/stored/traversed here
 *
 *	John Amanatides, Nov 2007
 */


#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "artInternal.h"

extern Material	GetCurrentMaterial(void);
extern int	IntersectSphere(Ray *, double *, Vector *);
extern int	IntersectPlane(Ray *, double *, Vector *);
extern int	IntersectCube(Ray *, double *, Vector *);
extern Point	InvTransPoint(Point, Affine *);
extern Vector	InvTransVector(Vector, Affine *), TransNormal(Vector, Affine *);
extern Matrix	MultMatrix(Matrix *, Matrix *);
extern void	InitCamera(void), InitShading(void), FinishShading(void);

#define SPHERE		1
#define PLANE		2
#define CUBE		3

typedef struct StackNode {
	Affine CTM;
	struct StackNode *next;
} StackNode;

typedef struct ListNode {
	int nodeType;
	Affine affine;
	Material material;
	struct ListNode *next;
} ListNode;

static Matrix identity= {       1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0};
static Affine CTM;
static StackNode *CTMstack;
static ListNode *scene;


char *
art_Start(void)
{
	CTM.TM= identity;
	CTM.inverseTM= identity;
	CTMstack= NULL;
	InitCamera();
	InitShading();
	scene= NULL;

	return NULL;
}


static void
FreeModel(scene)
	ListNode *scene;
{
	ListNode *node;
	while(scene) {
		node= scene;
		scene= scene->next;
		free((void *) node);
		/* note material node is never removed */
	}
}


char *
art_End(void)
{
	while(CTMstack != NULL)
		(void) art_PopTM();
	FreeModel(scene);
	FinishShading();
	return NULL;
}


char *
art_InitTM(void)
{
	CTM.TM= identity;
	CTM.inverseTM= identity;
	return NULL;
}


char *
art_PushTM(void)
{
        StackNode *sp;

	sp= (StackNode *) malloc(sizeof(StackNode));
	sp->CTM= CTM;
	sp->next= CTMstack;
	CTMstack= sp;
	return NULL;
}


char *
art_PopTM(void)
{
        StackNode *sp;

        if(CTMstack != NULL) {
                CTM= CTMstack->CTM;
                sp= CTMstack;
                CTMstack= CTMstack->next;
                free((void *) sp);
		return NULL;
        }
        else	return "stack underflow";
}


static void
ApplyAffine(Affine trans)
{
	CTM.TM= MultMatrix(&CTM.TM, &trans.TM);
	CTM.inverseTM= MultMatrix(&trans.inverseTM, &CTM.inverseTM);
}


char *
art_Scale(double sx, double sy, double sz)
{
	Affine trans;

int i,j;

for(i = 0; i < 4; i++)
{
	for(j = 0; j < 4; j++)
	{
		if(i == j)
		{
			if(i == 0)
			{
			(trans.TM).m[i][j] = sx;
			(trans.inverseTM).m[i][j] = 1.0/sx;
			}
			
			else if(i == 1)
			{
			(trans.TM).m[i][j] = sy;
			(trans.inverseTM).m[i][j] = 1.0/sy;
			}
			
			else if(i == 2)
			{
			(trans.TM).m[i][j] = sz;
			(trans.inverseTM).m[i][j] = 1.0/sz;
			}
			
			else
			{
			(trans.TM).m[i][j] = 1.0;
			(trans.inverseTM).m[i][j] = 1.0;
			}
		}
		else
		{
		(trans.TM).m[i][j] = 0.0;
		(trans.inverseTM).m[i][j] = 0.0;
		}
		
	}
}


	ApplyAffine(trans);
	return NULL;
}


char *
art_Rotate(char axis, double degrees)
{
	Affine trans;
	
	double rad = (degrees * M_PI) / 180;
	int i,j;
	
if( axis == 'x')
{	
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{	
			if((i == 0 && j == 0) || (i == 3 && j == 3))
			{
			(trans.TM).m[i][j] = 1;
			(trans.inverseTM).m[i][j] = 1;
			}
			
			else if(i == 1 && j == 1)
			{
			(trans.TM).m[i][j] = cos(rad);
			(trans.inverseTM).m[i][j] = cos(-rad);
			}
			
			else if(i == 1 && j == 2)
			{
			(trans.TM).m[i][j] = -sin(rad);
			(trans.inverseTM).m[i][j] = -sin(-rad);
			}
			
			else if(i == 2 && j == 1)
			{
			(trans.TM).m[i][j] = sin(rad);
			(trans.inverseTM).m[i][j] = sin(-rad);
			}
			
			else if(i == 2 && j == 2)
			{
			(trans.TM).m[i][j] = cos(rad);
			(trans.inverseTM).m[i][j] = cos(-rad);
			}
			
			else
			{
			(trans.TM).m[i][j] = 0;
			(trans.inverseTM).m[i][j] = 0;
			}
		}
	}
}
else if ( axis == 'y')
{
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{	
			if((i == 1 && j == 1) || (i == 3 && j == 3))
			{
			(trans.TM).m[i][j] = 1;
			(trans.inverseTM).m[i][j] = 1;
			}
			
			else if(i == 0 && j == 0)
			{
			(trans.TM).m[i][j] = cos(rad);
			(trans.inverseTM).m[i][j] = cos(-rad);
			}
			
			else if(i == 0 && j == 2)
			{
			(trans.TM).m[i][j] = sin(rad);
			(trans.inverseTM).m[i][j] = sin(-rad);
			}
			
			else if(i == 2 && j == 0)
			{
			(trans.TM).m[i][j] = -sin(rad);
			(trans.inverseTM).m[i][j] = -sin(-rad);
			}
			
			else if(i == 2 && j == 2)
			{
			(trans.TM).m[i][j] = cos(rad);
			(trans.inverseTM).m[i][j] = cos(-rad);
			}
			
			else
			{
			(trans.TM).m[i][j] = 0;
			(trans.inverseTM).m[i][j] = 0;
			}
		}
	}

}

else if ( axis == 'z')
{
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{	
			if((i == 0 && j == 0) || (i == 1 && j == 1))
			{
			(trans.TM).m[i][j] = cos(rad);
			(trans.inverseTM).m[i][j] = cos(-rad);
			}
			
			else if(i == 0 && j == 1)
			{
			(trans.TM).m[i][j] = -sin(rad);
			(trans.inverseTM).m[i][j] = -sin(-rad);
			}
			
			else if(i == 1 && j == 0)
			{
			(trans.TM).m[i][j] = sin(rad);
			(trans.inverseTM).m[i][j] = sin(-rad);
			}
			
			else if (i == j)
			{
			(trans.TM).m[i][j] = 1;
			(trans.inverseTM).m[i][j] = 1;
			}
			
			else
			{
			(trans.TM).m[i][j] = 0;
			(trans.inverseTM).m[i][j] = 0;
			}
		}
	}

}

	ApplyAffine(trans);
	return NULL;
}


char *
art_Translate(double tx, double ty, double tz)
{
	Affine trans;

	int i,j;
	
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			if(i == j)
			{
			(trans.TM).m[i][j] = 1;
			(trans.inverseTM).m[i][j] = 1;
			}
			
			else if (i == 0 && j == 3)
			{
			(trans.TM).m[i][j] = tx;
			(trans.inverseTM).m[i][j] = -tx;
			}
			
			else if (i == 1 && j == 3)
			{
			(trans.TM).m[i][j] = ty;
			(trans.inverseTM).m[i][j] = -ty;
			}
			
			else if (i == 2 && j == 3)
			{
			(trans.TM).m[i][j] = tz;
			(trans.inverseTM).m[i][j] = -tz;
			}
			
			else
			{
			(trans.TM).m[i][j] = 0;
			(trans.inverseTM).m[i][j] = 0;			
			}
		}			
	}
	
	
	ApplyAffine(trans);
	return NULL;
}


char *
art_Shear(char axis1, char axis2, double shear)
{
	Affine trans;
	int a1, a2;

	int i,j;
	
	
	if(axis1 == 'x')
	{
		if(axis2 == 'y')
		{
			(trans.TM).m[0][1] = shear;
			(trans.inverseTM).m[0][1] = -shear;
		}
		
		else if(axis2 == 'z')
		{
			(trans.TM).m[0][2] = shear;
			(trans.inverseTM).m[0][2] = -shear;
		}	
	}
	
	else if(axis1 == 'y')
	{
		if(axis2 == 'x')
		{
			(trans.TM).m[1][0] = shear;
			(trans.inverseTM).m[1][0] = -shear;
		}
		
		else if(axis2 == 'z')
		{
			(trans.TM).m[1][2] = shear;
			(trans.inverseTM).m[1][2] = -shear;
		}	
	}
	
	else if(axis1 == 'z')
	{
		if(axis2 == 'x')
		{
			(trans.TM).m[2][0] = shear;
			(trans.inverseTM).m[2][0] = -shear;
		}
		
		else if(axis2 == 'y')
		{
			(trans.TM).m[2][1] = shear;
			(trans.inverseTM).m[2][1] = -shear;
		}	
	}
	
	
for(i = 0; i < 4; i++)
{
	for(j = 0; j < 4; j++)
	{
		if(i == j)
		{
			(trans.TM).m[i][j] = 1;
			(trans.inverseTM).m[i][j] = 1;
		}
		else if(((trans.TM).m[i][j]) != shear)
		{
			(trans.TM).m[i][j] = 0;
			(trans.inverseTM).m[i][j] = 0;
		}
		
	}
	
}

	ApplyAffine(trans);
	return NULL;
}


static void AddObject(int nodeType)
{
	ListNode *object;

	object= (ListNode *) malloc (sizeof(ListNode));
	object->nodeType= nodeType;
	object->affine= CTM;
	object->material= GetCurrentMaterial();
	object->next= scene;
	scene= object;
}


char *
art_Sphere()
{
	AddObject(SPHERE);
	return NULL;
}


char *
art_Plane()
{
	AddObject(PLANE);
	return NULL;
}


char *
art_Cube()
{
	AddObject(CUBE);
	return NULL;
}


/*
 * This function, when passed a ray and list of objects
 * returns a pointer to the closest intersected object in the list
 * (whose t-value is less than t) and updates t and normal to
 * the value of the closest object's normal and t-value.
 * It returns NULL if it find no object closer than t from
 * the ray origin.  If anyHit is true then it returns the
 * first object that is closer than t.
 */
static ListNode *
ReallyIntersectScene(Ray *ray, ListNode *obj, int anyHit, double *t, Vector *normal)
{
	ListNode *closestObj, *resultObj;
	Ray transRay;
	int i, result;

	closestObj= NULL;

	while(obj != NULL) {
		/* transform ray */
		transRay.origin= InvTransPoint(ray->origin, &obj->affine);
		transRay.direction= InvTransVector(ray->direction, &obj->affine);

		/* intersect object */
		switch(obj->nodeType) {

		case SPHERE:
			result= IntersectSphere(&transRay, t, normal);
			break;
		case PLANE:
			result= IntersectPlane(&transRay, t, normal);
			break;
		case CUBE:
			result= IntersectCube(&transRay, t, normal);
			break;
		}

		/* keep closest intersection */
		if (result == HIT) {
			closestObj= obj;
			if(anyHit)
				return obj;
		}

		obj= obj->next;
	}

	return closestObj;
}


int
IntersectScene(Ray *ray, double *t, Vector *normal, Material *material)
{
	ListNode *closestObj;
	double closestT;
	Vector closestNormal;

	closestT= UNIVERSE;
	closestObj= ReallyIntersectScene(ray, scene, 0, &closestT, &closestNormal);
	if(closestObj != NULL) {
		*t= closestT;
		*normal= TransNormal(closestNormal, &closestObj->affine);
		*material= closestObj->material;
		return HIT;
	}
	else	return MISS;
}


int
ShadowProbe(Ray *ray, double distanceToLight)
{
	ListNode *closestObj;
	double closestT;
	Vector closestNormal;

	closestT= distanceToLight;
	closestObj= ReallyIntersectScene(ray, scene, 1, &closestT, &closestNormal);
	if(closestObj != NULL)
		return HIT;
	else	return MISS;
}

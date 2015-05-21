/*
 * deals with lights/shading functions
 *
 *	Matthew Conte
 	Victoria Kirnos
	Dec 3, 2007
 */


#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include "artInternal.h"

#define CHECKERBOARD	1
#define ZONE_PLATE	2

#define MAX_DEPTH	5

#define MIN(X,Y) ((X) < (Y) ? : (X) : (Y))
#define MAX(X,Y) ((X) > (Y) ? : (X) : (Y))


extern double	Normalize(Vector *);
extern Vector	ReflectRay(Vector, Vector);
extern int	IntersectScene(Ray *, double *, Vector *, Material *);
extern int	ShadowProbe(Ray *, double);
extern Vector vx, vy, vz;
extern double  focusDistance, lensRadius;
static Color	background;
static Material	currentMaterial;
static Color black= {0.0, 0.0, 0.0}, white= {1.0, 1.0, 1.0};


Color GetRadiance(Ray *ray, int depth);
double absval(double);


//Light data structure
typedef struct {	
	Vector pos; 		  //xyz position of light source
	double intensity, radius; //intensity and covering radius of light source	
} Light;

// Node of a light source in linked-list of light sources
typedef struct LightListNode {		

	Light light;
	struct LightListNode *next;	
	
} LightListNode;


//Actual linked-list of light sources
LightListNode *LightList;


static void AddLight(Light l)
{
	LightListNode *object;

	object= (LightListNode *) malloc (sizeof(LightListNode));	
	object->light = l;
	object->next = LightList;
	LightList= object;
}



/* add Light source to Linked List of Light Sources */
char *
art_Light(double x, double y, double z, double intensity, double radius)
{	
	Light l;
	
	l.pos.v[0] = x; l.pos.v[1] = y; l.pos.v[2] = z;
	l.intensity = intensity; 
	l.radius = radius;
	
	AddLight(l);
	
	return NULL;
}


char *
art_Material(Material material)
{
	currentMaterial= material;
	return NULL;
}


Material
GetCurrentMaterial(void)
{
	return currentMaterial;
}


char *
art_Background(Color color)
{
	background= color;
	return NULL;
}


static Color
Texture(Material *material, Point position)
{               
	int funnySum;
	double EPSILON= 0.0001;
	double contribution;
	Color result;

	switch(material->texture) {

	case CHECKERBOARD: 
		funnySum= floor(position.v[0]+EPSILON)
			+ floor(position.v[1]+EPSILON)
			+ floor(position.v[2]+EPSILON);
		if(funnySum % 2)
			return white;
		else    return material->col;
	case ZONE_PLATE:
		contribution= 0.5*cos(DOT(position, position))+0.5;
		TIMES(result, material->col, contribution);
		return result;  
	default:                
	return material->col;
	}       
}       


static Color
ComputeRadiance(Ray *ray, double t, Vector normal, Material material, int depth)
{		

	//reset LightListNode after loop with this pointer to head of list
	LightListNode *obj;	
	obj = LightList;
		
		
	//components of overall shading for local illumination
	double ambient[3] = {0.0,0.0,0.0};
	double diffuse[3] = {0.0,0.0,0.0};
	double specular[3] = {0.0,0.0,0.0}; 
	
	double intensity;	
	
	Color newColor, reflectColor, transmitColor;
	
	
	//Black by default
	reflectColor = black;
	transmitColor = black;

	
	double lightDistance, incidentLength, normalLength;
	
	int j;
		
	
	Ray lightRay, reflectRay, transmitRay;
	Vector reflect, transmit, light;		
	
	
	//Point of Intersection of Ray
	Point intersection;	
	intersection.v[0] = ray->origin.v[0] + (ray->direction.v[0]*t);
	intersection.v[1] = ray->origin.v[1] + (ray->direction.v[1]*t);
	intersection.v[2] = ray->origin.v[2] + (ray->direction.v[2]*t);
	
					
	//origin of Light Ray is at the point of intersection
	(lightRay.origin) = intersection;											

	
	
	//inverse incident and normal vector for "inside/outside" testing	
	Vector inverseIncident = ray->direction;	
	Vector inverseNormal = normal;
			
	for(j = 0; j < 3; j++)		
	{
		inverseNormal.v[j] = -inverseNormal.v[j];
		inverseIncident.v[j] = -inverseIncident.v[j];
	}						
	
	incidentLength =  Normalize(&(ray->direction));
	normalLength = Normalize(&normal);			
	
	
	reflect = ReflectRay(ray->direction, normal);
		
		
	//Updating to Texture Color
	material.col = Texture(&material, intersection);	
	
	//Loop through light sources
	while(LightList != NULL)
	{					
		//intensity of current light source
		intensity = (LightList->light).intensity;						
		
		//direction of shadow ray  = (Point of Light Source) - (Point of Intersection on surface)
		MINUS(lightRay.direction, (LightList->light).pos, lightRay.origin);				
							
		
		lightDistance =  Normalize(&(lightRay.direction));
				
		int shadow = ShadowProbe(&lightRay, lightDistance);									
							
	light = lightRay.direction;
		
		
	//compute Diffuse component		
	double d = DOT(light, normal);	


	//Compute Specular component			
	double s = DOT(reflect, light);
												
		for(j = 0; j < 3; j++)
		{									
			//if shadow ray didn't hit something, not in shadow, include specular + diffuse intensity			
			if(shadow == MISS)
			{					
				if(s > 0)
					specular[j] += (intensity * (material.Ks) * pow(s, material.n)) 
					/ pow(lightDistance, 2);	


				if(d > 0)				
					diffuse[j]  += (intensity * ((material.col).v[j] * (material.Kd)) * d) 
					/ pow(lightDistance, 2);	
				
			}
		}
		
		LightList = LightList->next;
	}

		
	//reset LightListNode to head Node
	LightList = obj;
				
	
	
	/////////////////////Calculate Local Illumination Here (via Phong Model)//////////////////
	for(j = 0; j < 3; j++)
		ambient[j] = material.Ka * (material.col).v[j];
		
	
	for(j = 0; j < 3; j++)
	{	
		//ambient component
		newColor.v[j] = ambient[j];
		
		//diffuse and specular component
		newColor.v[j] += diffuse[j] + specular[j];
	}
		
	
	
	//////////////////////////////////////////////////////////////////////////////////////////
	////////////Global Illumination, include reflect/transmit ray, (Recursive Part)///////////
	//////////////////////////////////////////////////////////////////////////////////////////
			
					
	//Create Reflection and Transmit Ray and recurse to find there radiance		
	reflectRay.direction = reflect;	
		
	(transmitRay.origin) = intersection;
	(reflectRay.origin) = intersection;
	
	// if material is transmissive
	if(material.Kt > 0.0)
	{						
			//Angle between incident and normal vector for inside/outside testing
			double theta;			
			theta = acos(DOT(inverseIncident, normal) / (incidentLength * normalLength));						
			
			
			//if angle between incident ray and surface normal is within 90 degrees, transmit ray's going inside the object
			if(theta <= M_PI_2)
			{			
				if (TransmitRay(ray->direction, normal, 1.0, material.index, &transmit))			
					transmitRay.direction = transmit;																
			}
			
			//otherwise, transmit ray's going out of the object
			else
			{
				if (TransmitRay(ray->direction, inverseNormal, material.index, 1.0, &transmit))	
					transmitRay.direction = transmit;	
			}			
				
				
			//Recursively trace into transmit ray			
			transmitColor = GetRadiance(&transmitRay, depth + 1);						
	}


	// if material is reflective
	if(material.Kr > 0.0)

			//Recursively trace into reflect ray
			reflectColor = GetRadiance(&reflectRay, depth + 1);
	
	
	TIMES(reflectColor, reflectColor, material.Kr);
	TIMES(transmitColor, transmitColor, material.Kt);
	
	PLUS(newColor, newColor, reflectColor);
	PLUS(newColor, newColor, transmitColor);
		
	return newColor;
}



Color
GetRadiance(Ray *ray, int depth)
{
	double t;
	Vector normal;
	Material material;
	
	
	if(IntersectScene(ray, &t, &normal, &material) == HIT)
	{		
		if (depth < MAX_DEPTH)		
			return ComputeRadiance(ray, t, normal, material, depth);			
		else
			return material.col;

	}
	else	return background;
}






void InitShading()
{
	Material material;

	material.col= white;
	material.Ka= 0.2;
	material.Kd= 0.6;
	material.Ks= 0.7;
	material.n= 50.0;
	material.Kr= 0.0;
	material.Kt= 0.0;
	material.index= 1.0;
	material.texture= 0;
	(void) art_Material(material);
	(void) art_Background(black);
}


//free all light nodes at end of program
static void
FreeLights(LightList)
	LightListNode *LightList;
{
	LightListNode *node;
	
	while(LightList) {
		node= LightList;
		LightList= LightList->next;
		free((void *) node);		
	}
}

char *
art_Pop_Light(void)
{
	LightListNode *node;
	
	if(LightList != NULL)
	{
		node = LightList;
		LightList = LightList->next;
		free((void *) node);
	}
	
	return NULL;
}


void FinishShading()
{	
	FreeLights(LightList);
}


/* Absolute Value Function */
double absval(double val)
{
   if (val >= 0) return val;
   else return -val;
}

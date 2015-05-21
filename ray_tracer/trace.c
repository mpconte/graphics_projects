/*
 * does the actual tracing of rays and does anti-aliasing with randomized super-sampling
 * 
 *
 *	Matthew Conte
 *	Victoria Kirnos
 *	Dec 3, 2007
 */


#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "artInternal.h"
#include <time.h>

extern	Ray *ShootRays(double, double);
extern	Color GetRadiance(Ray *, int);
extern	Color lens(Ray *, int);
extern FILE *OpenTIFF(int, int, char *);
extern CloseTIFF(FILE *);
extern WritePixelTIFF(FILE *, int, int, int);

#define INVERSE_GAMMA	(1.0/2.2)

static Color black = {0.0, 0.0, 0.0};


char *
art_Trace(int xRes, int yRes, int numSamples, char *filename)
{
	FILE *fp;
	Ray *rays;		
	int x, y, red, green, blue;
	double u, v;
	Color sample;
	
	
	double SampleStepX = 1.0/(numSamples * xRes);
	double SampleStepY = 1.0/(numSamples * yRes);
	
	
	if(xRes < 1 || yRes < 1 || numSamples < 1 )
		return "art_Trace: domain error";
	if ((fp = OpenTIFF(xRes, yRes, filename)) == NULL)
		return "art_Trace: couldn't open output file";

	/* compute image */
        for(y=0; y < yRes; y++) {
	
                for(x= 0; x < xRes; x++) {
            			            
	              		u= ((double) x)/xRes;
                      		v= 1.0 - ((double) y)/yRes;
			                        
				int i, j;			
												
						/* Ranomized Super Sampling:
						* selects a random point within the designated pixel
						* for each sample		
						*/												
						sample = black;									
						for(i = 0; i < numSamples; i++)
						{		
							double minX, maxX, minY, maxY, ranX, ranY;
							
							minX = u;
							maxX = u + (1.0/xRes);										
							ranX = (rand() / ((double)(RAND_MAX))) * (maxX-minX) + minX;
										
							minY = v - (1.0/yRes);
							maxY = v;										
							ranY = (rand() / ((double)(RAND_MAX))) * (maxY-minY) + minY;
							
							//with randomized u and v	
							rays = ShootRays(ranX, ranY);
				
										
							//Calculate and sum of the colors of the rays created by the lens
							Color lensColor = {0.0, 0.0, 0.0};											
							for(j=0; j<NUM_LENS_RAYS; j++)											
								PLUS(lensColor, lensColor, GetRadiance(&(*(rays + j)), 0));	
							
							//average the total color of the lens				
							TIMES(lensColor, lensColor, 1.0/NUM_LENS_RAYS);
									
							//addd result to sample color													                                                        
							PLUS(sample, sample, lensColor);										
						}	
						
									
						                        
			//Average out the samples and their lens rays			
			TIMES(sample, sample, 1.0/numSamples);
			
			
			/* convert to bytes and write out */
			red= 255.0*pow(sample.v[0], INVERSE_GAMMA);
			if(red > 255)
				red= 255;
			else if(red < 0)
				red= 0;
			green= 255.0*pow(sample.v[1], INVERSE_GAMMA);
			if(green > 255)
				green= 255;
			else if(green < 0)
				green= 0;
			blue= 255.0*pow(sample.v[2], INVERSE_GAMMA);
			if(blue > 255)
				blue= 255;
			else if(blue < 0)
				blue= 0;
			WritePixelTIFF(fp, red, green, blue);
                }
        }

        CloseTIFF(fp);
	return NULL;
}

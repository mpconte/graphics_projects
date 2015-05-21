/*
	This file contains a function that draws a Surface of revolution, given the parameters
	
	Matthew Conte   207333230 cs243082  
	Victoria Kirnos 206009310 cs231167
*/

#ifdef DARWIN
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include "vector.h"
#include <math.h>

#

static float white[]= {1.0, 1.0, 1.0, 1.0};
static float black[]= {0.0, 0.0, 0.0, 0.0};
static float red[]= {1.0, 0.0, 0.0, 1.0};
static float green[]= {0.0, 1.0, 0.0, 1.0};
static float blue[]= {0.0, 0.0, 1.0, 1.0};
static float yellow[]= {1.0, 1.0, 0.0, 1.0};
static float ambient[]= {0.2, 0.2, 0.2, 1.0};



void drawModel(float *axis_point1, float *axis_point2, int MAX_PLOTS, float plot_points[MAX_PLOTS][3], int num_plots, int
line_drawn, int SOR )
{

	glMatrixMode(GL_MODELVIEW);
	
	/* Draw Green Ground Polygon */
	glBegin(GL_LINE_LOOP);
		glColor3fv(green);
		glVertex3i(-5, 0, -5);
		glVertex3i( 5, 0, -5);
		glVertex3i( 5, 0,  5);
		glVertex3i(-5, 0,  5);
	glEnd();
		
		
	glBegin(GL_POLYGON);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
		glNormal3f(0.0, 1.0, 0.0);
		glVertex3i(-5, 0, -5);
		glVertex3i( 5, 0, -5);
		glVertex3i( 5, 0,  5);
		glVertex3i(-5, 0,  5);		
	glEnd();
	

	/* if the axis is drawn or being drawn, do it here */
	if((line_drawn == 1 || line_drawn == 2))
	{
		
		glPointSize(4);
		glColor3fv(yellow);
		
		/* yellow axis end points */
		glBegin(GL_POINTS);
			glVertex3f(axis_point1[0], axis_point1[1], axis_point1[2]);
			glVertex3f(axis_point2[0], axis_point2[1], axis_point2[2]);		
		glEnd();
		
		glPointSize(1);
		
		/* black axis line */
		glBegin(GL_LINES);
			glColor3fv(black);			
			glVertex3f(axis_point1[0], axis_point1[1], axis_point1[2]);
			glVertex3f(axis_point2[0], axis_point2[1], axis_point2[2]);
		glEnd();				

	}
	
	
	/*if line is drawn and we have some plotted points, display the plots */
	if (line_drawn == 2 && num_plots > 0)
	{			
		int i = 0;
		
		/* display all plots */				
		glPointSize(4);
		
		glBegin(GL_POINTS);			
		glColor3fv(white);
			
		/* white plot points */			
		for(i = 0; i < num_plots; i++)
		{			
			glVertex3f(plot_points[i][0], plot_points[i][1], plot_points[i][2]);			
		}		
		glEnd();
		
		glPointSize(1);
		
		
	}	
	
	
/************************************************************/
/*Create surface of revolution if SOR flag indicates to do so*/
/************************************************************/
		
	if (SOR == 1)
     {	      	    			        		    	
    	// first point of line to rotate about axis 
	float x,y,z;    	    	
    	
	// second point of line to rotate about axis 
    	float xNext, yNext, zNext;
    	
		
    	x = axis_point1[0];
    	y = axis_point1[1];
    	z = axis_point1[2];
    	    	
	/* used to preserve previous values of x,y,z during transformations */
	float oldx, oldy, oldz;
	float oldNewx, oldNewy, oldNewz;
			
						
	/* The new altered co-ordinates after transformations */
	float xNew, yNew, zNew, xNextNew, yNextNew, zNextNew;								
			
	
	
       	// number of rotations about axis of rotation	
        const float num_rotations = 36.0;	
	  
	// step size around circle of revolution
	const float DTHETA = 360.0/num_rotations; 
    	
        
        int ix;	
        
         //scroll through each plot here    
	for (ix = 0 ; ix <= num_plots ; ++ix )  
	{   
		   	 	
		 if (ix < num_plots )
		 {   				
	        		xNext = plot_points[ix][0];
		            	yNext = plot_points[ix][1];
		         	zNext = plot_points[ix][2];
	   	 }
	   	 else
	   	 {
			   	 xNext = axis_point2[0];
		   		 yNext = axis_point2[1];
			   	 zNext = axis_point2[2];
	   	 }
	        		
	   	
	                     
		float ia;
	
		GLfloat light_position[] = { 0.0, -2.0, -2.0, 0.0 };
		glEnable (GL_DEPTH_TEST);
		glEnable ( GL_LIGHTING );
		glEnable (GL_NORMALIZE);
		glEnable ( GL_LIGHT0 );
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);
			
			
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 50);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
			
			
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
			
		
		glBegin( GL_QUAD_STRIP ) ;
			
			
		    //rotate the two points around line of axis  here	
	            for (ia=0.0 ; ia < num_rotations ; ++ia )
	            {						    			    		           		       			
			Vector u;
			MINUS(u, axis_point2, axis_point1);				
			Normalize(u);	

			//bisecting angle into x-y plane
			float d = sqrt((u[1]*u[1]) + (u[2]*u[2])); 						
			
			
			// Rotate about arbitrary axis (step-by-step)
						
			//Step 1 - translate points to axis_point1
			xNew = x - axis_point1[0];			
			xNextNew = xNext - axis_point1[0];
			
			yNew = y - axis_point1[1];			
			yNextNew = yNext - axis_point1[1];
			
			zNew = z - axis_point1[2];
			zNextNew = zNext - axis_point1[2];
					
									
			oldx = xNew;
			oldy = yNew;
			oldz = zNew;
				
			oldNewx = xNextNew;
			oldNewy = yNextNew;
			oldNewz = zNextNew;
				
									
			float cost, sint;
			
			//Step 2 - rotate points about x-axis such that unit vector is on xz-plane					
			if (d != 0)
			{
			cost = u[2]/d;
			sint = u[1]/d;			
			
			yNew = (oldy*cost) - (oldz*sint);			
			zNew = (oldy*sint) + (oldz*cost);
			
			yNextNew = (oldNewy*cost) - (oldNewz*sint);			
			zNextNew = (oldNewy*sint) + (oldNewz*cost);
			
			oldy = yNew;
			oldz = zNew;
			
			oldNewy = yNextNew;
			oldNewz = zNextNew;	
												
			}
			
			
			//Step 3 - rotate about y-axis such that unit vector is aligned with z-axis
			xNew = (oldx*d) - (oldz*u[0]);
			zNew = (oldx*u[0]) + (oldz*d);

			xNextNew = (oldNewx*d) - (oldNewz*u[0]);
			zNextNew = (oldNewx*u[0]) + (oldNewz*d); 
			
			oldx = xNew;
			oldz = zNew;
			
			oldNewx = xNextNew;
			oldNewz = zNextNew;
							
			
			
			//Step 4 - rotate about z-axis to display SOR
			float theta_rad = (DTHETA*M_PI)/180.0;				
				
			xNew = (oldx*cos(theta_rad)) - (oldy*sin(theta_rad));
			yNew = (oldx*sin(theta_rad)) + (oldy*cos(theta_rad));
			
			xNextNew = (oldNewx*cos(theta_rad)) - (oldNewy*sin(theta_rad));
			yNextNew = (oldNewx*sin(theta_rad)) + (oldNewy*cos(theta_rad));			
			
			
			oldx = xNew;
			oldy = yNew;
			
			oldNewx = xNextNew;
			oldNewy = yNextNew;

			
			
			//Step 5 - rotate about y-axis such that unit vector is aligned with z-axis
			
			xNew = (oldx*d) + (oldz*u[0]);
			zNew = (-(oldx*u[0])) + (oldz*d);

			xNextNew = (oldNewx*d) + (oldNewz*u[0]);
			zNextNew = (-(oldNewx*u[0])) + (oldNewz*d); 
			
			oldx = xNew;
			oldz = zNew;
			
			oldNewx = xNextNew;
			oldNewz = zNextNew;
			
			
			//Step 6 - rotate points about x-axis such that unit vector is on xz-plane					
			if (d != 0)
			{
			cost = u[2]/d;
			sint = u[1]/d;			
			
			yNew = (oldy*cost) + (oldz*sint);			
			zNew = (-(oldy*sint)) + (oldz*cost);
			
			yNextNew = (oldNewy*cost) + (oldNewz*sint);			
			zNextNew = (-(oldNewy*sint)) + (oldNewz*cost);
			
			oldy = yNew;
			oldz = zNew;
			
			oldNewy = yNextNew;
			oldNewz = zNextNew;	
												
			}
			
			//Step 7 - translate points to axis_point1
			xNew =  oldx + axis_point1[0];
			xNextNew  = oldNewx + axis_point1[0];
			
			yNew =  oldy + axis_point1[1];
			yNextNew = oldNewy + axis_point1[1];
			
			zNew =  oldz + axis_point1[2];
			zNextNew = oldNewz + axis_point1[2];
			
			
			
			
			//Final display here
			
			/* normal = normal face plane of four vertex */
							
          		Vector v1 = {x, y, z};
	          	Vector v2 ={xNext,yNext,zNext};        		    			    
			    
			Vector v21;
		        MINUS(v21, v2,v1);
		        Normalize(v21);		            			    
			    
		        Vector v3 = {xNew,yNew,zNew};		        
			    
			Vector v31;			    			    		            
			MINUS(v31, v3, v1);
		        Normalize(v31);		            
			    
			Vector normal;
		        Cross(normal, v21, v31);		
	
			
			glNormal3f(normal[0], normal[1], normal[2]);	
			glVertex3f(     x,     y,     z ) ;
	               	glVertex3f( xNext, yNext, zNext ) ;	
			glVertex3f(     xNew,     yNew,     zNew ) ;
	               	glVertex3f( xNextNew, yNextNew, zNextNew ) ;			
			 	
				
			x = xNew;
			y = yNew;
			z = zNew;
			
			xNext = xNextNew; 
			yNext = yNextNew;
			zNext =	zNextNew;	
	            }	          

		    
		 glEnd() ;    
		    
		glDisable(GL_LIGHT0);	
		glDisable(GL_LIGHTING);			
		glDisable(GL_DEPTH_TEST);  
		    
		// Prepare for next pass through the outer loop
	        x= xNext ; 
	        y= yNext ;
	        z= zNext ;			                        	    		        
	       }	       	   	      	         			     		

	}

}


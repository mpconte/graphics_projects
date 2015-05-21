/*
	A2 - This file contains function that draws a wireframe camera.

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
#include "camera.h"

/*
*/
void
drawCamera(float *lookFrom, float *lookAt, float *lookUp, float angle, float AspectRatio)
{
	/* draw your camera here */	
			
	/* Unit Vectors*/
		
		   Vector vz;
           MINUS(vz, lookFrom,lookAt); 
           Normalize(vz);
		   Vector lookup = {lookUp[0], lookUp[1], lookUp[2]};
		   Vector vx;
		   Cross(vx, lookup, vz);
		   Normalize (vx);
		   Vector vy;
		   Cross(vy, vz, vx);
		   Normalize(vy);  
		
		/*Draw fixed lookUp here (2* vy vertex) */
		lookUp[0] = vy[0];
		lookUp[1] = vy[1];
		lookUp[2] = vy[2];
		
		/*Draw points (White)*/	
		glPointSize(6);															
		
		glBegin(GL_POINTS);
						
			/* LookFrom Point */			            
            		glColor3f(256.0, 256.0, 256.0);						
			glVertex3f(lookFrom[0], lookFrom[1], lookFrom[2]);		
			
			/* lookAt Point */
			glColor3f(0, 0, 0);																							
			glVertex3f(lookAt[0], lookAt[1], lookAt[2]);		
		
			
            /* lookUp Point */
            Vector lookUpp;
            PLUS(lookUpp,lookFrom,lookUp);
			glPointSize(2);		
			glColor3f(256.0, 0, 0);																																							
			glVertex3f(lookUpp[0],lookUpp[1],lookUpp[2]);		
			
        /*glVertex3f(lookUp[0], lookUp[1], lookUp[2]);*/
		glEnd();
		
		glPointSize(1);
		
        /*Line between lookFrom and lookAt (Blue, Wide, Dashed)  */		
			
			glLineWidth(3.0);			
			glLineStipple(2, 0xAAAA);
			glEnable(GL_LINE_STIPPLE);
			
         glBegin(GL_LINES);
			glColor3f(0, 0, 1);
			glVertex3f(lookAt[0], lookAt[1], lookAt[2]);
			glVertex3f(lookFrom[0], lookFrom[1], lookFrom[2]);									
         glEnd( );	
		
			glLineWidth(1.0); 	    /*reset line width */
			glDisable(GL_LINE_STIPPLE); /*reset Stipple */
			
			
			
  /******************************************************
   Draw actual camera and/or frustum/pyramid view here ***
  *******************************************************/
		
        glTranslatef(lookFrom[0] - 2*vz[0], lookFrom[1]  - 2*vz[1], lookFrom[2] - 2*vz[2]);
       
       /* Calculate the Width and Height for the base of the Pyramid*/
       	float h;
		h = 2*tan(angle*3.14/360);
	
    	float w;
		w = h*AspectRatio;
        
        /* Vector calculations for drawing the Pyramid*/
         Vector vx1;
         TIMES(vx1,vx,h);
         Vector vy1;
         TIMES(vy1,vy,h);
         Vector vx2;
         TIMES(vx2,vx,w);
         Vector vy2; 
         TIMES(vy2,vy,w);
         Vector vxyp1;
         PLUS(vxyp1, vx1,vy1);
         Vector vxym1;
         MINUS(vxym1, vx1,vy1);
         Vector vxym2;
         TIMES(vxym2,vx2,-1);
         MINUS(vxym2, vxym2,vy2);
         Vector vxyp11;
         TIMES(vxyp11,vx1,-1);
         PLUS(vxyp11,vxyp11,vy1);
         Vector vzt;
         TIMES(vzt,vz,2);
         Vector vzyp;
         PLUS(vzyp,vzt,vy);   
         Vector vxyp2;
         PLUS(vxyp2, vx2,vy2);    
          
		glLineWidth(2.0);
        	
	/* Drawing UP vector  (green) */
		glBegin(GL_LINE_STRIP);		
			glColor3f(0,1,0);
			glVertex3f(vzt[0], vzt[1], vzt[2]);
			glVertex3f(vzyp[0], vzyp[1], vzyp[2]);											
		glEnd();   	
			
		glLineWidth(1.0);
         
        /* Drawing the HEIGHT of the base of the Pyramid  (blue)*/
        glBegin(GL_LINE_STRIP);
            glColor3f(0,0,1);		
		    glVertex3f(vxyp2[0] , vxyp2[1], vxyp2[2]);
		    glVertex3f(vxym1[0] , vxym1[1], vxym1[2]);
        glEnd();  
        
       	 /* Drawing the WIDTH of the base of the Pyramid     */    
         glBegin(GL_LINE_STRIP);
            glColor3f(0,0,1);	
		    glVertex3f(vxym1[0] , vxym1[1], vxym1[2]);
			glVertex3f(vxym2[0] , vxym2[1], vxym2[2]);
		glEnd();  
        
       	/* Drawing the HEIGHT of the base of the Pyramid */
         glBegin(GL_LINE_STRIP);  
			glColor3f(0,0,1);		
			glVertex3f(vxym2[0] , vxym2[1], vxym2[2]);
			glVertex3f(vxyp11[0] , vxyp11[1], vxyp11[2]);
		glEnd(); 
         
         /* Drawing the WIDTH of the base of the Pyramid  */  
         glBegin(GL_LINE_STRIP);
			glColor3f(0,0,1);
			glVertex3f(vxyp11[0] , vxyp11[1], vxyp11[2]);
			glVertex3f(vxyp2[0] , vxyp2[1], vxyp2[2]);
		 glEnd();
        
        /*Drawing the side of the pyramid */
        glBegin(GL_LINE_STRIP); 
            glColor3f(0.5,1,1);
			glVertex3f(vzt[0], vzt[1], vzt[2]);
			glVertex3f(vxyp2[0] , vxyp2[1], vxyp2[2]);
        glEnd();
        
        
        /*Drawing the side of the pyramid */
         glBegin(GL_LINE_STRIP); 
			glColor3f(0.5,1,1);
			glVertex3f(vzt[0], vzt[1], vzt[2]);
			glVertex3f(vxym1[0] , vxym1[1], vxym1[2]);
         glEnd();
           
            /*Drawing the side of the pyramid */
         glBegin(GL_LINE_STRIP); 
			glColor3f(0.5,1,1);
            glVertex3f(vzt[0], vzt[1], vzt[2]);
			glVertex3f(vxym2[0] , vxym2[1], vxym2[2]);
		 glEnd();
            
            
            /*Drawing the side of the pyramid */
            glBegin(GL_LINE_STRIP); 
               glColor3f(0.5,1,1);
			   glVertex3f(vzt[0], vzt[1], vzt[2]);
		       glVertex3f(vxyp11[0] , vxyp11[1], vxyp11[2]);
            glEnd();
                 
   
}

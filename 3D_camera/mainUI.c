/*
	A2 - interactive openGL program that plays with the camera

	Right button quits program

	Matthew Conte   207333230 cs243082  
	Victoria Kirnos 206009310 cs231167
*/

#ifdef DARWIN
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#include "camera.h"

#define GRID_SIZE	10


/* initial values */

static float windowWidth= 600;
static float windowHeight= 400;
static float aspectRatio= 3.0/2.0;
static int middleX= 300;
static int middleY= 200;
static float lookFrom[]= {5.0, 3.0, 8.0};
static float lookAt[]= {0.0, 0.0, 0.0};
static float lookUp[]= {0.0, 1.0, 0.0}; 


static float cameraAngle= 60.0;

/*LookFrom Screen Co-ordinates*/
static float lookFromScreenx;
static float lookFromScreeny;
		
/*LookAt Screen Co-ordinates*/
static float lookAtScreenx;
static float lookAtScreeny;		

/*LookUp Screen Co-ordinates*/
static float lookUpScreenx;
static float lookUpScreeny;	


static int isLeftMouseActive;  						/* = 1 if Left Mouse Button down, else 0 */														
static int lookFromDrag, lookAtDrag, lookUpDrag;			/* = 1 if dragging look Point, else 0 */
static int top_left_drag, bot_right_drag, bot_left_drag, top_right_drag;/* = 1 if dragging point in quadrant, else 0 */

static float oldX, oldY;				/*For Camera Angle Adjustment in Top_Right Quadrant*/

							
/* return 1 if all points in all windows are within the window, return 0 otherwise */						
int all_in_range()
{
	/*length of single grid in pixels (Window Height / number of grids)*/
	float num_pixel_grid = windowHeight/(4*GRID_SIZE);
	
	if(		   
			/* All points within Top_Left */
		   ((windowWidth/4) + (lookFrom[0] * num_pixel_grid) < windowWidth/2) && 
		   ((windowHeight/4) - (lookFrom[1] * num_pixel_grid) < windowHeight/2) &&
				
		   ((windowWidth/4) + (lookAt[0] * num_pixel_grid)  < windowWidth/2 )&& 
		   ((windowHeight/4) - (lookAt[1] * num_pixel_grid) < windowHeight/2 )&& 
					
	           ((windowWidth/4) + (lookUp[0] * num_pixel_grid) < windowWidth/2 )&& 
		   ((windowHeight/4) - (lookUp[1] * num_pixel_grid) < windowHeight/2 )&& 
		
		   
		   
		   /* All points within Bot_Left */
		   ((windowWidth/4) + (lookFrom[0] * num_pixel_grid) < windowWidth / 2 ) &&
		   (((3*windowHeight)/4) + (lookFrom[2] * num_pixel_grid) < windowHeight )&&
		   (((3*windowHeight)/4) + (lookFrom[2] * num_pixel_grid) > windowHeight / 2 )&&

		   ((windowWidth/4) + (lookAt[0] * num_pixel_grid) < windowWidth / 2 )&&	   
		   (((3*windowHeight)/4) + (lookAt[2] * num_pixel_grid) < windowHeight )&&
		   (((3*windowHeight)/4) + (lookAt[2] * num_pixel_grid) > windowHeight / 2 )&&	   
				
		   ((windowWidth/4) + (lookUp[0] * num_pixel_grid) < windowWidth / 2 )&&		   
		   (((3*windowHeight)/4) + (lookUp[2] * num_pixel_grid) < windowHeight )&& 
		   (((3*windowHeight)/4) + (lookUp[2] * num_pixel_grid) > windowHeight / 2 )&&
		   
		   
		   /* All points within Bot_Right */
		   (((3*windowWidth)/4) + (lookFrom[2] * num_pixel_grid) < windowWidth )&&
		   (((3*windowWidth)/4) + (lookFrom[2] * num_pixel_grid) > windowWidth / 2 )&&
		   (((3*windowHeight)/4) + (lookFrom[1] * num_pixel_grid) < windowHeight )&&
		   (((3*windowHeight)/4) + (lookFrom[1] * num_pixel_grid) > windowHeight / 2) &&

		   (((3*windowWidth)/4) + (lookAt[2] * num_pixel_grid) < windowWidth )&&
   		   (((3*windowWidth)/4) + (lookAt[2] * num_pixel_grid) > windowWidth / 2 )&&
		   (((3*windowHeight)/4) + (lookAt[1] * num_pixel_grid) < windowHeight )&& 
		   (((3*windowHeight)/4) + (lookAt[1] * num_pixel_grid) > windowHeight / 2 )&& 		   
				
		   (((3*windowWidth)/4) + (lookUp[2] * num_pixel_grid) < windowWidth )&&
   		   (((3*windowWidth)/4) + (lookUp[2] * num_pixel_grid) > windowWidth / 2) &&
		   (((3*windowHeight)/4) + (lookUp[1] * num_pixel_grid) < windowHeight) && 
		   (((3*windowHeight)/4) + (lookUp[1] * num_pixel_grid) > windowHeight / 2)
		   
		   )
		   {			   
			   return 1;
		   }
		   
		  return 0;		   
}							



void
myDisplay(void)
{
	int range;
	
	middleX= windowWidth/2;
	middleY= windowHeight/2;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* draw shaded view in perspective as seen from the camera */
	/* in top right quadrant */
	glViewport(middleX, middleY, windowWidth/2, windowHeight/2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(cameraAngle, aspectRatio, 0.1, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(lookFrom[0], lookFrom[1], lookFrom[2],
			lookAt[0], lookAt[1], lookAt[2],
			lookUp[0], lookUp[1], lookUp[2]);
	drawModel(0);
	

	/* draw the orthographic views */
	if(aspectRatio < 1.0)
		range= GRID_SIZE;
	else
		range= GRID_SIZE*aspectRatio+1;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-GRID_SIZE*aspectRatio, GRID_SIZE*aspectRatio, -GRID_SIZE, GRID_SIZE, -GRID_SIZE*aspectRatio, GRID_SIZE*aspectRatio);
	glMatrixMode(GL_MODELVIEW);
	

	
	/* draw xy ortho view in top left quadrant */
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0,   0.0, 0.0, -1.0,   0.0, 1.0, 0.0);
	glViewport(0, middleY, windowWidth/2, windowHeight/2);
	drawGrid(XY, range);
	drawModel(1);	
	drawCamera(lookFrom, lookAt, lookUp, cameraAngle, aspectRatio);
	


	
	/* draw xz ortho view in bottom left quadrant */
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0,   0.0, -1.0, 0.0,   0.0, 0.0, -1.0);
	glViewport(0, 0, windowWidth/2, windowHeight/2);
	drawGrid(XZ, range);
	drawModel(1);
	drawCamera(lookFrom, lookAt, lookUp, cameraAngle, aspectRatio);

	
	
	
	/* draw yz ortho view in bottom right quadrant */
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0,   -1.0, 0.0, 0.0,   0.0, 1.0, 0.0);
	glViewport(middleX, 0, windowWidth/2, windowHeight/2);
	drawGrid(YZ, range);
	drawModel(1);
	drawCamera(lookFrom, lookAt, lookUp, cameraAngle, aspectRatio);

	
	
	
	/* draw subwindow borders */
	glViewport(0, 0, windowWidth, windowHeight);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, windowWidth, 0.0, windowHeight);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glColor3f(1.0, 1.0, 1.0);
	glBegin(GL_LINES);
		glVertex2i(0, middleY);
		glVertex2i(windowWidth, middleY);
		glVertex2i(middleX, 0);
		glVertex2i(middleX, windowHeight);
	glEnd();
	glutSwapBuffers();
}





void myMouse(int button,int state,int x,int y)
{
	
	/*length of single grid in pixels (Window Height / number of grids)*/
	float num_pixel_grid = windowHeight/(4*GRID_SIZE);
	
	
	if (state == GLUT_UP)
	{	
		/* reset drag flags */
		lookFromDrag = 0;
		lookAtDrag = 0;
		lookUpDrag = 0;
		
		top_left_drag = 0;
		bot_left_drag = 0;		
		bot_right_drag = 0;
		top_right_drag = 0;
		
		
		if (button == GLUT_LEFT_BUTTON)
		{
			isLeftMouseActive = 0;		
		}
	}

	else if (state == GLUT_DOWN)
	{		
			
		
		/* On Left Mouse click */							
		if (button == GLUT_LEFT_BUTTON)
		{	
				/* Screen Co-ordinates of lookFrom,At,Up in Top_Left*/
				
				lookFromScreenx = (windowWidth/4) + (lookFrom[0] * num_pixel_grid);
				lookFromScreeny = (windowHeight/4) - (lookFrom[1] * num_pixel_grid);
				
				lookAtScreenx = (windowWidth/4) + (lookAt[0] * num_pixel_grid);
				lookAtScreeny = (windowHeight/4) - (lookAt[1] * num_pixel_grid);
					
				
				lookUpScreenx = (windowWidth/4) + ((lookUp[0] + lookFrom[0]) * num_pixel_grid);
				lookUpScreeny = (windowHeight/4) - ((lookUp[1] + lookFrom[1]) * num_pixel_grid);
				

				/*if clicked within 5 pixels of lookFrom in Top_left, switch appropriate flags */
				if ( (x >= lookFromScreenx - 5 && x <= lookFromScreenx + 5) && (y >= lookFromScreeny - 5 && y <= lookFromScreeny + 5))
				{						
					lookFromDrag = 1;
					top_left_drag = 1;	
				}
				/*if clicked within 5 pixels of lookAt in Top_left, switch appropriate flags*/				
				else if ( (x >= lookAtScreenx - 5 && x <= lookAtScreenx + 5) && (y >= lookAtScreeny - 5 && y <= lookAtScreeny + 5))
				{						
					lookAtDrag = 1;
					top_left_drag = 1;	
				}
				/*if clicked within 5 pixels of lookUp in Top_left, switch appropriate flags*/				
				else if ( (x >= lookUpScreenx - 5 && x <= lookUpScreenx + 5) && (y >= lookUpScreeny - 5 && y <= lookUpScreeny + 5))
				{						
					lookUpDrag = 1;
					top_left_drag = 1;	
				}
				
				
				
				/* Screen Co-ordinates of lookFrom,At,Up in Bottom_Left*/
				lookFromScreenx = (windowWidth/4) + (lookFrom[0] * num_pixel_grid);
				lookFromScreeny = ((3*windowHeight)/4) + (lookFrom[2] * num_pixel_grid);

				lookAtScreenx = (windowWidth/4) + (lookAt[0] * num_pixel_grid);
				lookAtScreeny = ((3*windowHeight)/4) + (lookAt[2] * num_pixel_grid);
				
				lookUpScreenx = (windowWidth/4) + ((lookUp[0] + lookFrom[0]) * num_pixel_grid);
				lookUpScreeny = ((3*windowHeight)/4) + ((lookUp[2] + lookFrom[2]) * num_pixel_grid);
					
				/*if clicked within 5 pixels of lookFrom in Bottom_Left, switch appropriate flags*/
				if ( (x >= lookFromScreenx - 5 && x <= lookFromScreenx + 5) && (y >= lookFromScreeny - 5 && y <= lookFromScreeny + 5))
				{						
					lookFromDrag = 1;
					bot_left_drag = 1;	
				}
				/*if clicked within 5 pixels of lookAt in Bottom_Left, switch appropriate flags*/	
				else if ( (x >= lookAtScreenx - 5 && x <= lookAtScreenx + 5) && (y >= lookAtScreeny - 5 && y <= lookAtScreeny + 5))
				{						
					lookAtDrag = 1;
					bot_left_drag = 1;	
				}
				/*if clicked within 5 pixels of lookup in Bottom_Left, switch appropriate flags*/	
				else if ( (x >= lookUpScreenx - 5 && x <= lookUpScreenx + 5) && (y >= lookUpScreeny - 5 && y <= lookUpScreeny + 5))
				{						
					lookUpDrag = 1;
					bot_left_drag = 1;	
				}

				
				
				
				/* Screen Co-ordinates of lookFrom,At,Up in Bottom_Right*/
				lookFromScreenx = ((3*windowWidth)/4) - (lookFrom[2] * num_pixel_grid);
				lookFromScreeny = ((3*windowHeight)/4) - (lookFrom[1] * num_pixel_grid);
				
				lookAtScreenx = ((3*windowWidth)/4) - (lookAt[2] * num_pixel_grid);
				lookAtScreeny = ((3*windowHeight)/4) - (lookAt[1] * num_pixel_grid);
				
				lookUpScreenx = ((3*windowWidth)/4) - ((lookUp[2] + lookFrom[2]) * num_pixel_grid);
				lookUpScreeny = ((3*windowHeight)/4) - ((lookUp[1] + lookFrom[1]) * num_pixel_grid);
				
				/*if clicked within 5 pixels of lookFrom in Bottom_Right, switch appropriate flags*/
				if ( (x >= lookFromScreenx - 5 && x <= lookFromScreenx + 5) && (y >= lookFromScreeny - 5 && y <= lookFromScreeny + 5))
				{						
					lookFromDrag = 1;
					bot_right_drag = 1;	
				}
				/*if clicked within 5 pixels of lookAt in Bottom_Right, switch appropriate flags*/
				else if ( (x >= lookAtScreenx - 5 && x <= lookAtScreenx + 5) && (y >= lookAtScreeny - 5 && y <= lookAtScreeny + 5))
				{						
					lookAtDrag = 1;
					bot_right_drag = 1;	
				}
				/*if clicked within 5 pixels of lookAt in Bottom_Right, switch appropriate flags*/
				else if ( (x >= lookUpScreenx - 5 && x <= lookUpScreenx + 5) && (y >= lookUpScreeny - 5 && y <= lookUpScreeny + 5))
				{						
					lookUpDrag = 1;
					bot_right_drag = 1;	
				}												
				
				
				/*Top_Right Quadrant uses Left-Click to adjust camera Angle */
				if (x > windowWidth/2 & x < windowWidth & y > 0 & y < windowHeight/2)
				{				
					oldY = y;
					oldX = x;
					top_right_drag = 1;
				}
				
			/* set Left Mouse button to active */
			isLeftMouseActive = 1;		
		}
		
		/* Quit program on right-click */
		else if(button == GLUT_RIGHT_BUTTON)
		{
			exit(0);
		}

	}
}

void myMotion(int x,int y)
{
	/*length of single grid in pixels (Window Height / number of grids)*/
	float num_pixel_grid = windowHeight/(4.0*GRID_SIZE);
	
	/* save x-y co-ord of previous point on the window */
	float oldSpotx, oldSpoty;

	/*Left_Click Motion*/
	if(isLeftMouseActive == 1)
	{		
		   
			/*Top_left Quadrant*/
			if ( x > 0 & x < windowWidth/2 & y < windowHeight/2 & y > 0 & top_left_drag == 1)
			{						
					if(lookFromDrag == 1)
					{	
						oldSpotx = lookFrom[0];
						oldSpoty = lookFrom[1];				
						
						lookFrom[0] = ((x - (windowWidth/4))) / num_pixel_grid;							
						lookFrom[1] = (((windowHeight/4) - y)) / num_pixel_grid;	
						
																		
						/*reset points if not all points are within range */
						if(all_in_range() == 0)
						{						
							lookFrom[0] = oldSpotx;		
							lookFrom[1] = oldSpoty;							
						}
						
					}						
					else if(lookAtDrag == 1)
					{
						oldSpotx = lookAt[0];
						oldSpoty = lookAt[1];	
						
						lookAt[0] = ((x - (windowWidth/4))) / num_pixel_grid;							
						lookAt[1] = (((windowHeight/4) - y)) / num_pixel_grid;											
						
						/*reset points if not all points are within range */
						if(all_in_range() == 0)
						{	
							lookAt[0] = oldSpotx;
							lookAt[1] = oldSpoty;
						}
						
					}	
					else if(lookUpDrag == 1)
					{		
						oldSpotx = lookUp[0];
						oldSpoty = lookUp[1];
							     						
						lookUp[0] = (((x - (windowWidth/4))) / num_pixel_grid)  - lookFrom[0] ;							
						lookUp[1] = ((((windowHeight/4) - y)) / num_pixel_grid) - lookFrom[1] ;																										  
						
						/*reset points if not all points are within range */															
						if(all_in_range() == 0)
						{
							lookUp[0] = oldSpotx;
							lookUp[1] = oldSpoty;
						}	
						
					}	
									
			}
	
			/*Bot_Left Quadrant */
			else if (x > 0 & x < windowWidth/2 & y > windowHeight/2 & y < windowHeight & bot_left_drag == 1)
			{
					if(lookFromDrag == 1)
					{
						oldSpotx = lookFrom[0];
						oldSpoty = lookFrom[2];
						
						lookFrom[0] = ((x - (windowWidth/4))) / num_pixel_grid;								
						lookFrom[2] = (y - ((3*windowHeight)/4)) / num_pixel_grid;		

						/*reset points if not all points are within range */
						if(all_in_range() == 0)
						{
							lookFrom[0] = oldSpotx;
							lookFrom[2] = oldSpoty;							
						}
					}
					else if(lookAtDrag == 1)
					{
						oldSpotx = lookAt[0];
						oldSpoty = lookAt[2];
						
						lookAt[0] = ((x - (windowWidth/4))) / num_pixel_grid;								
						lookAt[2] = (y - ((3*windowHeight)/4)) / num_pixel_grid;
						
						/*reset points if not all points are within range */
						if(all_in_range() == 0)
						{
							lookAt[0] = oldSpotx;
							lookAt[2] = oldSpoty;
						}
					}
					else if(lookUpDrag == 1)
					{
						oldSpotx = lookUp[0];
						oldSpoty = lookUp[2];
						
						lookUp[0] = (((x - (windowWidth/4))) / num_pixel_grid) - lookFrom[0];								
						lookUp[2] = ((y - ((3*windowHeight)/4)) / num_pixel_grid) - lookFrom[2];
						
						/*reset points if not all points are within range */
						if(all_in_range() == 0)
						{
							lookUp[0] = oldSpotx;
							lookUp[2] = oldSpoty;
						}
					}
			}
			
			/*Bot_Right Quadrant*/
			else if (x < windowWidth & x > windowWidth/2 & y > windowHeight/2 & y < windowHeight & bot_right_drag == 1)
			{
					if(lookFromDrag == 1)
					{
						oldSpotx = lookFrom[2];
						oldSpoty = lookFrom[1];
						
						lookFrom[2] = ((((3*windowWidth)/4))- x) / num_pixel_grid;								
						lookFrom[1] = (((3*windowHeight)/4) - y) / num_pixel_grid;				
				
						/*reset points if not all points are within range */
						if(all_in_range() == 0)
						{							
							lookFrom[2] = oldSpotx;
							lookFrom[1] = oldSpoty;							
						}
							
					}
					else if (lookAtDrag == 1)
					{
						oldSpotx = lookAt[2];
						oldSpoty = lookAt[1];
						
						lookAt[2] = ((((3*windowWidth)/4))- x) / num_pixel_grid;								
						lookAt[1] = (((3*windowHeight)/4) - y) / num_pixel_grid;
						
						/*reset points if not all points are within range */
						if(all_in_range() == 0)
						{
							lookAt[2] = oldSpotx;
							lookAt[1] = oldSpoty;
						}
					}
					else if (lookUpDrag == 1)
					{
						oldSpotx = lookUp[2];
						oldSpoty = lookUp[1];
						
						lookUp[2] = (((((3*windowWidth)/4))- x) / num_pixel_grid) - lookFrom[2];								
						lookUp[1] = ((((3*windowHeight)/4) - y) / num_pixel_grid) - lookFrom[1];
						
						/*reset points if not all points are within range */
						if(all_in_range() == 0)
						{
							lookUp[2] = oldSpotx;
							lookUp[1] = oldSpoty;
						}
					}					
			}
		
	
	
			/*Top_Right Quadrant, scroll up/down with left mouse to increase/decrease camera Angle */
			else if (x < windowWidth & x > windowWidth/2 & y > 0 & y
			< windowHeight/2 & top_right_drag == 1)
			{											
						
				cameraAngle += y - oldY;
					
				if(cameraAngle <= 0 || cameraAngle >= 180)
				{
				cameraAngle -= y - oldY;			
				}

				oldY = y;												
			}
	
	}
	
	glutPostRedisplay();
	
}




void
myReshape(int width, int height) 
{
	windowWidth= width;
	windowHeight= height;
	aspectRatio= windowWidth/windowHeight;
	glutPostRedisplay();
}

void
myInit(void)
{
	glClearColor(0.5, 0.5, 0.5, 1.0);
}

int
main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(0,0);
	glutCreateWindow(argv[0]);
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	glutReshapeFunc(myReshape);
	myInit();
	glutMainLoop();
	
	return(0);
}

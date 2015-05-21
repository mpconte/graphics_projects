/* a1 - Square Koch curve
Name: Matthew Conte
cs#: cs243082
*/

#include <GL/glut.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define RESOLUTION 512

/* Global Variables */

/*Line co-ordinates*/
static GLfloat mOldX = 0.0;
static GLfloat mOldY = 0.0;

/* recursion level */
static int currentLevel = 0;



void printHelp( char *progname )
{
	fprintf(stdout,
			"\n%s - SQUARE KOCH CURVE\n\n"
			"'a' key to go to the next iteration\n"
			"'s' key to go to previous iteration\n"
			"Escape key    - exit the program\n\n",
			progname);
}


GLvoid init( GLvoid )
{
       		glClearColor( 0.0, 0.0, 0.0, 1.0);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();		
		gluOrtho2D(0.0, 1.0, -0.2, 1.0);
		glMatrixMode(GL_MODELVIEW);
		
}


GLvoid keyboard( GLubyte key, GLint x, GLint y)
{
	
    /* Exit program if escape key is pressed */
    if (key == 27)
	{ 
	exit(0);
	}

    /* Next Iteration if 'a' is pressed */
    else if (key == 97)
    	glutPostRedisplay();
	
    /* Previous Iteration if 's' is pressed */	
    else if (key == 115)
	{
	    if (currentLevel > 1)
		{
		currentLevel -= 2;	
		glutPostRedisplay();	    	
		}
    	}
}


GLvoid drawKoch( GLdouble dir, GLdouble len, GLint n)
{
	GLdouble dirRad = (M_PI / 180) * dir;  	/* convert to radians ((Pi / 180) * degrees)*/
	
	GLfloat newX = mOldX + len * cos(dirRad); /*find new orientation for x*/
			
	GLfloat newY = mOldY + len * sin(dirRad); /*find new orientation for y*/ 

	
	if (n == 0)
	{
		/* Now draw the line here*/
		glVertex2f(mOldX, mOldY);
		glVertex2f(newX, newY);
		mOldX = newX;
		mOldY = newY;
	}
	else
	{
		
			n--;
			
			/*first 3/8 of the line, draw it flat*/
			len *= (3.0/8.0);
			drawKoch(dir, len, n);
			
			/*next 1/4 of the line, adjust length of line to 1/4 &
			adjust angle 90 degrees (up)*/
			dir += 90.0;
			len *= (8.0/3.0);			
			len /= 4.0;
			drawKoch(dir, len, n);
			
			/*adjust angle 90 degrees (right/flat)*/
			dir -= 90.0;
			drawKoch(dir, len, n);
			
			/*adjust angle 90 degrees (down) */
			dir -= 90.0;
			drawKoch(dir, len, n);
			
			/*adjust angle 90 degrees (right/flat) 
			and reset size of the line to 3/8 from 1/4*/
			dir += 90.0;
			len *= 4.0;
			len *= (3.0/8.0);
			drawKoch(dir, len, n);
		
	}
}


GLvoid display( GLvoid )
{
	glClear( GL_COLOR_BUFFER_BIT );

	/* Construct the Square Koch Curve */
	
	glBegin(GL_LINES);
	
	/* blue lines */
	glColor3f(0.3, 0.25, 0.8);
	glLineWidth(4.0);
	
	mOldX = 0.0;
	mOldY = 0.0;
	drawKoch( 0.0, 1.0, currentLevel );
	
	currentLevel++;
	
	glEnd();
	glutSwapBuffers();
}


int main( int argc, char *argv[] )
{
	glutInit( &argc, argv );
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(RESOLUTION, RESOLUTION);
	glutCreateWindow( argv[0] );
	
	printHelp( argv[0] );			
	 	
	glutDisplayFunc( display );
	glutKeyboardFunc( keyboard );  			
	
	init();
	glutMainLoop();

	return 0;
}


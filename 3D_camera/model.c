/*
	This file contains a function that draws a very simple 3D model either
	in wireframe or solid/shaded.

	John Amanatides, Sept. 2007
*/

#ifdef DARWIN
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>

static float white[]= {1.0, 1.0, 1.0, 1.0};
static float red[]= {1.0, 0.0, 0.0, 1.0};
static float green[]= {0.0, 1.0, 0.0, 1.0};
static float blue[]= {0.0, 0.0, 1.0, 1.0};
static float yellow[]= {1.0, 1.0, 0.0, 1.0};
static float ambient[]= {0.2, 0.2, 0.2, 1.0};

/*
 This function draws a simple scene without permanently changing the modelview
  matrix.

 It draws a sphere, teapot, and cone sitting on top of a 5x5 square on
 the x-z plane

 It can draw this model either in wireframe or fully shaded.

*/
void drawModel(int wireframe)
{
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	if(wireframe) {
		glBegin(GL_LINE_LOOP);
			glColor3fv(green);
			glVertex3i(-5, 0, -5);
			glVertex3i( 5, 0, -5);
			glVertex3i( 5, 0,  5);
			glVertex3i(-5, 0,  5);
		glEnd();
		glPushMatrix();
			glColor3fv(red);
			glTranslatef(-3., 0.75, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glutWireSphere(0.75, 10, 10);
		glPopMatrix();
		glPushMatrix();
			glColor3fv(yellow);
			glTranslatef(0.0, 0.8, 0.0);
			glutWireTeapot(1.0);
		glPopMatrix();
		glPushMatrix();
			glColor3fv(blue);
			glTranslatef(3., 0.0, 0.0);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glutWireCone(1.0, 1.5, 16, 8);
		glPopMatrix();
	} else {
		float lightDirection[]= {0.0, 0.7, 0.7, 0.0};

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LIGHTING);
		glEnable(GL_NORMALIZE);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, lightDirection);
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambient);

		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 30);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);

		glBegin(GL_POLYGON);
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, green);
			glNormal3f(0.0, 1.0, 0.0);
			glVertex3i(-5, 0, -5);
			glVertex3i( 5, 0, -5);
			glVertex3i( 5, 0,  5);
			glVertex3i(-5, 0,  5);
		glEnd();
		glPushMatrix();
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, red);
			glTranslatef(-3., 0.75, 0.0);
			glRotatef(90.0, 1.0, 0.0, 0.0);
			glutSolidSphere(0.75, 16, 16);
		glPopMatrix();
		glPushMatrix();
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, yellow);
			glTranslatef(0.0, 0.8, 0.0);
			glutSolidTeapot(1.0);
		glPopMatrix();
		glPushMatrix();
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, blue);
			glTranslatef(3., 0.0, 0.0);
			glRotatef(-90.0, 1.0, 0.0, 0.0);
			glutSolidCone(1.0, 1.5, 16, 8);
		glPopMatrix();

		glDisable(GL_LIGHT0);
		glDisable(GL_LIGHTING);
		glDisable(GL_DEPTH_TEST);
	}
	glPopMatrix();
}

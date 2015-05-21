#ifdef DARWIN
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>

#include "camera.h"

void
drawGrid(int plane, int range)
{
	int i;

	glBegin(GL_LINES);

	switch(plane) {

	case XY:
		glColor3f(0.4, 0.4, 0.4);
		for(i= -range; i <= range; i++) {
			glVertex3i(i, -range, 0);	
			glVertex3i(i,  range, 0);	
		}
		for(i= -range; i <= range; i++) {
			glVertex3i(-range, i, 0);	
			glVertex3i( range, i, 0);	
		}
		glColor3f(0.0, 0.0, 0.0);
		glVertex3i(0, -range, 0);
		glVertex3i(0,  range, 0);
		glVertex3i(-range, 0, 0);
		glVertex3i( range, 0, 0);
		break;
	case XZ:
		glColor3f(0.4, 0.4, 0.4);
		for(i= -range; i <= range; i++) {
			glVertex3i(i, 0, -range);	
			glVertex3i(i, 0,  range);	
		}
		for(i= -range; i <= range; i++) {
			glVertex3i(-range, 0, i);	
			glVertex3i( range, 0, i);	
		}
		glColor3f(0.0, 0.0, 0.0);
		glVertex3i(0, 0, -range);
		glVertex3i(0, 0,  range);
		glVertex3i(-range, 0, 0);
		glVertex3i( range, 0, 0);
		break;
	case YZ:
		glColor3f(0.4, 0.4, 0.4);
		for(i= -range; i <= range; i++) {
			glVertex3i(0, i, -range);	
			glVertex3i(0, i,  range);	
		}
		for(i= -range; i <= range; i++) {
			glVertex3i(0, -range, i);	
			glVertex3i(0,  range, i);	
		}
		glColor3f(0.0, 0.0, 0.0);
		glVertex3i(0, 0, -range);
		glVertex3i(0, 0, range);
		glVertex3i(0, -range, 0);
		glVertex3i(0,  range, 0);
		break;
	default:
		exit(1);
	}
	glEnd();
}

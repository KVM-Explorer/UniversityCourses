#include "cgTriangle.h"
#include <GL/glew.h>//OpenGL¿â

cgTriangle::cgTriangle(void)
{
}


cgTriangle::~cgTriangle(void)
{
}

	

void cgTriangle::Render()
{
	glBegin(GL_TRIANGLES);
		for (int i=0; i<3; i++)
			glVertex3f(Pt[i].x,Pt[i].y,Pt[i].z);
	glEnd();
}
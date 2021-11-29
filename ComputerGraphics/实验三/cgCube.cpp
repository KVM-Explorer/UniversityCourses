#include "cgCube.h"
#include <math.h>

cgCube::cgCube(void)
{
}


cgCube::~cgCube(void)
{
}

void cgCube::SetPos(cgPoint3D pos)
{
	ptPos = pos;
}

void cgCube::InitData(float length)
{
	fLength = length;

	Planes.clear();
	float l = fLength/2;
	float pt[8][3];
	float nor[8][3];
	float text[8][2];


    Plane plane;
    plane.Pt[0] = cgPoint3D(l,-l,-l);
    plane.Pt[1] = cgPoint3D(l,l,-l);
    plane.Pt[2] = cgPoint3D(-l,l,-l);
    plane.Pt[3] = cgPoint3D(-l,-l,-l);
    Planes.push_back(plane);

    plane.Pt[0] = cgPoint3D(-l,-l,-l);
    plane.Pt[1] = cgPoint3D(-l,l,-l);
    plane.Pt[2] = cgPoint3D(-l,l,l);
    plane.Pt[3] = cgPoint3D(-l,-l,l);
    Planes.push_back(plane);

    plane.Pt[0] = cgPoint3D(-l,-l,l);
    plane.Pt[1] = cgPoint3D(-l,l,l);
    plane.Pt[2] = cgPoint3D(l,l,l);
    plane.Pt[3] = cgPoint3D(l,-l,l);
    Planes.push_back(plane);

    plane.Pt[0] = cgPoint3D(l,-l,l);
    plane.Pt[1] = cgPoint3D(l,l,l);
    plane.Pt[2] = cgPoint3D(l,l,-l);
    plane.Pt[3] = cgPoint3D(l,-l,-l);
    Planes.push_back(plane);

    plane.Pt[0] = cgPoint3D(-l,l,l);
    plane.Pt[1] = cgPoint3D(-l,l,-l);
    plane.Pt[2] = cgPoint3D(l,l,-l);
    plane.Pt[3] = cgPoint3D(l,l,l);
    Planes.push_back(plane);

    plane.Pt[0] = cgPoint3D(-l,-l,l);
    plane.Pt[1] = cgPoint3D(-l,-l,-l);
    plane.Pt[2] = cgPoint3D(l,-l,-l);
    plane.Pt[3] = cgPoint3D(l,-l,l);
    Planes.push_back(plane);
}

void cgCube::Render(GLuint texture)
{
	vector<Plane>::iterator it = Planes.begin();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(ptPos.x,ptPos.y,ptPos.z);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture);
	for(; it !=Planes.end(); it++)
	{
        glBegin(GL_QUADS);
           for(int i=0;i<4;i++)
           {
               int idx,idy;
               idx = i/2;
               idy = i%2;
               if(i>1)idy=(i+1)%2;
               glTexCoord2f(idx,idy);
               glNormal3f(0,1,1);
               glVertex3f((*it).Pt[i].x,(*it).Pt[i].y,(*it).Pt[i].z);
           }
       glEnd();
	}
    glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

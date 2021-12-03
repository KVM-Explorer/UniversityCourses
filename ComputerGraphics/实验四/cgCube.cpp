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
    Plane normal;
    normal.Pt[0] = cgPoint3D(0,0,-1);
    normal.Pt[1] = cgPoint3D(0,0,-1);
    normal.Pt[2] = cgPoint3D(0,0,-1);
    normal.Pt[3] = cgPoint3D(0,0,-1);
    Planes.push_back(plane);
    Normal.push_back(normal);

    plane.Pt[0] = cgPoint3D(-l,-l,-l);
    plane.Pt[1] = cgPoint3D(-l,l,-l);
    plane.Pt[2] = cgPoint3D(-l,l,l);
    plane.Pt[3] = cgPoint3D(-l,-l,l);
    normal.Pt[0] = cgPoint3D(-1,0,0);
    normal.Pt[1] = cgPoint3D(-1,0,0);
    normal.Pt[2] = cgPoint3D(-1,0,0);
    normal.Pt[3] = cgPoint3D(-1,0,0);
    Planes.push_back(plane);
    Normal.push_back(normal);

    plane.Pt[0] = cgPoint3D(-l,-l,l);
    plane.Pt[1] = cgPoint3D(-l,l,l);
    plane.Pt[2] = cgPoint3D(l,l,l);
    plane.Pt[3] = cgPoint3D(l,-l,l);
    normal.Pt[0] = cgPoint3D(0,0,1);
    normal.Pt[1] = cgPoint3D(0,0,1);
    normal.Pt[2] = cgPoint3D(0,0,1);
    normal.Pt[3] = cgPoint3D(0,0,1);
    Planes.push_back(plane);
    Normal.push_back(normal);

    plane.Pt[0] = cgPoint3D(l,-l,l);
    plane.Pt[1] = cgPoint3D(l,l,l);
    plane.Pt[2] = cgPoint3D(l,l,-l);
    plane.Pt[3] = cgPoint3D(l,-l,-l);
    normal.Pt[0] = cgPoint3D(1,0,0);
    normal.Pt[1] = cgPoint3D(1,0,0);
    normal.Pt[2] = cgPoint3D(1,0,0);
    normal.Pt[3] = cgPoint3D(1,0,0);
    Planes.push_back(plane);
    Normal.push_back(normal);

    plane.Pt[0] = cgPoint3D(-l,l,l);
    plane.Pt[1] = cgPoint3D(-l,l,-l);
    plane.Pt[2] = cgPoint3D(l,l,-l);
    plane.Pt[3] = cgPoint3D(l,l,l);
    normal.Pt[0] = cgPoint3D(0,1,0);
    normal.Pt[1] = cgPoint3D(0,1,0);
    normal.Pt[2] = cgPoint3D(0,1,0);
    normal.Pt[3] = cgPoint3D(0,1,0);
    Planes.push_back(plane);
    Normal.push_back(normal);

    plane.Pt[0] = cgPoint3D(-l,-l,l);
    plane.Pt[1] = cgPoint3D(-l,-l,-l);
    plane.Pt[2] = cgPoint3D(l,-l,-l);
    plane.Pt[3] = cgPoint3D(l,-l,l);
    normal.Pt[0] = cgPoint3D(0,-1,0);
    normal.Pt[1] = cgPoint3D(0,-1,0);
    normal.Pt[2] = cgPoint3D(0,-1,0);
    normal.Pt[3] = cgPoint3D(0,-1,0);
    Planes.push_back(plane);
    Normal.push_back(normal);
}

void cgCube::Render(GLuint texture)
{
	vector<Plane>::iterator itp = Planes.begin();
    vector<Plane>::iterator itn = Normal.begin();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(ptPos.x,ptPos.y,ptPos.z);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture);
	for(; itp != Planes.end(); itp++,itn++)
	{
        glBegin(GL_QUADS);
           for(int i=0;i<4;i++)
           {
               int idx,idy;
               idx = i/2;
               idy = i%2;
               if(i>1)idy=(i+1)%2;
               glTexCoord2f(idx,idy);
               glNormal3f((*itn).Pt[i].x,(*itn).Pt[i].y,(*itn).Pt[i].z);
               glVertex3f((*itp).Pt[i].x, (*itp).Pt[i].y, (*itp).Pt[i].z);
           }
       glEnd();
	}
    glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}

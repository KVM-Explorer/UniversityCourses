#pragma once
#include "cgCylinder.h"
#include <math.h>
#include <iostream>
cgCylinder::cgCylinder(void)
{
}


cgCylinder::~cgCylinder(void)
{
}

void cgCylinder::SetPos(cgPoint3D pos)
{
	ptPos = pos;
}

void cgCylinder::InitData(float r,float h)
{
	fR = r;
	fH = h;

	vTris.clear();

    int m = 20;

	float dalpha = 2.0f*PI/m;

	float z0 = -h/2;
	float z1 =  h/2;
    float ph = (h+fR)/(h+2*fR);
    float pd = (fR)/(h+2*fR);
	float alpha = 0.0f;
	for (int j=0; j<m; j++)
	{
		float x01 = fR*cos(alpha);
		float y01 = fR*sin(alpha);

		float x02 = fR*cos(alpha+dalpha);
		float y02 = fR*sin(alpha+dalpha);

        float a01 = alpha/M_PI/2.f;
        float a02 = (alpha+dalpha)/M_PI/2.f;

        std::cout<<fR<<std::endl;
        // 构建圆环
		cgTriangle tri; cgTriangle texture;
		tri.Pt[0] = cgPoint3D(x01,y01,z0);
		tri.Pt[1] = cgPoint3D(x02,y02,z0);
		tri.Pt[2] = cgPoint3D(x01,y01,z1);
        texture.Pt[0] = cgPoint3D(a01,pd);
        texture.Pt[1] = cgPoint3D(a02,pd);
        texture.Pt[2] = cgPoint3D(a01,ph);
        sTexture.push_back(texture);
		vTris.push_back(tri);

		tri.Pt[0] = cgPoint3D(x02,y02,z0);
		tri.Pt[1] = cgPoint3D(x02,y02,z1);
		tri.Pt[2] = cgPoint3D(x01,y01,z1);
        texture.Pt[0] = cgPoint3D(a02,pd);
        texture.Pt[1] = cgPoint3D(a02,ph);
        texture.Pt[2] = cgPoint3D(a01,ph);
        sTexture.push_back(texture);
		vTris.push_back(tri);

        // 构建上底
		tri.Pt[0] = cgPoint3D(x01,y01,z1);
		tri.Pt[1] = cgPoint3D(x02,y02,z1);
		tri.Pt[2] = cgPoint3D(0,0,z1);
        texture.Pt[0] = cgPoint3D(a01,ph);
        texture.Pt[1] =cgPoint3D(a02,ph);
        texture.Pt[2] = cgPoint3D((a01+a02)/2,1);
        sTexture.push_back(texture);
		vTris.push_back(tri);

        // 构建下底
		tri.Pt[0] = cgPoint3D(x01,y01,z0);
		tri.Pt[1] = cgPoint3D(x02,y02,z0);
		tri.Pt[2] = cgPoint3D(0,0,z0);
        texture.Pt[0] = cgPoint3D(a01,pd);
        texture.Pt[1] =cgPoint3D(a02,pd);
        texture.Pt[2] = cgPoint3D((a01+a02)/2,0);
        sTexture.push_back(texture);
		vTris.push_back(tri);


		alpha += dalpha;
	}
}

void cgCylinder::Render(GLuint texture)
{
	vector<cgTriangle>::iterator it1 = vTris.begin();
    vector<cgTriangle>::iterator it2 = sTexture.begin();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glTranslatef(ptPos.x,ptPos.y,ptPos.z);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,texture);
	for(; it1 != vTris.end(); it1++,it2++)
	{
        glBegin(GL_TRIANGLES);
        for(int i=0;i<3;i++)
        {
           glTexCoord2f((*it2).Pt[i].s,(*it2).Pt[i].t);
            glNormal3f(0,1,0);
           glVertex3f((*it1).Pt[i].x,(*it1).Pt[i].y,(*it1).Pt[i].z);
        }
        glEnd();
	}
    glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}
#pragma once
#include "cgSphere.h"
#include <math.h>

cgSphere::cgSphere(void)
{
	ptPos = cgPoint3D();
}


cgSphere::~cgSphere(void)
{
}

void cgSphere::SetPos(cgPoint3D pos)
{
	ptPos = pos;
}

void cgSphere::InitData(float r)
{
	fR = r;

    RectArray.clear();
    FArray.clear();
    int m = 20;
	int n = 20;

	float dalpha = 2*PI/m;
	float dbeta  = PI/n;

	float beta = -PI/2.0f;
	for (int i=0; i<n;i++)
	{
		float alpha = 0.0f;

		float z1 = fR*sin(beta);
		float z2 = fR*sin(beta+dbeta);

		float r0 = fR*cos(beta);
		float r1 = fR*cos(beta+dbeta);

		beta += dbeta;
		for (int j=0; j<m; j++)
		{
			float x01 = r0*cos(alpha);
			float y01 = r0*sin(alpha);

			float x02 = r0*cos(alpha+dalpha);
			float y02 = r0*sin(alpha+dalpha);

			float x11 = r1*cos(alpha);
			float y11 = r1*sin(alpha);

			float x12 = r1*cos(alpha+dalpha);
			float y12 = r1*sin(alpha+dalpha);

			Rect rect;
			rect.Pt[0] = cgPoint3D(x01,y01,z1);
			rect.Pt[1] = cgPoint3D(x02,y02,z1);
			rect.Pt[2] = cgPoint3D(x12,y12,z2);
            rect.Pt[3] = cgPoint3D(x11,y11,z2);
            RectArray.push_back(rect);

            F f;
            f.Pt[0] = cgPoint3D(alpha/2.f/M_PI,beta/M_PI);
            f.Pt[1] = cgPoint3D((alpha+dalpha)/M_PI/2.f,beta/M_PI);
            f.Pt[2] = cgPoint3D((alpha+dalpha)/2.f/M_PI,(beta+dbeta)/M_PI);
            f.Pt[3] = cgPoint3D(alpha/2.f/M_PI,(beta+dbeta)/M_PI);
            FArray.push_back(f);

			alpha += dalpha;
		}
	}
}

void cgSphere::Render(GLuint texture)
{
	vector<Rect>::iterator it1 = RectArray.begin();
    vector<F>::iterator it2 = FArray.begin();

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glRotatef(90,1,0,0);
	glTranslatef(ptPos.x,ptPos.y,ptPos.z);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,texture);
	for(; it1 != RectArray.end(); it1++,it2++)
	{
        glBegin(GL_POLYGON);
        for(int i=0;i<4;i++)
        {
            glTexCoord2f((*it2).Pt[0].s,(*it2).Pt[0].t);
            glVertex3f((*it1).Pt[0].x,(*it1).Pt[0].y,(*it1).Pt[0].z);

            glTexCoord2f((*it2).Pt[1].s,(*it2).Pt[1].t);
            glVertex3f((*it1).Pt[1].x,(*it1).Pt[1].y,(*it1).Pt[1].z);

            glTexCoord2f((*it2).Pt[2].s,(*it2).Pt[2].t);
            glVertex3f((*it1).Pt[2].x,(*it1).Pt[2].y,(*it1).Pt[2].z);

            glTexCoord2f((*it2).Pt[3].s,(*it2).Pt[3].t);
            glVertex3f((*it1).Pt[3].x,(*it1).Pt[3].y,(*it1).Pt[3].z);

        }
        glEnd();
	}
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
}



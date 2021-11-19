#pragma once
#include "cgPoint3D.h"
#include "cgTriangle.h"
#include <GL/glew.h>
#include<vector>
using namespace std;

class cgCylinder
{
public:
	cgCylinder(void);
	~cgCylinder(void);

	void InitData(float r,float h);
	void Render(GLuint texture);
	void SetPos(cgPoint3D pos);	

private:
	float fR;
	float fH;

	cgPoint3D ptPos;
	vector<cgTriangle> vTris;
    vector<cgTriangle> sTexture;
};


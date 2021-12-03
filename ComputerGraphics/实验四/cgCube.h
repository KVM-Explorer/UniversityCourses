#pragma once
#include "cgPoint3D.h"
#include<vector>
#include <GL/glew.h>
using namespace std;

class cgCube
{
public:
	cgCube(void);
	~cgCube(void);

	void InitData(float length);
	void Render(GLuint texture);
	void SetPos(cgPoint3D pos);	
    struct Plane
    {
        cgPoint3D Pt[4];
    };
private:
	float fLength;
	cgPoint3D ptPos;
	vector<Plane> Planes;
    vector<Plane> Normal;
};


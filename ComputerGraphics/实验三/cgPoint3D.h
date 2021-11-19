#pragma once

#define PI 3.14259

class cgPoint3D
{
public:
	cgPoint3D(void);
	cgPoint3D(float xx,float yy,float zz);
    cgPoint3D(float s,float t);
	~cgPoint3D(void);

	float x;
	float y;
	float z;

	float nX;
	float nY;
	float nZ;

	float s;
	float t;
};



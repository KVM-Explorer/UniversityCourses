#pragma once
#include "cgPoint3D.h"
#include<vector>
#include <GL/glew.h>
using namespace std;

class cgSphere
{
public:
	cgSphere(void);
	~cgSphere(void);

	void InitData(float r);
	void Render(GLuint texture);
	void SetPos(cgPoint3D pos,float d,float angle);
    struct Rect
    {
        cgPoint3D Pt[4];
    };
    struct F
    {
        cgPoint3D Pt[4];
    };
private:
	float fR;
    float D;
    float Angle;
	cgPoint3D ptPos; 
	vector<Rect> RectArray;
    vector<F> FArray;
    vector<Rect> Normal;
};


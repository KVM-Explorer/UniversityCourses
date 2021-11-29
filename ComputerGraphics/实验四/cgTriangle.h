#pragma once
#include "cgPoint3D.h"

class cgTriangle
{
public:
	cgTriangle(void);
	~cgTriangle(void);

	cgPoint3D Pt[3];

	void Render();
};


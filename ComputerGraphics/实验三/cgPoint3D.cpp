#include "cgPoint3D.h"


cgPoint3D::cgPoint3D(void)
{
	x = y = z = 0.0f;
}

cgPoint3D::cgPoint3D(float xx,float yy,float zz)
{
	x = xx;
	y = yy;
	z = zz;
}
cgPoint3D::cgPoint3D(float s,float t) {
    this->s = s;
    this->t = t;
}
cgPoint3D::~cgPoint3D(void)
{
}

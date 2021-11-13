#include <GL/glut.h>
#include <GL/glu_mangle.h>
#include <opencv2/opencv.hpp>
using namespace std;

cv::Vec3f CameraPostion= cv::Vec3f(0,0,3);
cv::Vec3f CameraTarget= cv::Vec3f(0,0,0) ;
cv::Vec3f CameraDirection = CameraPostion-CameraTarget;

cv::Vec3f UpVector = cv::Vec3f(0,1,0);
cv::Vec3f RightVectot = cv::normalize(UpVector.cross(CameraDirection));
cv::Matx44f View;

int main()
{
}
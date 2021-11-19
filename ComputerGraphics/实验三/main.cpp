
#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "cgSphere.h"
#include "cgCube.h"
#include "cgCylinder.h"
#include <GL/glu.h>
#include <GL/glut.h>
using namespace std;

float Header[]={0,0,-1}; // 观察位置相对目标位置偏移量
float Position[]={3,0,5}; // 目标位置

float step = 10;  // 平移步长
float Angle = 0;    // 初始绝对角度
float AngleStep = 5; // 旋转步长

GLuint textureID1;
GLuint textureGround;
GLuint textureSphere;
GLuint textureSky;
GLuint textureWall;
GLuint textureFun;
cgSphere Sphere;
cgCube Cube;
cgCylinder Cylinder;

void RotatedLookAt(float theta)
{
    auto angle = theta/180*M_PI;
    float  p = Header[2];
    Header[0] = sin(angle);
    Header[2] = -cos(angle);
}


GLuint LoadTexture(char* name)
{
    GLuint tID;

    int width ,height,channels;
    auto image = cv::imread(name);
    width = image.cols;
    height = image.rows;
    channels = image.channels();

    glGenTextures(1,&tID);
    glBindTexture(GL_TEXTURE_2D,tID);

    glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    // 多层采样
    gluBuild2DMipmaps(GL_TEXTURE_2D,
                      GL_RGB,
                      width,
                      height,
                      GL_BGR,
                      GL_UNSIGNED_BYTE,image.data);
    // 直接构建纹理
//    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_BGR_EXT,GL_UNSIGNED_BYTE,image.data);

    GLenum err;
    if ((err=glGetError())!=GL_NO_ERROR)
    {
        std::cout<<err<<std::endl;
        exit(0);
    }

    return tID;

}


void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float at[3];
    for (int i=0;i<3;i++)
    {
        at[i] = Position[i]+Header[i];
    }

    gluLookAt(Position[0],Position[1],Position[2],
              at[0],at[1],at[2],
              0,1,0);
    std::cout<<Header[0]<<' '<<Header[1]<<' '<<Header[2]<<std::endl;

    // 天空远景
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, textureGround);
//    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
//    glColor3f(1,0,1);
//    glBegin(GL_POLYGON);
//        glTexCoord2f(0,0);glVertex3f(-500,-200,-500);
//        glTexCoord2f(0,10);glVertex3f(500,-200,-500);
//        glTexCoord2f(10,10);glVertex3f(500,-200,500);
//        glTexCoord2f(10,0);glVertex3f(-500,-200,500);
//    glEnd();

    //  地面
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D,textureSphere);
//    glBegin(GL_QUADS);
//    glTexCoord2f(0,0);
//    glVertex3f(-500,500,0);
//    glTexCoord2f(0,10);
//    glVertex3f(500,500,0);
//    glTexCoord2f(10,10);
//    glVertex3f(500,-200,0);
//    glTexCoord2f(10,0);
//    glVertex3f(-500,-200,0);
//    glEnd();
//    glDisable(GL_TEXTURE_2D);

    //  绘制球、正方体、圆柱体
    Sphere.Render(textureFun);
    Cube.Render(textureFun);
    Cylinder.Render(textureID1);

    glutSwapBuffers();
}

void Controller()
{

    glutPostRedisplay();
}

void reshape(int width,int height)
{
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(60,(GLfloat )width/(GLfloat)height,1,2000);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void keyBoard(unsigned char key,int x,int y)
{
    std::cout<<key<<std::endl;
    switch (key) {
        case 'w':
        case 'W':
            for(int i=0;i<3;i++) Position[i]+=step*Header[i];
            break;
        case 's':
        case 'S':
            for(int i=0;i<3;i++) Position[i]-=step*Header[i];
            break;
        case 'a':
        case 'A':
            {
                Position[0]-=step;
                break;
            }
        case 'd':
        case 'D':
            {
               Position[0]+=step;
                break;
            }
        case 'q':
        case 'Q':
            Angle -=AngleStep;
            RotatedLookAt(Angle);
            break;
        case 'e':
        case 'E':
            Angle += AngleStep;
            RotatedLookAt(Angle);
            break;
        case 'z':
        case 'Z':
            Header[1] -= 0.01;
            break;
        case 'C':
        case 'c':
            Header[1] += 0.01;
            break;
        case 'X':
        case 'x':
            Position[1] -=step;
            break;
        case ' ':
            Position[1]+=step;

    }
    glutPostRedisplay();
}
void init()
{
    textureID1 = LoadTexture("timg.jpg");
    textureGround = LoadTexture("pattern.jpeg");
    textureSphere = LoadTexture("Emjo.png");
    textureSky = LoadTexture("sky.png");
    textureWall = LoadTexture("wall.png");
    textureFun = LoadTexture("FUN.png");
    Sphere.InitData(30);
    Sphere.SetPos(cgPoint3D(10.f,20.f,30.f));
    Cube.InitData(30);
    Cube.SetPos(cgPoint3D(50,50,30));
    Cylinder.InitData(10,20);
    Cylinder.SetPos(cgPoint3D(-10,-10,-10));
}
int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitWindowPosition(200,200);
    glutInitWindowSize(800,800);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    glutCreateWindow("Class3-CameraAndPattern");
    glEnable(GL_DEPTH_TEST);
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(Controller);
    glutKeyboardFunc(keyBoard);
    glutMainLoop();
}
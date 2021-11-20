
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

float step = 1;  // 平移步长
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
cgCube LightBox;


//光照变量
GLfloat light_position[] = { 0, 2, 0, 0.1 };
GLfloat light_color[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat aMaterial[] = { .20, 0.20, 0.20, 1.0 }; //环境光反射系数
GLfloat dMaterial[] = { .60, 0.60, 0.60, 1.0 }; //漫反射光反射系数
GLfloat sMaterial[] = { 0.8, 0.8,0.8, 1 }; //镜面反射光反射系数
GLfloat shiniess = 20;//高光指数


void RotatedLookAt(float theta)
{
    auto angle = theta/180*M_PI;
    float  p = sqrt(Header[2]*Header[2]+Header[0]*Header[0]);
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

    glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT );//清除帧缓存和深度缓存

    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);

    glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT , aMaterial);
    glMaterialfv(GL_FRONT_AND_BACK,GL_DIFFUSE , dMaterial);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR, sMaterial);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS, &shiniess);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    //模型观察矩阵初始化
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float at[3];
    for (int i=0; i<3; i++)
        at[i] = Position[i] + Header[i];
    gluLookAt (Position[0], Position[1], Position[2], at[0], at[1], at[2], 0.0, 1.0, 0.0);

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    //地面
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureWall);
    glBegin(GL_QUADS);
    glColor3f(1.0f,1.0f,1.0f);

    glTexCoord2f(0.0f,0.0f);//指定顶点纹理坐标
    glNormal3f(0,1,0);//指定顶点法向量
    glVertex3f(-50.0f,0.0f,50.0f);//指定顶点坐标

    glTexCoord2f(1.0f,0.0f);
    glNormal3f(0,1,0);
    glVertex3f(50.0f,0.0f,50.0f);

    glTexCoord2f(1.0f,1.0f);
    glNormal3f(0,1,0);
    glVertex3f(50.0f,0.0f,-50.0f);

    glTexCoord2f(0.0f,1.0f);
    glNormal3f(0,1,0);
    glVertex3f(-50.0f,0.0f,-50.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);

//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    //光照
//    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
//    glLightfv(GL_LIGHT0, GL_AMBIENT, light_color);
//    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
//    glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
//
//    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, aMaterial);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, dMaterial);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, sMaterial);
//    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shiniess);
//
//    glEnable(GL_LIGHTING);
//    glEnable(GL_LIGHT0);
//    for(int i=0;i<3;i++) std::cout<<light_position[i]<<' ';
//    std::cout<<endl;
//
//    GLenum err;
//    if ((err=glGetError())!=GL_NO_ERROR)
//    {
//        std::cout<<err<<std::endl;
//        exit(0);
//    }
//
//    // 观察矩阵初始化
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//
//    float at[3];
//    for (int i=0;i<3;i++)
//    {
//        at[i] = Position[i]+Header[i];
//    }
//
//    gluLookAt(Position[0],Position[1],Position[2],
//              at[0],at[1],at[2],
//              0,1,0);
//    std::cout<<Header[0]<<' '<<Header[1]<<' '<<Header[2]<<std::endl;
//
//    // 地面
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, textureGround);
//    glColor3f(1,1,1);
//    glBegin(GL_POLYGON);
//    glTexCoord2f(0,0);
//    glNormal3f(0,1,0);
//    glVertex3f(-50,-20,-50);
//
//    glTexCoord2f(0,10);
//    glNormal3f(0,1,0);
//    glVertex3f(50,-20,-50);
//
//    glTexCoord2f(10,10);
//    glNormal3f(0,1,0);
//    glVertex3f(50,-20,50);
//
//    glTexCoord2f(10,0);
//    glNormal3f(0,1,0);
//    glVertex3f(-50,-20,50);
//    glEnd();
//    glDisable(GL_TEXTURE_2D);

//   天空
//    glEnable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D,textureSky);
//    glBegin(GL_QUADS);
//    glTexCoord2f(0,0);
//    glNormal3f(0,1,0);
//    glVertex3f(-500,500,-200);
//    glTexCoord2f(0,10);
//    glNormal3f(0,1,0);
//    glVertex3f(500,500,-200);
//    glTexCoord2f(10,10);
//    glNormal3f(0,1,0);
//    glVertex3f(500,-200,-200);
//    glTexCoord2f(10,0);
//    glNormal3f(0,1,0);
//    glVertex3f(-500,-200,-200);
//    glEnd();
//    glDisable(GL_TEXTURE_2D);
//
//    //  绘制球、正方体、圆柱体
//    Sphere.Render(textureSphere);
//    Cube.Render(textureWall);
////    Cylinder.Render(textureFun);

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

    gluPerspective(60,(GLfloat )width/(GLfloat)height,1,200);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void keyBoard(unsigned char key,int x,int y)
{
//    std::cout<<key<<std::endl;
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
            break;
        case 'i':
        case 'I':
            light_position[1] += 1;
            break;
        case 'k':
        case 'K':
            light_position[1] -=1;
            break;
        case 'n':
            light_color[0] = 0.0;
            light_color[1] = 1.0;
            light_color[2] = 1.0;
            break;
        case 'm':
            light_color[0] = 1.0;
            light_color[1] = 1.0;
            light_color[2] = 0.0;
            break;
        case 'b'://原来颜色
            light_color[0] = 1.0;
            light_color[1] = 1.0;
            light_color[2] = 1.0;
            break;

    }
    glutPostRedisplay();
}
void init()
{
    glClearColor(0.5, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
//    textureID1 = LoadTexture("timg.jpg");
//    textureGround = LoadTexture("timg.jpg");
//    textureSphere = LoadTexture("Emjo.png");
//    textureSky = LoadTexture("sky.png");
    textureWall = LoadTexture("wall.png");
//    textureFun = LoadTexture("FUN.png");
//    Sphere.InitData(30);
//    Sphere.SetPos(cgPoint3D(10.f,20.f,30.f));
//    Cube.InitData(30);
//    Cube.SetPos(cgPoint3D(50,50,30));
//    Cylinder.InitData(10,20);
//    Cylinder.SetPos(cgPoint3D(-10,-10,-10));
//    LightBox.InitData(5);
}
int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitWindowPosition(200,200);
    glutInitWindowSize(800,800);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE| GLUT_DEPTH );
    glutCreateWindow("Class4-Light");
    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(Controller);
    glutKeyboardFunc(keyBoard);
    glutMainLoop();
}
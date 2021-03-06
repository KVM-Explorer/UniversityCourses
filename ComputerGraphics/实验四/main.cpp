#include <GL/glew.h>//OpenGL库
#include <GL/glut.h>//OpenGL辅助库
#include <SOIL/SOIL.h>
#include <math.h>
#include <opencv2/opencv.hpp>
#include "cgSphere.h"
#include "cgCylinder.h"
#include "cgCube.h"
#include <GL/glu.h>
#include <GL/glut.h>
using namespace std;

float alpha=0.0f;
float pos[]={3,2.0,5};
float headdir[]={0.0f,0.0f,-1.0f};
float rightdir[]={0.1f,0.0f,0.0f};

float Header[]={0,0,-1}; // 观察位置相对目标位置偏移量
float Position[]={3,10,30}; // 目标位置

float step = 1;  // 平移步长
float Angle = 0;    // 初始绝对角度
float AngleStep = 5; // 旋转步长
float RotatedAngle = 0;
GLuint textureID1;
GLuint textureGround;
GLuint textureSphere;
GLuint textureSky;
GLuint textureWall;
GLuint textureFun;
cgSphere Sphere;
cgCube Cube;
cgCylinder Cylinder;
//cgCube LightBox;


//光照变量
GLfloat light_position[] = { 0, 2, 0, 0.1 };
GLfloat light_color[] = { 1.0, 1.0, 1.0, 1.0 };

GLfloat aMaterial[] = { .20, 0.20, 0.20, 1.0 }; //环境光反射系数
GLfloat dMaterial[] = { .60, 0.60, 0.60, 1.0 }; //漫反射光反射系数
GLfloat sMaterial[] = { 0.8, 0.8,0.8, 1 }; //镜面反射光反射系数
GLfloat shiniess = 20;//高光指数
float headdir_buffer[] = {0.0,0.0,0.0};
int rendermode = 0;//0:填充； 1:线框；2：纹理
float turn_step = 0.025f;
GLuint textureID;
GLuint texture2;
GLuint texture3;
GLuint texture4;
int  frames=0;

GLuint LoadTexture(char* name)
{
    GLuint tID;

    int width ,height;
    auto image = cv::imread(name);
    width = image.cols;
    height = image.rows;

    glGenTextures(1,&tID);
    glBindTexture(GL_TEXTURE_2D,tID);


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
        at[i] = pos[i] + headdir[i];
    gluLookAt (pos[0], pos[1], pos[2], at[0], at[1], at[2], 0.0, 1.0, 0.0);

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    //地面
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID1);
    glBegin(GL_POLYGON);
//    glColor3f(1.0f,1.0f,1.0f);

    glTexCoord2f(0.0f,0.0f);//指定顶点纹理坐标
    glNormal3f(0,1,0);//指定顶点法向量
    glVertex3f(-50.0f,-1.0f,50.0f);//指定顶点坐标

    glTexCoord2f(1.0f,0.0f);
    glNormal3f(0,1,0);
    glVertex3f(50.0f,-1.0f,50.0f);

    glTexCoord2f(1.0f,1.0f);
    glNormal3f(0,1,0);
    glVertex3f(50.0f,-1.0f,-50.0f);

    glTexCoord2f(0.0f,1.0f);
    glNormal3f(0,1,0);
    glVertex3f(-50.0f,-1.0f,-50.0f);
    glEnd();
    glDisable(GL_TEXTURE_2D);




    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,textureWall);
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);
    glNormal3f(0,1,0);
    glVertex3f(-50,50,-20);
    glTexCoord2f(0,10);
    glNormal3f(0,1,0);
    glVertex3f(50,50,-20);
    glTexCoord2f(10,10);
    glNormal3f(0,1,0);
    glVertex3f(50,-20,-20);
    glTexCoord2f(10,0);
    glNormal3f(0,1,0);
    glVertex3f(-50,-20,-20);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    //  绘制球、正方体、圆柱体

    Sphere.Render(textureFun);
    Cube.Render(textureWall);
    Cylinder.Render(textureSky);
    glutSwapBuffers();
}
void Controller()
{
    RotatedAngle+=1;
    Sphere.SetPos(cgPoint3D(0,2,0) ,20,RotatedAngle);
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

void keyboard(unsigned char key, int x, int y)
{
    switch (key) {
        case 'w':
            for (int i=0; i<3; i++)
                pos[i] += step*headdir[i];
            break;
        case 'S':   //下移
        case 's':
            for (int i=0; i<3; i++)
                pos[i] -= step*headdir[i];
            break;
        case 'A':  //左移
        case 'a':
            for (int i=0; i<3; i++)
                pos[i] -= step*rightdir[i]*5;
            break;
        case 'D':   //右移
        case 'd':
            for (int i=0; i<3; i++)
                pos[i] += step*rightdir[i]*5;
            break;

        case 'Z':
        case 'z':
            //̧光源
            //pos[1] += .30f;
            light_position[1] += 1.0;
            break;
        case 'j':
        case 'J':
            // 光源左
            light_position[0]-=1.0;
            break;

        case ' l':
        case 'L':
            // 光源右
            light_position[0]+=1.0;
            break;
        case 'i':
        case 'I':
            light_position[2]-=1.0;
            break;
        case 'k':
        case 'K':
            light_position[2]+=1.0;
            break;
        case 'X':
        case 'x':
            //光源
            //pos[1] -= .30f;
            light_position[1] -= 1.0;
            break;
        case 'N':   //蓝
        case 'n':
            light_color[0] = 0.0;
            light_color[1] = 0.0;
            light_color[2] = 1.0;
            break;
        case 'M':   //黄
        case 'm':
            light_color[0] = 1.0;
            light_color[1] = 1.0;
            light_color[2] = 0.0;
            break;
        case 'o'://原来颜色
            light_color[0] = 1.0;
            light_color[1] = 1.0;
            light_color[2] = 1.0;
            break;
        case ' ':
            pos[1]+=step;
            break;
    }

    glutPostRedisplay();
}

void init()
{
    glClearColor(0.5, 1.0, 1.0, 1.0);
    glEnable(GL_DEPTH_TEST);
    textureID1 = LoadTexture("timg.jpg");
    textureGround = LoadTexture("timg.jpg");
    textureSphere = LoadTexture("Emjo.png");
    textureSky = LoadTexture("sky.png");
    textureWall = LoadTexture("wall.png");
    textureFun = LoadTexture("rose.png");
    Sphere.InitData(10);
    Sphere.SetPos(cgPoint3D(0,2,0),20 ,0);
    Cube.InitData(4);
    Cube.SetPos(cgPoint3D(10,0,-5));
    Cylinder.InitData(3,6);
    Cylinder.SetPos(cgPoint3D(-10,0,-5));
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
    glutKeyboardFunc(keyboard);
    glutMainLoop();
    return 0;
}

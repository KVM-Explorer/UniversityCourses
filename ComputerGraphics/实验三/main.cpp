#pragma once
#include <GL/glut.h>
#include <GL/glu.h>
#include <iostream>
#include <math.h>
#include <opencv2/opencv.hpp>
#include <SOIL/SOIL.h>
using namespace std;

float Header[]={0,0,-1};
float Position[]={3,0,5};

float alpha = 0;
float step = 10;
float Angle = 90;
float AngleStep = 1;

GLuint textureID;

GLuint LoadTexture(char* name)
{
    GLuint tID;

    int width ,height,channels;
    auto image = cv::imread(name);
    width = image.cols;
    height = image.rows;
    channels = image.channels();
    int pixel_length = width*height*channels;
    auto pixels = new GLubyte [pixel_length];

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
    free(pixels);

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

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    float at[3];
    for (int i=0;i<3;i++)
    {
        at[i] = Position[i]+Header[i];
    }

    gluLookAt(Position[0],Position[1],Position[2],
              0,0,0,
              0,1,0);
    std::cout<<at[0]<<' '<<at[1]<<' '<<at[2]<<"   Position"<<Position[0]<<' '<<Position[1]<<' '<<Position[2]<<std::endl;
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D,textureID);

    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glColor3f(1,0,0);
    {
        glBegin(GL_TRIANGLES);
        {
            glTexCoord2f(0,0);glVertex3f(-30,-30,0);
            glTexCoord2f(0.5,1);glVertex3f(0,30,0);
            glTexCoord2f(1,0);glVertex3f(30,-30,0);
        }
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    glColor3f(1,1,1);
    {
        glBegin(GL_POLYGON);
        for (int i = 0; i < 200; i++) {
            glVertex3f(2*cos(2*M_PI/200*i),2*sin(2*M_PI/200*i),0);
        }
        glEnd();
    }
    glutSwapBuffers();
}

void Controller()
{
    alpha +=2.f;
    if(alpha>360) alpha-=360;

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
    std::cout<<key<<std::endl;
    switch (key) {
        case 'W':
            for(int i=0;i<3;i++) Position[i]+=step*Header[i];
            break;
        case 'S':
            for(int i=0;i<3;i++) Position[i]-=step*Header[i];
            break;
        case 'A':
            {
                Angle +=AngleStep;
                float radius = sqrt(Position[0]*Position[0]+Position[2]*Position[2]);
                Position[0]=radius*cos(Angle/180*M_PI);
                Position[2]=radius*sin(Angle/180*M_PI);
                break;
            }
        case 'D':
            {
                Angle -=AngleStep;
                float radius = sqrt(Position[0]*Position[0]+Position[2]*Position[2]);
                Position[0]=radius*cos(Angle/180*M_PI);
                Position[2]=radius*sin(Angle/180*M_PI);
                break;
            }

    }
    glutPostRedisplay();
}

int main(int argc,char** argv)
{
    glutInit(&argc,argv);
    glutInitWindowPosition(200,200);
    glutInitWindowSize(800,800);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    glutCreateWindow("Class3-CameraAndPattern");
    glEnable(GL_DEPTH_TEST);

    textureID = LoadTexture("timg.jpg");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(Controller);
    glutKeyboardFunc(keyBoard);
    glutMainLoop();
}
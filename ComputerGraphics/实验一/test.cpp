#pragma once
//#include <GL/glext.h>
#include <GL/glut.h>
#include <iostream>

void init()
{

    glClearColor(0.2,0.3,0.3,1.0);
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT); // 清除帧缓存
    glBegin(GL_TRIANGLES);
        glColor3f(1.0f,0.0f,0.0f);// 指定顶点颜色
        glVertex2f(100.0f,100.0f);  //指定顶点坐标
        glColor3f(0.0f,1.0f,0.0f);
        glVertex2f(500.0f,100.0f);
        glColor3f(0.0f,0.0f,1.0f);
        glVertex2f(300.0f,400.0f);
        std::cout<<"Loading";
    glEnd();

    glutSwapBuffers();
}

void reshape(int width,int height)
{
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,width,0,height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitWindowPosition(200,200);
    glutInitWindowSize(800,600);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    glutCreateWindow("Project One");



    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    glutMainLoop();
}
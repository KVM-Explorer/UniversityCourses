#pragma once
#include <GL/glut.h>
#include <math.h>
#include <iostream>

float DegreeToRad(float angle)
{
    return M_PI*angle/180.f;
}

void DrawOutLine(int x,int y,int r,int l,int angle)
{
    glBegin(GL_LINES);
    float x1 = x + r * cos(DegreeToRad(angle+36));
    float y1 = y + r * sin(DegreeToRad(angle+36));
    glColor3f(1.0f,0.0,0.0f);
    glVertex2f(x1,y1);

    float x2 = x + l * cos(DegreeToRad(angle));
    float y2 = y + l * sin(DegreeToRad(angle));
    glColor3f(1.0f,0.0,0.0f);
    glVertex2f(x2,y2);

    glColor3f(0,1,0);
    glVertex2f(x,y);
    glColor3f(0,1,0);
    glVertex2f(x2,y2);

    glColor3f(1.0f,0.0,0.0f);
    glVertex2f(x2,y2);
    float x3 = x + r * cos(DegreeToRad(angle-36));
    float y3 = y + r * sin(DegreeToRad(angle-36));
    glColor3f(1.0f,0.0,0.0f);
    glVertex2f(x3,y3);
    glEnd();
}

void DrawFill(int x,int y,int r,int l,int angle,int type) {
    if (type < 2)
    {
        glBegin(GL_TRIANGLES);
        // 单方向四点坐标求解
        float x1 = x + r * cos(DegreeToRad(angle+36));
        float y1 = y + r * sin(DegreeToRad(angle+36));

        float x2 = x + l * cos(DegreeToRad(angle));
        float y2 = y + l * sin(DegreeToRad(angle));

        float x3 = x + r * cos(DegreeToRad(angle-36));
        float y3 = y + r * sin(DegreeToRad(angle-36));

        // 上半三角形
        if(type==0)glColor3f(1.0f,0.0,0.0f);
        glVertex2f(x1,y1);

        if(type==0)glColor3f(1.0f,0.0,0.0f);
        glVertex2f(x2,y2);

        if(type==0)glColor3f(0,1,0);
        glVertex2f(x,y);

        // 下半三角形
        if(type==0)glColor3f(0,1,0);
        glVertex2f(x2,y2);

        if(type==0)glColor3f(1.0f,0.0,0.0f);
        glVertex2f(x,y);

        if(type==0)glColor3f(1.0f,0.0,0.0f);
        glVertex2f(x3,y3);
        glEnd();
    }
    else
    {
        glBegin(GL_LINES);
        float x1 = x + r * cos(DegreeToRad(angle+36));
        float y1 = y + r * sin(DegreeToRad(angle+36));

        float x2 = x + l * cos(DegreeToRad(angle));
        float y2 = y + l * sin(DegreeToRad(angle));

        float x3 = x + r * cos(DegreeToRad(angle-36));
        float y3 = y + r * sin(DegreeToRad(angle-36));

        glVertex2f(x,y);
        glVertex2f(x1,y1);

        glVertex2f(x1,y1);
        glVertex2f(x2,y2);



        glEnd();
    }

}


void Frame()
{
    glClear(GL_COLOR_BUFFER_BIT);

    float R = 100;
    int n = 80;     //这里的n表示用多边形绘制圆的精度，可以考虑增大精度
    float PI = M_PI;
    std::cout<<"Drawing"<<std::endl;
    for(int i=18;i<360; i+=72)
    {
        // 绘制轮廓
//        DrawOutLine(400,300,100,300,i);

        // 绘制填充图形
        DrawFill(300,300,30,90,i,2);
        DrawFill(200,200,30,90,i,1);
        DrawFill(100,100,30,90,i,0);

    }

    glutSwapBuffers();

}

void Reshape(int width,int height)
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

    glutCreateWindow("HomeWork One");

    glutDisplayFunc(Frame);
    glutReshapeFunc(Reshape);

    glutMainLoop();
}


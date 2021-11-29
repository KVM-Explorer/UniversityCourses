#pragma once
#include <GL/glut.h>
#include <math.h>
#include <iostream>
#include <chrono>
#include <unistd.h>
float DegreeToRad(float angle)
{
    return M_PI*angle/180.f;
}

void DrawStar(int x,int y,int r,int l,int compsole)
{

    for (int angle = 0 + compsole; angle <360+compsole; angle += 72)
//    int angle =0;
    {
        glBegin(GL_POLYGON);
        // 单方向四点坐标求解
            srand(0);
            glColor3f(0,1,0);
            glVertex2f(x,y);
            for(int delta = -36 ; delta<=36;delta+=36)
            {
                float x1 = x + r * cos(DegreeToRad(angle+delta));
                float y1 = y + r * sin(DegreeToRad(angle+delta));
                if (delta == 0)
                {
                    x1 = x + l * cos(DegreeToRad(angle+delta));
                    y1 = y + l * sin(DegreeToRad(angle+delta));
                }
                srand(time(0));
//                std::cout<<rand()%255/255.f<<' '<<rand()%255/255.f<<' '<<rand()%255/255.f<<std::endl;
                glColor3f(rand()%255/255.f,rand()%255/255.f,rand()%255/255.f);
                glVertex2f(x1,y1);
            }
        glEnd();
    }

}

void DrawRect()
{
    glColor3f(1,1,0);
    glBegin(GL_LINE_LOOP);
        glVertex2f(100,100);
        glVertex2f(100,700);
        glVertex2f(700,700);
        glVertex2f(700,100);
    glEnd();
}

int Angle = 0;
int X=100,Y=100;
int DeltaX=1,DeltaY=1;
float Scale = 1,DeltaScale=0.1;
void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glPushMatrix();
    glTranslatef(X,Y,0);
    glRotatef(Angle,0,0,1);
    glScaled(Scale,Scale,0);
    glTranslatef(-X,-Y,0);
    DrawStar(X,Y,30,100,18);
    glPopMatrix();

    // 绘制缩放变换
    glPushMatrix();
    glTranslatef(400,400,0);
    glRotatef(Angle,0,0,1);
    glScaled(Scale,Scale,0);
    glTranslatef(-400,-400,0);
    DrawStar(400,400,20,60,18);

    glPopMatrix();

    // 画矩形框
    DrawRect();

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


void Controller()
{
    Angle++;
    X = X +DeltaX;
    Y = Y +DeltaY;
    if(X+100>700 || X-100<100) DeltaX = -DeltaX;
    if(Y+100>700 || Y-100<100) DeltaY = -DeltaY;
    Scale = Scale + DeltaScale;
    if(Scale >1 || Scale<0.5) DeltaScale  = -DeltaScale;
//    sleep(1);
    glutPostRedisplay();
}

int main(int argc,char **argv)
{
    glutInit(&argc,argv);
    glutInitWindowPosition(200,200);
    glutInitWindowSize(800,800);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

    // 移动速度
    DeltaX=1;
    DeltaY=2;
    // 起始坐标
    X=200,Y=200;
    Scale = 1;
    DeltaScale = 0.02;
    glutCreateWindow("Project Two");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(Controller);
    glutMainLoop();
}
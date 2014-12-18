//-------------------------------------------------------------------------------------------
//  University of Bath Computer Science Department
//	FrameWork for OpenGL application under QT
//  Course title: Computer Graphics CM20219
//-------------------------------------------------------------------------------------------
//These two lines are header guardians against multiple includes
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QProgressBar>
#include "foundation.h"
#include <QtGui>
#include <QtOpenGL>
#include <glu.h>
#include "version.h"
#include <QList>



const int maxw=1024;
const int maxh=1024;
//This is our OpenGL Component we built it on top of QGLWidget
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    //Constructor for GLWidget
    GLWidget(QWidget *parent = 0);

    //Destructor for GLWidget
    ~GLWidget();


    void clear();
    void about();
    void help();
    void rotx(int);
    void roty(int);
    void rotz(int);
    void setxFrom(int);
    void setyFrom(int);
    void setzFrom(int);
    void setFilled(bool a);
    void setState(int s);
    void deletePoint();
    void cleanAllPoint();
    void frenetFrameMove(int);
    bool drawFrenet;
    void setDrawCylinder(bool);
    void setDrawCube(bool);
    void setDrawFrenetFrame(bool);
    void setCylinderRadius(int);
    void setDrawHeart(bool);

protected:
    //Initialize the OpenGL Graphics Engine
    void initializeGL();

    //All our painting stuff are here
    void paintGL();

    //When user resizes main window, the scrollArea will be resized and it will call this function from
    //its attached GLWidget
    void resizeGL(int width, int height);
    //Handle mouse
        void mousePressEvent(QMouseEvent *event);
        void mouseReleaseEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);

private:
    void startup();
    GLdouble xfrom,yfrom,zfrom, xto, yto, zto,radius,angleX,angleY;
    int winw, winh, button,imx,imy;
    int version;
    void dopan(int x, int y, bool click);
    void zoomWheel(int z);
    QImage *myimage;
    QImage timage;
    void drawFace(float w, float h);
    void displayImage();
    void drawCircle(double radius, double xcen, double ycen, bool line, int nseg);
    // for display list
    int object;
    // for the cube
    double xangle, yangle, zangle, scale;
    QImage tex[6]; // one for each face
    bool filled;
    int mClickLocationX;
    int mClickLocationY;
    int mClickLocationZ;
    bool left;
    bool right;
    float mouseSpeed;
    int pointNumber;
    QList<QList<double> > pointAll;
    QList<QList<double> > controlPoint;
    QList<double> point;
    int state;
    int upX;
    int upY;
    int upZ;
    bool movePoint;
    int theMovePoint;
    int theNumberOfcp; //the number the control point
    double tOfFrenet;
    double Ax,Ay,Az,Bx,By,Bz,Cx,Cy,Cz,Dx,Dy,Dz,Vx,Vy,Vz,Qx,Qy,Qz,tPosition;
    double px,py,pz,fVx,fVy,fVz,fBx,fBy,fBz,fNx,fNy,fNz;
    double timeID;
    bool drawFCube;
    bool drawCylinder;
    bool blockFrenetFrame;
    bool drawHeart;
    double cylinderRadius;

    // Functions
    void redraw();
   // void makeSpots(int tim, QImage *buf);
    //void drawCircle(int radius, int xcen, int ycen,  QImage *buf);
    void drawFace( int tim, float w);
    GLuint makeDice();
    void initLight();
    void makeGround();
    void makeAxes();
    void setViewPoint();
    void getViewPoint();
    void setCameraPosition();
    void setPoint();
    void drawCatmullRoom();
    void drawFrenetFrame();
    void setFactor(int i);
    void drawTheCylinder();
    void showError();
    void drawTheHeart();
};


#endif

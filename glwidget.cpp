//-------------------------------------------------------------------------------------------
//   Qtgl for graphics class
//-------------------------------------------------------------------------------------------

#include "glwidget.h"
#include <QtCore/qmath.h>
#include <QVector>
#include <QtMath>
#include <QList>
#define USE MATH DEFINES
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288419717
#endif


const double torad = M_PI/180.0;
const double lim=0.5;
GLfloat vertices[4][2] = {{100.0, 100.0}, {400.0, 100.0}, {400.0, 400.0}, {100.0, 400.0}}; //  vertex coords
GLubyte indices[] = {0,1,1,2,2,3,3,0};

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent),
      mClickLocationX(0),
      mClickLocationY(0),
      mClickLocationZ(0),
      left(false),
      right(false),
      mouseSpeed(1/100.0),
      pointNumber(0),
      state(0),
      theNumberOfcp(1),
      tPosition(0),
      drawFrenet(false),
      drawFCube(false),
      drawCylinder(false),
      blockFrenetFrame(true)
{
    startup();
}

GLWidget::~GLWidget()
{    

}

void GLWidget::startup()
{
    winw=width();  // width returns width of window
    winh=height();
    button = 0;
    cerr << "Glwidget\n";
    version=MYVERSION;
    xangle= yangle= zangle=0.0;
    scale = 1.5;
    object =0; // in this version no display list
    xfrom=yfrom=zfrom=10.0;
    radius = sqrt(300);
    xto=yto=zto=0.0;
    angleX = atan(1);
    angleY = atan(10/sqrt(200));
    filled=false;
    upX = 0;
    upY = 1;
    upZ = 0;
    setViewPoint();
    movePoint = false;
    theMovePoint = -1;
    timeID = 0;
    cylinderRadius = 0.1;
    drawHeart = false;
}

void GLWidget::clear()
{
     updateGL();
}

void GLWidget::initializeGL()
{

    int i;
    QImage buf(256, 256, QImage::Format_RGB32);

GLfloat whiteDir[4] = {2.0, 2.0, 2.0, 1.0};
GLfloat whiteAmb[4] = {1.0, 1.0, 1.0, 1.0};
GLfloat lightPos[4] = {30.0, 30.0, 30.0, 1.0};

//glEnable(GL_LIGHTING);
glEnable(GL_LIGHT0);
glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteAmb);

glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDir);
glMaterialfv(GL_FRONT, GL_SPECULAR, whiteDir);
glMaterialf(GL_FRONT, GL_SHININESS, 20.0);

glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDir);		// enable diffuse
glLightfv(GL_LIGHT0, GL_SPECULAR, whiteDir);	// enable specular
glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

glShadeModel( GL_SMOOTH );

/* Set up the textures
 comented out for this simple version
for (i=0; i<6; i++) {
makeSpots(i, &buf);
tex[i] = QGLWidget::convertToGLFormat( buf );  // flipped 32bit RGBA
}

glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
glEnable( GL_TEXTURE_2D );
*/

// Set up various other stuff
glClearColor( 0.5, 1.0, 0.75, 0.0 ); // Let OpenGL clear to black
//glEnable( GL_CULL_FACE );  	// don't need Z testing for convex objects
//if(state == 10){
glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
//}else{

//     glOrtho(0, 1.6, 0, 1.6, 0.001f, 100);
//     glLoadIdentity();
//}
 //glHint( GL_2D, GL_NICEST );
// No display list in this simple version
//object = makeDice( );	// Generate an OpenGL display list
}

void GLWidget::redraw()
{
        updateGL();
}

void GLWidget::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT );
    glLoadIdentity();
    gluLookAt(xfrom,yfrom,zfrom, xto, yto, zto, upX, upY, upZ);
    makeGround();                   //draw the ground
    makeAxes();                     //draw axes
    makeDice();
    setPoint();                     //draw the point we have added on the screen
    drawFrenetFrame();              //draw the frenet frame
    drawCatmullRoom();              //draw the catmull room
    drawTheCylinder();              //draw the cylinder
    drawTheHeart();                 //draw heart shape
}

/* 2D */
void GLWidget::resizeGL( int w, int h )
{
    glViewport( 0, 0, (GLint)w, (GLint)h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    if(state == 0){
        glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 1500.0 );
    }else{
        glOrtho(-4.0, 4.0, -4.0, 4.0, 5.0, 1000.0);
    }
    glMatrixMode( GL_MODELVIEW );
}

void GLWidget::setViewPoint()
{
    point.append(0);
    point.append(0);
    point.append(0);
    pointAll.append(point);

    point.clear();
    point.append(0);
    point.append(0.8);
    point.append(0);
    pointAll.append(point);

    point.clear();
    point.append(-0.8);
    point.append(0);
    point.append(0);
    pointAll.append(point);

    point.clear();
    point.append(-0.8);
    point.append(0.8);
    point.append(0);
    pointAll.append(point);

    point.clear();
    point.append(0.8);
    point.append(0.8);
    point.append(-0.8);
    pointAll.append(point);

    point.clear();
    point.append(0.8);
    point.append(0.8);
    point.append(0);    
    pointAll.append(point);

}


void GLWidget::about()
{
    QString vnum;
    QString mess, notes;
    QString title="QtOpenGl-2 ";

    vnum.setNum (version );
    mess="Simple OpenGl and Qt by Brian Wyvill Release Version: ";
    mess = mess+vnum;
    notes = "\n\n News: No News.";
    mess = mess+notes;
    QMessageBox::information( this, title, mess, QMessageBox::Ok );
}

void GLWidget::help()
{
    QString vnum;
    QString mess, notes;
    QString title="Cube ";

    vnum.setNum ( version );
    mess="Simple Interface to openGl and Qt by Brian Wyvill Release Version: ";
    mess = mess+vnum;
    notes = "\nThis version driven from the GLwidget. \n \
            Draws a cube, allows crude camera control and Euler angle rotations \n   ";
    mess = mess+notes;
    QMessageBox::information( this, title, mess, QMessageBox::Ok );
}

//set the state
void GLWidget::setState(int s)
{
    xto = yto = zto = 0;
    state = s;

    if(state == 0)                      //perspective view
    {
        angleX = atan(1);
        angleY = atan(10/sqrt(200));
        radius = sqrt(300);
        upX = 0; upY = 1; upZ = 0;
        yfrom = 10;
        xfrom = 10;
        zfrom = 10;
    }else if(state == 1){                //top view
        upX = 0; upY = 0; upZ = -1;
        xfrom = 0;
        yfrom = 10;
        zfrom = 0;
    }else if(state == 2)                //right view
    {
        upX = 0; upY = 1; upZ = 0;
        xfrom = 10;
        yfrom = 0;
        zfrom = 0;
    }else if(state == 3)                //front view
    {
        upX = 0; upY = 1; upZ = 0;
        xfrom = 0;
        yfrom = 0;
        zfrom = 10;
    }
    resizeGL(794,603);
    updateGL();
}



void GLWidget::initLight()
{
   GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
   GLfloat mat_shininess[] = { 128.0 };
   //   GLfloat light_position[] = { 0.3, 0.3, 0.8, 0.0 };
   GLfloat light_position[] = { 0.2, 0.2, 0.9, 0.0 };
   GLfloat light_ambient[] = { 0.05, 0.05, 0.05, 1.0 };
//   GLfloat light_specular[] = { 0.1, 0.1, 0.1, 1.0 };
   GLfloat light_specular[] = { 0.99, 0.99, 0.99, 1.0 };
   GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };

//   glClearColor(1.0, 1.0, 0.9, 1.0);
     glShadeModel (GL_SMOOTH);
   //   glShadeModel (GL_FLAT);

// MATERIAL
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
   glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

// LIGHT0
   glLightfv(GL_LIGHT0, GL_POSITION, light_position);
   glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
   glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

   glColorMaterial(GL_FRONT, GL_DIFFUSE);
   glEnable(GL_COLOR_MATERIAL);

   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   glEnable(GL_DEPTH_TEST);
   //   glDisable(GL_LIGHTING);

}

GLuint GLWidget::makeDice()
{
    GLuint list;
    float w = 0.8;

  //  list = glGenLists( 1 );
 //   glNewList( list, GL_COMPILE );   no display list this version

    // one
    glRotatef( xangle, 1.0, 0.0, 0.0 );
    glRotatef( yangle, 0.0, 1.0, 0.0 );
    glRotatef( zangle, 0.0, 0.0, 1.0 );

    glLineWidth(4);

    drawFace(0,  w);
// six
    glPushMatrix();
    glRotatef( 180.0, 1.0, 0.0, 0.0 );
    drawFace(5, w);
    glPopMatrix();

    // four on left
    glPushMatrix();
    glRotatef( -90.0, 0.0, 1.0, 0.0 );
    drawFace(3, w);
    glPopMatrix();

    // three on right
    glPushMatrix();
    glRotatef( 90.0, 0.0, 1.0, 0.0 );
    drawFace(2, w);
    glPopMatrix();

    // two
    glPushMatrix();
    glRotatef( 90.0, 1.0, 0.0, 0.0 );
    drawFace(1, w);
    glPopMatrix();

    // five
    glPushMatrix();
    glRotatef( -90.0, 1.0, 0.0, 0.0 );
    drawFace(4, w);
    glPopMatrix();
    glEndList();

  //  return list;
    return 0;
}

void GLWidget::drawFace( int tim, float w)
{
    // this version no texturing
 //   glTexImage2D( GL_TEXTURE_2D, 0, 3, tex[tim].width(), tex[tim].height(), 0,
           //       GL_RGBA, GL_UNSIGNED_BYTE, tex[tim].bits() );

    if (filled) glBegin( GL_POLYGON ); else glBegin( GL_LINE_LOOP );
   // glTexCoord2f(0.0, 0.0);
    glVertex3f(  -w,  -w, w );
   // glTexCoord2f(0.0, 1.0);
    glVertex3f(   w,  -w, w );
    //glTexCoord2f(1.0, 1.0);
    glVertex3f(   w,   w, w );
    //glTexCoord2f(1.0, 0.0);
    glVertex3f(  -w,   w, w );

    glEnd();

}

void GLWidget::makeGround(void)
{
    //glClear(GL_COLOR_BUFFER_BIT);
    glLineWidth(0.3);
    glColor3f(0.0, 0.0, 0.1);
    glBegin(GL_LINES);
        for (float i = 10; i >= -10; i = i - 0.5) {
            glVertex3f(10, -0.8, i);
            glVertex3f(-10, -0.8, i);
            glVertex3f(i, -0.8, 10);
            glVertex3f(i, -0.8, -10);
        }

    glEnd();
}

void GLWidget::makeAxes()
{
    glLineWidth(4);
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    //x
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(3.0,0.0,0.0);

    //y
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,3.0,0.0);

    //z
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,0.0,3.0);
    glEnd();

}

// communication with the window widget
void GLWidget::rotx(int a)
{
        xangle =  (double)a;
//	std::cerr << " x angle "<<xangle<<"\n";
        updateGL();
}
void GLWidget::roty(int a)
{
        yangle =  (double)a;
        updateGL();
}

void GLWidget::rotz(int a)
{
        zangle =  (double)a;
        updateGL();
}


void GLWidget::setxFrom(int a)
{
    xfrom=a;
    updateGL();
}

void GLWidget::setyFrom(int a)
{
    yfrom=a;
    updateGL();
}
void GLWidget::setzFrom(int a)
{
    zfrom=a;
    updateGL();
}

void GLWidget::getViewPoint()
{
    if(pointNumber >= 5){
        pointNumber = 0;
    }else{
        pointNumber++;
    }

    xto = pointAll[pointNumber][0];
    yto = pointAll[pointNumber][1];
    zto = pointAll[pointNumber][2];
}

void GLWidget::setPoint(){

    glColor3f(0,0,1);
    glPointSize(10);
    glBegin(GL_POINTS);
    for(int i = 0; i < controlPoint.size(); i++)
    {
        if(i == theMovePoint){
            glColor3f(1,0,0);
            glVertex3f(controlPoint[i][0],controlPoint[i][1],controlPoint[i][2]);
        }else{
            glColor3f(0,0,1);
            glVertex3f(controlPoint[i][0],controlPoint[i][1],controlPoint[i][2]);
        }
    }
    glEnd();

}

//delete all the point that we have added
void GLWidget::deletePoint()
{
    if(theMovePoint != -1)
    {
        controlPoint.removeAt(theMovePoint);
        theMovePoint = -1;
        updateGL();
    }
}


void GLWidget::drawCatmullRoom()
{

    if(controlPoint.size() > 3)
    {
        glPointSize(10);
        glColor3f(0,0,0);
        glBegin(GL_LINE_STRIP);
        for(int i = 1; i < controlPoint.size()-2; i++)
                {
                    for(int j = 0;  j < 100; j++){
                        tPosition = j*0.01; //time
                        setFactor(i);
                        glVertex3f(px,py,pz);
                     }                   
                 }
        glEnd();
    }
}

void GLWidget::drawTheCylinder()
{
    if(drawCylinder){
        glColor3f(0,0,1);

        for(int i = 1; i < controlPoint.size()-2; i++)
                {
                    for(int j = 0;  j <= 200; j++){
                        tPosition = j*0.005; //time
                        setFactor(i);
                        QVector3D p(px+(fBx*cylinderRadius),py+(fBy*cylinderRadius),pz+(fBz*cylinderRadius));
                        QVector3D a(px,py,pz);
                        QMatrix4x4 m;
                        glBegin(GL_LINE_LOOP);
                        for(double k = 0;k < 360;k = k + 0.5){
                            p-=a;
                            m.rotate(k, Vx, Vy, Vz);
                            p = p*m;
                            p+=a;
                            glVertex3f(p.x(),p.y(),p.z());
                        }
                        glEnd();
                     }

                 }            
    }
}

void GLWidget::drawTheHeart(){
    if(drawHeart){
        {
                glColor3f(0,0,1);

                for(int i = 1; i < controlPoint.size()-2; i++)
                        {
                            for(int j = 0;  j <= 200; j++){
                                tPosition = j*0.005; //time
                                setFactor(i);
                                glBegin(GL_LINE_LOOP);
                                glVertex3f(px+0.5*fBx,py+0.5*fBy,pz+0.5*fBz);
                                glVertex3f(px+(-0.5)*fNx,py+(-0.5)*fNy,pz+(-0.5)*fNz);
                                glVertex3f(px+(-0.5)*fBx,py+(-0.5)*fBy,pz+(-0.5)*fBz);
                                glVertex3f(px+0.5*fNx,py+0.5*fNy,pz+0.5*fNz);
                                glEnd();

                                double radi = sqrt(0.5)/2.0;
                                glBegin(GL_LINE_LOOP);
                                for(double x = radi; x>(-radi); x = x-0.01){
                                    double y = sqrt(radi *radi-x*x);
                                    glVertex3f(px-radi*fNx+radi*fBx+x*fNx+y*fBx,
                                               py-radi*fNy+radi*fBy+x*fNy+y*fBy,
                                               pz-radi*fNz+radi*fBz+x*fNz+y*fBz);
                                    y = -sqrt(radi*radi-x*x);
                                    glVertex3f(px-radi*fNx+radi*fBx+x*fNx+y*fBx,
                                               py-radi*fNy+radi*fBy+x*fNy+y*fBy,
                                               pz-radi*fNz+radi*fBz+x*fNz+y*fBz);
                                }
                                glEnd();

                                glBegin(GL_LINE_LOOP);
                                for(double x = radi; x>(-radi); x = x-0.01){
                                    double y = sqrt(radi *radi-x*x);
                                    glVertex3f(px-radi*fNx-radi*fBx+x*fNx+y*fBx,
                                               py-radi*fNy-radi*fBy+x*fNy+y*fBy,
                                               pz-radi*fNz-radi*fBz+x*fNz+y*fBz);
                                    y = -sqrt(radi*radi-x*x);
                                    glVertex3f(px-radi*fNx-radi*fBx+x*fNx+y*fBx,
                                               py-radi*fNy-radi*fBy+x*fNy+y*fBy,
                                               pz-radi*fNz-radi*fBz+x*fNz+y*fBz);
                                }
                                glEnd();

                             }

                         }

            }
    }
}

void GLWidget::frenetFrameMove(int time)
{
    timeID = time;
    updateGL();
}

void GLWidget::drawFrenetFrame()
{
    if(drawFrenet || drawFCube)
    {
        double unitT = 100/(controlPoint.size()-3);
        theNumberOfcp = (timeID/unitT) + 1;
        tPosition = (timeID - (theNumberOfcp-1)*unitT)*(1/unitT);
        if(theNumberOfcp >= controlPoint.size()-2)
        {
            theNumberOfcp = controlPoint.size()-3;
            tPosition = 0.99999;
        }

        if(drawFrenet){
            glColor3f(1,0,0);
            glBegin(GL_LINES);
                setFactor(theNumberOfcp);

                //draw unit vector of tangent
                glVertex3f(px,py,pz);
                glVertex3f(px+fVx,py+fVy,pz+fVz);

                //draw unit vector of binormal
                glVertex3f(px,py,pz);
                glVertex3f(px+fBx,py+fBy,pz+fBz);

                //draw unit vector of normal
                glVertex3f(px,py,pz);
                glVertex3f(px+fNx,py+fNy,pz+fNz);

           glEnd();

        }

        if(drawFCube){
            glColor3f(0,0,0);

            glBegin(GL_LINE_LOOP);

            setFactor(theNumberOfcp);
            glVertex3f(px,py,pz);
            glVertex3f(px+fVx,py+fVy,pz+fVz);
            glVertex3f(px+fBx+fVx,py+fBy+fVy,pz+fBz+fVz);
            glVertex3f(px+fBx,py+fBy,pz+fBz);
            glEnd();
            glBegin(GL_LINE_LOOP);
            glVertex3f(px,py,pz);
            glVertex3f(px+fBx,py+fBy,pz+fBz);
            glVertex3f(px+fNx+fBx,py+fNy+fBy,pz+fNz+fBz);
            glVertex3f(px+fNx,py+fNy,pz+fNz);
            glEnd();

            glBegin(GL_LINE_LOOP);
            glVertex3f(px+fBx+fVx,py+fBy+fVy,pz+fBz+fVz);
            glVertex3f(px+fVx,py+fVy,pz+fVz);
            glVertex3f(px+fNx+fVx,py+fNy+fVy,pz+fNz+fVz);
            glVertex3f(px+fNx+fVx+fBx,py+fNy+fVy+fBy,pz+fNz+fVz+fBz);
            glEnd();

            glBegin(GL_LINE_LOOP);
            glVertex3f(px+fNx,py+fNy,pz+fNz);
            glVertex3f(px+fNx+fVx,py+fNy+fVy,pz+fNz+fVz);
            glVertex3f(px+fNx+fVx+fBx,py+fNy+fVy+fBy,pz+fNz+fVz+fBz);
            glVertex3f(px+fNx+fBx,py+fNy+fBy,pz+fNz+fBz);
            glEnd();

        }

    }


}


//set the factor for all the drawing work
void GLWidget::setFactor(int i){

    //Px = Ax*t^3 + Bx*t^2 + Cx*t + Dx
    Ax = -0.5*controlPoint[i-1][0] + 1.5*controlPoint[i][0] - 1.5*controlPoint[i+1][0] + 0.5*controlPoint[i+2][0];
    Ay = -0.5*controlPoint[i-1][1] + 1.5*controlPoint[i][1] - 1.5*controlPoint[i+1][1] + 0.5*controlPoint[i+2][1];
    Az = -0.5*controlPoint[i-1][2] + 1.5*controlPoint[i][2] - 1.5*controlPoint[i+1][2] + 0.5*controlPoint[i+2][2];
    Bx = controlPoint[i-1][0] - 2.5*controlPoint[i][0] + 2*controlPoint[i+1][0] - 0.5*controlPoint[i+2][0];
    By = controlPoint[i-1][1] - 2.5*controlPoint[i][1] + 2*controlPoint[i+1][1] - 0.5*controlPoint[i+2][1];
    Bz = controlPoint[i-1][2] - 2.5*controlPoint[i][2] + 2*controlPoint[i+1][2] - 0.5*controlPoint[i+2][2];
    Cx = 0.5*(-controlPoint[i-1][0]+controlPoint[i+1][0]);
    Cy = 0.5*(-controlPoint[i-1][1]+controlPoint[i+1][1]);
    Cz = 0.5*(-controlPoint[i-1][2]+controlPoint[i+1][2]);
    Dx = controlPoint[i][0];
    Dy = controlPoint[i][1];
    Dz = controlPoint[i][2];

    //unit vector of tangent
    Vx = (3*Ax*tPosition*tPosition + 2*Bx*tPosition +Cx)/
            sqrt(pow((3*Ax*tPosition*tPosition + 2*Bx*tPosition +Cx),2)
                     + pow((3*Ay*tPosition*tPosition + 2*By*tPosition +Cy),2) + pow((3*Az*tPosition*tPosition + 2*Bz*tPosition +Cz),2));
    Vy = (3*Ay*tPosition*tPosition + 2*By*tPosition +Cy)/
            sqrt(pow((3*Ax*tPosition*tPosition + 2*Bx*tPosition +Cx),2)
                     + pow((3*Ay*tPosition*tPosition + 2*By*tPosition +Cy),2) + pow((3*Az*tPosition*tPosition + 2*Bz*tPosition +Cz),2));
    Vz = (3*Az*tPosition*tPosition + 2*Bz*tPosition +Cz)/
            sqrt(pow((3*Ax*tPosition*tPosition + 2*Bx*tPosition +Cx),2)
                     + pow((3*Ay*tPosition*tPosition + 2*By*tPosition +Cy),2) + pow((3*Az*tPosition*tPosition + 2*Bz*tPosition +Cz),2));

    //unit vector of acceleration
    Qx = 6*Ax*tPosition+2*Bx;
    Qy = 6*Ay*tPosition+2*By;
    Qz = 6*Az*tPosition+2*Bz;

    px = Dx + tPosition*Cx + tPosition*tPosition*Bx + tPosition*tPosition*tPosition*Ax;
    py = Dy + tPosition*Cy + tPosition*tPosition*By + tPosition*tPosition*tPosition*Ay;
    pz = Dz + tPosition*Cz + tPosition*tPosition*Bz + tPosition*tPosition*tPosition*Az;

    //unit vector of tangent
    fVx = Vx;
    fVy = Vy;
    fVz = Vz;

    //draw unit vector of binormal
    fBx = (Vy*Qz - Vz*Qy)/sqrt(pow((Vy*Qz - Vz*Qy),2) + pow((Vx*Qz - Vz*Qx),2) + pow((Vx*Qy - Vy*Qx),2));
    fBy = -(Vx*Qz - Vz*Qx)/sqrt(pow((Vy*Qz - Vz*Qy),2) + pow((Vx*Qz - Vz*Qx),2) + pow((Vx*Qy - Vy*Qx),2));
    fBz = (Vx*Qy - Vy*Qx)/sqrt(pow((Vy*Qz - Vz*Qy),2) + pow((Vx*Qz - Vz*Qx),2) + pow((Vx*Qy - Vy*Qx),2));

    //draw unit vector of normal
    fNx = (-(Vx*Qz - Vz*Qx)*Vz - (Vx*Qy - Vy*Qx)*Vy)
            /sqrt(pow((-(Vx*Qz - Vz*Qx)*Vz - (Vx*Qy - Vy*Qx)*Vy),2)
                  + pow((-((Vy*Qz - Vz*Qy)*Vz - (Vx*Qy - Vy*Qx)*Vx)),2) + pow(((Vy*Qz - Vz*Qy)*Vy + (Vx*Qz - Vz*Qx)*Vx),2));
    fNy = (-((Vy*Qz - Vz*Qy)*Vz - (Vx*Qy - Vy*Qx)*Vx))
            /sqrt(pow((-(Vx*Qz - Vz*Qx)*Vz - (Vx*Qy - Vy*Qx)*Vy),2)
                  + pow((-((Vy*Qz - Vz*Qy)*Vz - (Vx*Qy - Vy*Qx)*Vx)),2) + pow(((Vy*Qz - Vz*Qy)*Vy + (Vx*Qz - Vz*Qx)*Vx),2));;
    fNz = ((Vy*Qz - Vz*Qy)*Vy + (Vx*Qz - Vz*Qx)*Vx)
            /sqrt(pow((-(Vx*Qz - Vz*Qx)*Vz - (Vx*Qy - Vy*Qx)*Vy),2)
                  + pow((-((Vy*Qz - Vz*Qy)*Vz - (Vx*Qy - Vy*Qx)*Vx)),2) + pow(((Vy*Qz - Vz*Qy)*Vy + (Vx*Qz - Vz*Qx)*Vx),2));;
}


// mouse routines for camera control to be implemented
void GLWidget::mousePressEvent( QMouseEvent *e )
{

 /*   if (df->getPan()) dopan(e->x(), height()-e->y() , true);
    else {*/
    double testX = 0;
    double testY = 0;
    double testZ = 0;
    button =  e->button();
    if(state == 0){
        if (button == Qt::LeftButton) {

            left = true;
            mClickLocationX = e->x();
            mClickLocationY = e->y();

        }else if(button == Qt::RightButton){
            right = true;
            mClickLocationZ = e->y();
        }else if(button == Qt::MidButton)
        {
            getViewPoint();
        }
    }
    else{
        if(button == Qt::RightButton){
            //(e->x()-284)/71.25 is used to change the screen axes to the 3D space(you may need to change the value if you are not to use the "retina screen")
            if(state == 1){
                point.clear();
                point.append((e->x()-284)/71.25);
                point.append(0);
                point.append((e->y()-255)/63.75);
                controlPoint.append(point);
            }else if(state == 2){
                point.clear();
                point.append(0);
                point.append(-(e->y()-255)/63.75);
                point.append(-(e->x()-284)/71.25);
                controlPoint.append(point);
            }else{
                point.clear();
                point.append((e->x()-284)/71.25);
                point.append(-(e->y()-255)/63.75);
                point.append(0);
                controlPoint.append(point);
            }

        }else if(button == Qt::LeftButton){
            mClickLocationX = e->x();
            mClickLocationY = e->y();
            if(state == 1){
                testX = (e->x()-284)/71.25;
                testZ = (e->y()-255)/63.75;
                for(int i = 0; i < controlPoint.size(); i++) //0.14 is length of the cube's sides
                {
                    if( (controlPoint[i][0] - 0.14 <= testX) && (testX <= controlPoint[i][0] + 0.14)&&
                            (controlPoint[i][2] - 0.14 <= testZ) && (testZ <= controlPoint[i][2] + 0.14)){
                        movePoint = true;
                        theMovePoint = i;
                    }
                }
            }else if(state == 2){
                testY = -(e->y()-255)/63.75;
                testZ = -(e->x()-284)/71.25;
                for(int i = 0; i < controlPoint.size(); i++) //0.14 is length of the cube's sides
                {
                    if((controlPoint[i][1] - 0.14 <= testY)&& (testY <= controlPoint[i][1] + 0.14) &&
                            (controlPoint[i][2] - 0.14 <= testZ) && (testZ <= controlPoint[i][2] + 0.14)){
                        movePoint = true;
                        theMovePoint = i;
                    }
                }
            }else if(state == 3){
                testX = (e->x()-284)/71.25;
                testY = -(e->y()-255)/63.75;
                for(int i = 0; i < controlPoint.size(); i++) //0.14 is length of the cube's sides
                {
                    if( (controlPoint[i][0] - 0.14 <= testX) && (testX <= controlPoint[i][0] + 0.14) && (controlPoint[i][1] - 0.14 <= testY)
                            && (testY <= controlPoint[i][1] + 0.14)){
                        movePoint = true;
                        theMovePoint = i;
                    }
                }
            }

        }
    }
    updateGL();
}

void GLWidget::mouseReleaseEvent( QMouseEvent *e)
{
    if(left){ left = false;}
    if(right){right = false;}
    movePoint = false;
    //updateGL();
}

void GLWidget::mouseMoveEvent ( QMouseEvent *e )
{
    button =  e->button();
    int mouseX = e->x();
    int mouseY = e->y();
    int mouseZ = e->y();

    if(state == 0){             //perspective view
        if(left)
        {
            angleX = angleX + (mouseX - mClickLocationX)*mouseSpeed;
            angleY = angleY + (mouseY - mClickLocationY)*mouseSpeed;
            xfrom = radius * cos(angleY)*cos(angleX);
            yfrom = radius * sin(angleY);
            zfrom = xfrom*tan(angleX);
        }
        else if(right){

            radius = radius + (mouseZ - mClickLocationZ)*mouseSpeed;
            yfrom = radius * sin(angleY);
            xfrom = radius * cos(angleY)*cos(angleX);
            zfrom = xfrom * tan(angleX);            
            mClickLocationZ = mouseZ;

        }

    }else{
        if(movePoint){
            if(state == 1)
            {
                controlPoint[theMovePoint][0] =  controlPoint[theMovePoint][0]  + (mouseX - mClickLocationX)/71.25;
                controlPoint[theMovePoint][2] = controlPoint[theMovePoint][2] +  (mouseY - mClickLocationY)/63.75;
            }else if(state == 2)
            {
                controlPoint[theMovePoint][1] =  controlPoint[theMovePoint][1]  - (mouseY - mClickLocationY)/63.75;
                controlPoint[theMovePoint][2] = controlPoint[theMovePoint][2] -  (mouseX - mClickLocationX)/71.25;
            }else if( state == 3){
                controlPoint[theMovePoint][0] =  controlPoint[theMovePoint][0]  + (mouseX - mClickLocationX)/71.25;
                controlPoint[theMovePoint][1] = controlPoint[theMovePoint][1] -  (mouseY - mClickLocationY)/63.75;
            }
        }
    }

    mClickLocationX = mouseX;
    mClickLocationY = mouseY;
    updateGL();
}


void GLWidget::setFilled(bool a)
{
    filled=a;
    updateGL();
}


void GLWidget::cleanAllPoint()
{
    controlPoint.clear();
    drawFrenet = false;
    updateGL();
}

void GLWidget::setDrawCylinder(bool b)
{
    if(controlPoint.size() > 3)
    {
        drawCylinder = b;
    }else{
        showError();
    }
    updateGL();
}

void GLWidget::setDrawCube(bool c)
{
    if(controlPoint.size() > 3)
    {
           drawFCube = c;
    }else{
        showError();
    }
    updateGL();
}

void GLWidget::setDrawFrenetFrame(bool d)
{
    if(controlPoint.size() > 3)
    {
        drawFrenet = d;
    }else{
        showError();
    }
    updateGL();
}

void GLWidget::setCylinderRadius(int r)
{
    cylinderRadius = r/10.0;
    updateGL();
}



void GLWidget::showError()
{
    QString vnum;
    QString mess;
    QString title="ERROR";

    vnum.setNum ( version );
    mess="Please add 4 points to generate catmull-rom firstly.";
    mess = mess+vnum;
    QMessageBox::information( this, title, mess, QMessageBox::Ok );
}

void GLWidget::setDrawHeart(bool b)
{
    if(controlPoint.size() > 3)
    {
        drawHeart = b;
    }else{
        showError();
    }
    updateGL();

}





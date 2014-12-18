//-------------------------------------------------------------------------------------------
//  This is a demo. program for open gl vertex arrays
//-------------------------------------------------------------------------------------------
#include <QtGui>
#include <QtOpenGL>
#include "window.h"
//#include "dfuncs.h"

#define BIT0 0x00000001   // alpha
#define BIT1 0x00000002   // no buttons
#define BIT2 0x00000004   // set don't use native dialog

//extern dfuncs *df;

//------------------------------------------------------------------------------------
// Creates and initializes the main window for application
//------------------------------------------------------------------------------------
Window::Window(QWidget *parent):QDialog(parent)
{
    //We create an instance of GLWidget component we built in glwidget.h
    m_glWidget = new GLWidget;

    //Setup application interface. Creates all the required components and sliders.
    setupUi(this);

    //We need to attach our m_glWidget to glWidgetArea
    //All our drawings will be on glWidgetArea
    glWidgetArea->setWidget(m_glWidget);

}

void Window::resizeEvent( QResizeEvent * )
{
  cerr << "new size "<< width() SEP height() NL;
}

void Window::clear()
{
    m_glWidget->clear();
}

void Window::aboutBut()
{
    m_glWidget->about();
}

void Window::helpBut()
{
    m_glWidget->help();
}

void Window::pushmebut()
{
    cerr << "pushme pushed\n";
}

void Window::xrot(int x)
{
    m_glWidget->rotx(x);
}

void Window::yrot(int y)
{
        m_glWidget->roty(y);
}

void Window::zrot(int z)
{
        m_glWidget->rotz(z);
}

void Window::xFrom(int a)
{
    m_glWidget->setxFrom(a);
}

void Window::yFrom(int a)
{
    m_glWidget->setyFrom(a);
}

void Window::zFrom(int a)
{
    m_glWidget->setzFrom(a);
}

void Window::filledOn(bool a)
{
    m_glWidget->setFilled(a);
}

void Window::pView()
{
    m_glWidget->setState(0);
}

void Window::tView()
{
     m_glWidget->setState(1);
}

void Window::rView()
{
     m_glWidget->setState(2);
}

void Window::fView()
{
     m_glWidget->setState(3);
}

void Window::dPoint()
{
    m_glWidget->deletePoint();
}


void Window::cleanPoint()
{
    m_glWidget->cleanAllPoint();
}

void Window::moveFrenetFrame(int t)
{
    m_glWidget->frenetFrameMove(t);
}

void Window::drawCylinder(bool b){
    m_glWidget->setDrawCylinder(b);
}

void Window::drawCube(bool c)

{
    m_glWidget->setDrawCube(c);
}

void Window::drawFrenetF(bool d)
{
    m_glWidget->setDrawFrenetFrame(d);
}

void Window::sendRadiusOfCyln(int t)
{
    m_glWidget->setCylinderRadius(t);
}

void Window::setHeart(bool b){
    m_glWidget->setDrawHeart(b);
}


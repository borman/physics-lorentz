#include <QTimer>
#include <QtOpenGL>
#include "SimulationViewer.h"
#include "Geom.h"

using namespace Geom;

SimulationViewer::SimulationViewer(Simulation *sim, QWidget *parent) :
    QGLWidget(parent), m_sim(sim)
{
  QTimer *t = new QTimer(this);
  t->setInterval(1000/60);
  t->start();
  connect(t, SIGNAL(timeout()), SLOT(onTimer()));
}

void SimulationViewer::initializeGL()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
}

void SimulationViewer::resizeGL(int w, int h)
{
  glViewport(0, 0, (GLint) w, (GLint) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, m_sim->width(), 0, m_sim->height(), 0, 15.0);
  glMatrixMode(GL_MODELVIEW);
}

void SimulationViewer::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT);

  glColor3d(0.9, 0.7, 0.5);
  for (int i=0; i<m_sim->ionCount(); i++)
  {
    Point pos = m_sim->ionPosition(i);
    double r = m_sim->ionRadius(i);
    //glRectd(pos.x()-r, pos.y()-r, pos.x()+r, pos.y()+r);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(pos.x(), pos.y());
    static const int circle_sides = 10;
    for (int j=0; j<=circle_sides; j++)
    {
      double alpha = j * (2*M_PI/circle_sides);
      glVertex2d(pos.x() + r*cos(alpha), pos.y() + r*sin(alpha));
    }
    glEnd();
  }

  glColor4d(1.0, 1.0, 1.0, 0.5);
  for (int i=0; i<m_sim->electronCount(); i++)
  {
    Point pos = m_sim->electronPosition(i);
    double r = 0.5;
    glRectd(pos.x()-r, pos.y()-r, pos.x()+r, pos.y()+r);
  }
}

void SimulationViewer::onTimer()
{
  m_sim->advanceTime(1.0/60);
  repaint();
}

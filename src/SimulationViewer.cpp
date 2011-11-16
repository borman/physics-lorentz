#include <QTimer>
#include <QtOpenGL>
#include "SimulationViewer.h"
#include "Geom.h"

using namespace Geom;

const double FPS = 60;

SimulationViewer::SimulationViewer(Simulation *sim, QWidget *parent)
  : QGLWidget(QGLFormat(QGL::AccumBuffer | QGL::SampleBuffers), parent),
    m_sim(sim)
{
  QTimer *t = new QTimer(this);
  t->setInterval(1000/FPS);
  //t->setInterval(500);
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

  glPointSize(2.0);
  glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
  glPointSize(2.0);
}

void SimulationViewer::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT);
  //glAccum(GL_RETURN, 1.0);

  glColor4d(1.0, 1.0, 1.0, 0.5);
  glBegin(GL_POINTS);
  for (int i=0; i<m_sim->electronCount(); i++)
  {
    Point pos = m_sim->electronPosition(i);
    glVertex2d(pos.x, pos.y);
  }
  glEnd();

  //glAccum(GL_LOAD, 1.0);
  //glAccum(GL_MULT, 0.9);

  glColor3d(0.9, 0.7, 0.5);
  for (int i=0; i<m_sim->ionCount(); i++)
  {
    Point pos = m_sim->ionPosition(i);
    double r = m_sim->ionRadius(i);

    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(pos.x, pos.y);
    static const int circle_sides = 10;
    for (int j=0; j<=circle_sides; j++)
    {
      double alpha = j * (2*M_PI/circle_sides);
      glVertex2d(pos.x + r*cos(alpha), pos.y + r*sin(alpha));
    }
    glEnd();
  }
}

void SimulationViewer::onTimer()
{
  m_sim->advanceTime(1.0/FPS);
  repaint();
}

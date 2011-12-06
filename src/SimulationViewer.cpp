#include <QTimer>
#include <QtOpenGL>
#include "SimulationViewer.h"
#include "Geom.h"

using namespace Geom;

const double FPS = 60;

SimulationViewer::SimulationViewer(QWidget *parent)
  : QGLWidget(QGLFormat(QGL::AccumBuffer | QGL::SampleBuffers), parent),
    m_sim(0), m_needResize(false)
{
  m_timer = new QTimer(this);
  m_timer->setInterval(1000/FPS);
  connect(m_timer, SIGNAL(timeout()), SLOT(step()));
}

void SimulationViewer::initializeGL()
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glPointSize(2.0f);
  glLineWidth(2.0f);
}

void SimulationViewer::resizeGL(int w, int h)
{
  double simAspect = m_sim->width() / m_sim->height();
  double aspect = double(w)/double(h);
  int cw=w, ch=h;
  if (aspect < simAspect)
    ch = w/simAspect;
  else
    cw = h*simAspect;

  glViewport(0, 0, cw, ch);
  if (m_sim)
    setupViewport();

  glClear(GL_COLOR_BUFFER_BIT | GL_ACCUM_BUFFER_BIT);
}

void SimulationViewer::setupViewport()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0, m_sim->width(), 0, m_sim->height(), -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
}

void SimulationViewer::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT);
  if (!m_sim)
    return;

  //glAccum(GL_RETURN, 1.0);

  glColor3d(1.0, 1.0, 1.0);
  glBegin(GL_LINE_STRIP);
    glVertex2d(0, 0);
    glVertex2d(m_sim->width(), 0);
    glVertex2d(m_sim->width(), m_sim->height());
    glVertex2d(0, m_sim->height());
    glVertex2d(0, 0);
  glEnd();

  // Electrons
  glBegin(GL_POINTS);
  for (size_t i=0; i<m_sim->electronCount(); i++)
  {
    Point pos = m_sim->electron(i).pos;
    glVertex2d(pos.x, pos.y);
  }
  glEnd();

  //glAccum(GL_LOAD, 1.0);
  //glAccum(GL_MULT, 0.9);

  glColor3d(0.9, 0.7, 0.5);
  for (size_t i=0; i<m_sim->ionCount(); i++)
  {
    Point pos = m_sim->ion(i).pos;
    double r = m_sim->ion(i).radius(m_sim->params(), m_sim->time());

    // TODO: use display lists
#if 1
    glBegin(GL_TRIANGLE_FAN);
    glVertex2d(pos.x, pos.y);
#else
    glBegin(GL_LINE_STRIP);
#endif
    static const int circle_sides = 20;
    for (int j=0; j<=circle_sides; j++)
    {
      double alpha = j * (2*M_PI/circle_sides);
      glVertex2d(pos.x + r*cos(alpha), pos.y + r*sin(alpha));
    }
    glEnd();
  }
}

void SimulationViewer::step()
{
  m_sim->advanceTime(1.0/FPS);
  update();
}

void SimulationViewer::run(bool enabled)
{
  if (enabled)
    m_timer->start();
  else
    m_timer->stop();
}

void SimulationViewer::setSimulation(Simulation *sim)
{
  m_sim = sim;
  if (isValid())
    setupViewport();
  connect(m_sim, SIGNAL(paramsChanged()), SLOT(simulationChanged()));
}

QSize SimulationViewer::sizeHint() const
{
  if (m_sim)
    return QSize(m_sim->width(), m_sim->height());
  else
    return minimumSizeHint();
}

int SimulationViewer::heightForWidth(int w) const
{
  if (m_sim)
    return (m_sim->height() * w) / m_sim->width();
  else
    return -1;
}

void SimulationViewer::simulationChanged()
{
  if (isValid())
    setupViewport();
  update();
}

#include <cstdlib>
#include <utility>
#include <QList>
#include "Simulation.h"

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif

using namespace Geom;
using namespace std;

static Scalar rand_u()
{
  return Scalar(rand()) / RAND_MAX;
}

static Scalar calcRadius(const Simulation::Params &p, Scalar time, Scalar phase)
{
#if 0
  return p.ionBaseRadius + sin(time * p.ionOscillSpeed + phase) * p.ionDeltaRadius;
#else
  return p.ionBaseRadius;
#endif
}

Simulation::Simulation(const Params &p, QObject *parent)
  : QObject(parent),
    m_params(p), m_time(0), 
    m_width(p.gridStep * p.gridWidth),
    m_height(p.gridStep * p.gridHeight)
{
  m_ions.reserve(p.gridWidth * p.gridHeight);
  for (int y=0; y<p.gridHeight; y++)
    for (int x=0; x<p.gridWidth; x++)
    {
      Ion ion;
      ion.pos = Point(p.gridStep * (x+0.5), p.gridStep * (y+0.5));
      if (p.ionPhaseDistribution == Uniform)
        ion.phase = rand_u() * 2 * M_PI;
      else
        ion.phase = 0;
      ion.radius = calcRadius(m_params, m_time, ion.phase);
      m_ions.push_back(ion);
    }

  m_electrons.reserve(p.electronCount);
  for (int i=0; i<p.electronCount; i++)
  {
    Electron e;
    e.pos = Point(rand_u() * m_width, rand_u() * m_height);
    Scalar phi = rand_u() * 2 * M_PI;
    Scalar vel = 100;
    e.vel = Point(vel * cos(phi), vel * sin(phi));
    m_electrons.push_back(e);
  }

  m_edges[0] = Line(Point(0,        0),        Point(m_width, 0));
  m_edges[1] = Line(Point(m_width,  0),        Point(m_width, m_height));
  m_edges[2] = Line(Point(m_width,  m_height), Point(0,       m_height));
  m_edges[3] = Line(Point(0,        m_height), Point(0,       0));
  for (int i=0; i<4; i++)
    m_edges[i].normalize();
}

static void deflectFromEdges(Simulation::Electron &e, const Line &l)
{

}

void Simulation::advanceElectron(Simulation::Electron &e, double dt)
{
  Point newPos = e.pos + e.vel * dt;

  QList<pair<Point, Point> > hits; // (position, normal)

  // Bounce off walls
  for (int j=0; j<4; j++)
    if (m_edges[j].crossedBy(e.pos, newPos))
    {
      Scalar d1 = m_edges[j] * e.pos;
      Scalar d2 = m_edges[j] * newPos;
      Point dv = (d2-d1) * m_edges[j].normal();
      e.pos += dv;
      e.vel += (-2/dt) * dv;
    }

  // Bounce off ions
  for (int j=0; j<m_ions.size(); j++) // TODO: optimize
  {
  }

  e.pos += e.vel * dt;
}

void Simulation::advanceTime(Scalar dt)
{
  Scalar newTime = m_time + dt;

  for (int i=0; i<m_ions.size(); i++)
    m_ions[i].radius = calcRadius(m_params, newTime, m_ions[i].phase);

#pragma omp parallel for
  for (int i=0; i<m_electrons.size(); i++)
    advanceElectron(m_electrons[i], dt);

  m_time = newTime;
}

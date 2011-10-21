#include <cstdlib>
#include "physmodel.h"

using namespace Geom;

static Scalar calcRadius(const PhysModel::Params &p, Scalar time, Scalar phase)
{
  return p.ionBaseRadius + sin(time * p.ionOscillSpeed + phase) * p.ionDeltaRadius;
}

PhysModel::PhysModel(const Params &p, QObject *parent)
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
        ion.phase = double(rand()) * 2 * M_PI / RAND_MAX;
      else
        ion.phase = 0;
      ion.radius = calcRadius(m_params, m_time, ion.phase);
      m_ions.push_back(ion);
    }
}

void PhysModel::advanceTime(Scalar dt)
{
  Scalar newTime = m_time + dt;
  for (int i=0; i<m_ions.size(); i++)
    m_ions[i].radius = calcRadius(m_params, newTime, m_ions[i].phase);
  m_time = newTime;
}

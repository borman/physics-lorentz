#include <cstdlib>
#include <utility>
#include <QList>
#include <QDebug>
#include "Simulation.h"

#ifndef M_PI
#define M_PI 3.1415926535897932385
#endif

using namespace Geom;
using namespace std;

class HitSorter
{
  public:
    HitSorter (const Point &ref, Scalar tf)
      : m_empty(true), m_timeframe(tf), m_ref(ref), m_time(0) {}

    bool empty() const { return m_empty; }
    Scalar timeframe() const { return m_timeframe; }
    Scalar time() const { return m_time; }
    Point pos() const { return m_pos; }
    Point norm() const { return m_norm; }
    Point vel() { return m_vel; }

    void add(Scalar time, const Point &pos, const Point &norm, const Point &vel = Point(0, 0))
    {
      if (time > 0 && time < m_timeframe+eps && (m_empty || time < m_time))
      {
        m_empty = false;
        m_time = time;
        m_pos = pos;
        m_norm = norm;
        m_vel = vel;
      }
    }
  private:
    bool m_empty;
    Scalar m_timeframe;
    Point m_ref;
    Scalar m_time;
    Point m_pos, m_norm, m_vel;
};

static inline Scalar rand_u()
{
  return Scalar(rand()) / RAND_MAX;
}

static inline Scalar sqr(Scalar x)
{
  return x*x;
}

static QDebug operator<<(QDebug dbg, const Simulation::Electron &e)
{
    dbg.nospace() << "E(p=" << e.pos << "v=" << e.vel << ")";
    return dbg.space();
}

static QDebug operator<<(QDebug dbg, const Simulation::Ion &ion)
{
    dbg.nospace() << "Ion(p=" << ion.pos << ")";
    return dbg.space();
}


Scalar Simulation::Ion::radius(const Params &p, Scalar time) const
{
#if 1
  return p.ionBaseRadius + sin(time * p.ionOscillSpeed + phase) * p.ionDeltaRadius;
#else
  return p.ionBaseRadius;
#endif
}

Scalar Simulation::Ion::radiusSpeed(const Params &p, Scalar time) const
{
  return cos(time * p.ionOscillSpeed + phase) * p.ionDeltaRadius;
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
      m_ions.push_back(ion);
    }

  m_electrons.reserve(p.electronCount);
  for (size_t i=0; i<p.electronCount; i++)
  {
    Electron e;

    // Avoid putting electrons inside ions
    bool properly = true;
    do
    {
      e.pos = Point(rand_u() * m_width, rand_u() * m_height);
      properly = true;
      for (size_t j=0; j<m_ions.size(); j++) 
        properly = properly && 
          (e.pos - m_ions[j].pos).sqr() >
                   sqr(m_ions[j].radius(m_params, 0)) + eps;
    }
    while (!properly);

    Scalar phi = rand_u() * 2 * M_PI;
    Scalar vel = p.electronBaseSpeed;
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

void Simulation::collideWithIon(Scalar time,
                                HitSorter &hits, 
                                const Simulation::Electron &e, 
                                const Simulation::Ion &ion)
{
  Point d = e.pos - ion.pos;

  // Assume linear radius function
  Scalar dt = hits.timeframe();
  Scalar r0 = ion.radius(m_params, time);
  Scalar r1 = ion.radius(m_params, time+dt);
  //qDebug() << "Test ion:" << ion << "r:" << r0 << "->" << r1;

  // kt^2 + lt + m = 0
  Scalar k = e.vel.sqr() - sqr((r1-r0)/dt);
  Scalar l = 2 * e.vel.dot(d) - 2 * r0*(r1-r0)/dt;
  Scalar m = d.sqr() - r0*r0;

  Scalar dis = l*l - 4*k*m;
  if (dis >= 0) // Solutions exist
  {
    Scalar ts[2];
    ts[0] = (-l - sqrt(dis)) / (2*k);
    ts[1] = (-l + sqrt(dis)) / (2*k);
    for (int q=0; q<2; q++)
    {
      Scalar t = ts[q];
      Point hit = e.pos + t*e.vel;

      // Fix hit position to be on the edge
      Scalar k = (ion.radius(m_params, time+t) + 0.1) / ((hit-ion.pos).length());
      hit = ion.pos + (hit-ion.pos)*k;

      Point norm = (hit - ion.pos).normalized();
      //qDebug() << "Collision:" << e << ion << "@" << t;
      hits.add(t, hit, norm, norm*ion.radiusSpeed(m_params, time+t));
    }
  }
}

void Simulation::collideWithWall(HitSorter &hits, 
                                 const Simulation::Electron &e, 
                                 const Line &wall)
{
  Point norm = wall.normal();
  Scalar det = norm.dot(e.vel);

  if (abs(det) > eps) // not parallel
  {
    Scalar t = - (norm.dot(e.pos) + wall.c) / det;
    hits.add(t, e.pos + t*e.vel, norm);
  }
}

static int gridCell(const Simulation::Params &p, Scalar v)
{
  return qBound(0, int(v / p.gridStep), p.gridWidth-1);
}

void Simulation::advanceElectron(Simulation::Electron &e,
                                 Scalar time,
                                 Scalar dt)
{
  HitSorter hits(e.pos, dt);
  Point straightPos = e.pos + dt*e.vel;

  // Bounce off walls
  for (int j=0; j<4; j++)
    collideWithWall(hits, e, m_edges[j]);

  // Bounce off ions
  // Find potentially colliding ions (should be ~1 each time)
  int x1 = gridCell(m_params, e.pos.x);
  int x2 = gridCell(m_params, straightPos.x);
  if (x2 < x1) swap(x1, x2);

  int y1 = gridCell(m_params, e.pos.y);
  int y2 = gridCell(m_params, straightPos.y);
  if (y2 < y1) swap(y1, y2);

  for(int y=y1; y<=y2; y++)
    for (int x=x1; x<=x2; x++)
      collideWithIon(time, hits, e, m_ions[y*m_params.gridWidth + x]);

  if (!hits.empty())
  {
    e.pos = hits.pos();
    if (e.vel.dot(hits.norm())) // Need to reflect
      e.vel += (-2 * e.vel.dot(hits.norm())) * hits.norm();
    e.vel += hits.vel(); // FIXME

    //qDebug() << "Hit:" << hits.pos() << hits.norm() << "@" << hits.time();

    e.pos += (dt-hits.time()) * e.vel;
    /*
    // Process remaining collisions within current timeslot
    dt -= hits.time();
    if (dt > eps)
      advanceElectron(e, time+hits.time(), dt);
      */
  }
  else
    e.pos = straightPos;
}

void Simulation::advanceTime(Scalar dt)
{
  //qDebug() << "-------------- Frame" << m_time << "->" << m_time+dt << "--------------";
#pragma omp parallel for
  for (int i=0; i<m_electrons.size(); i++)
    advanceElectron(m_electrons[i], m_time, dt);

  m_time += dt;
}

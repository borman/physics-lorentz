#ifndef PHYSMODEL_H
#define PHYSMODEL_H

#include <vector>
#include <QObject>

#include "Geom.h"

class HitSorter;

class Simulation: public QObject
{
  Q_OBJECT
  public:
    enum Distribution
    {
      Equal,
      Uniform
    };
    struct Params
    {
      Geom::Scalar gridStep;
      int gridWidth;
      int gridHeight;

      Geom::Scalar ionBaseRadius;
      Geom::Scalar ionDeltaRadius;
      Geom::Scalar ionOscillSpeed;
      Distribution ionPhaseDistribution;

      Geom::Scalar electronBaseSpeed;
      size_t electronCount;
    };
    struct Electron
    {
      // computed:
      Geom::Point pos;
      Geom::Point vel;
    };
    struct Ion
    {
      // constant:
      Geom::Point pos;
      Geom::Scalar phase;
      // computed:
      Geom::Scalar radius(const Params &p, Geom::Scalar time) const;
      Geom::Scalar radiusSpeed(const Params &p, Geom::Scalar time) const;
    };

    Simulation(const Params &p, QObject *parent = 0);

    void reset(const Params &p);

    void advanceTime(Geom::Scalar dt);
    Geom::Scalar time() const { return m_time; }

    Geom::Scalar width() const { return m_width; }
    Geom::Scalar height() const { return m_height; }

    size_t ionCount() const { return m_ions.size(); }
    Geom::Point ionPosition(int i) const { return m_ions[i].pos; }
    Geom::Scalar ionRadius(int i) const { return m_ions[i].radius(m_params, m_time); }

    size_t electronCount() const { return m_electrons.size(); }
    Geom::Point electronPosition(int i) const { return m_electrons[i].pos; }
  private:
    void advanceElectron(Electron &e, Geom::Scalar time, Geom::Scalar dt);
    void collideWithIon(Geom::Scalar time,
                        HitSorter &hits, 
                        const Electron &e,
                        const Ion &ion);
    void collideWithWall(HitSorter &hits, 
                         const Electron &e, 
                         const Geom::Line &wall);

    Params m_params;
    Geom::Scalar m_time;
    Geom::Scalar m_width;
    Geom::Scalar m_height;

    Geom::Line m_edges[4];

    std::vector<Ion> m_ions;
    std::vector<Electron> m_electrons;
};

#endif // PHYSMODEL_H


#ifndef PHYSMODEL_H
#define PHYSMODEL_H

#include <vector>
#include <QObject>
#include "geom.h"

class PhysModel: public QObject
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

      int electronCount;
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
      Geom::Scalar radius;
    };

    PhysModel(const Params &p, QObject *parent = 0);

    void advanceTime(Geom::Scalar dt);
    Geom::Scalar time() const { return m_time; }

    Geom::Scalar width() const { return m_width; }
    Geom::Scalar height() const { return m_height; }

    int ionCount() const { return m_ions.size(); }
    Geom::Point ionPosition(int i) const { return m_ions[i].pos; }
    Geom::Scalar ionRadius(int i) const { return m_ions[i].radius; }

    int electronCount() const { return m_electrons.size(); }
    Geom::Point electronPosition(int i) const { return m_electrons[i].pos; }
  private:

    Params m_params;
    Geom::Scalar m_time;
    Geom::Scalar m_width;
    Geom::Scalar m_height;

    Geom::Line m_edges[4];

    std::vector<Ion> m_ions;
    std::vector<Electron> m_electrons;
};

#endif // PHYSMODEL_H


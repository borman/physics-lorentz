#ifndef SIMULATIONVIEWER_H
#define SIMULATIONVIEWER_H

#include <QGLWidget>
#include "Simulation.h"

class SimulationViewer : public QGLWidget
{
  Q_OBJECT
public:
  explicit SimulationViewer(Simulation *sim, QWidget *parent = 0);

  void initializeGL();
  void resizeGL(int w, int h);
  void paintGL();

  QSize minimumSizeHint() { return QSize(100, 100); }
  QSize sizeHint() { return QSize(m_sim->width(), m_sim->height()); }

  private slots:
    void onTimer();

  private:
    Simulation *m_sim;
};

#endif // SIMULATIONVIEWER_H

#ifndef SIMULATIONVIEWER_H
#define SIMULATIONVIEWER_H

#include <QGLWidget>
#include "Simulation.h"

class QTimer;

class SimulationViewer : public QGLWidget
{
    Q_OBJECT
  public:
    explicit SimulationViewer(QWidget *parent = 0);

    void setSimulation(Simulation *sim);
    Simulation *simulation() const { return m_sim; }

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    QSize minimumSizeHint() const { return QSize(100, 100); }
    QSize sizeHint() const;
    int heightForWidth(int w) const;

  public slots:
    void run(bool enabled);
    void step();

  private slots:
    void setupViewport(int w, int h);
    void simulationChanged();
  private:
    Simulation *m_sim;
    QTimer *m_timer;
    bool m_needResize;
};

#endif // SIMULATIONVIEWER_H

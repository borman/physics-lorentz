#ifndef SIMULATIONSTATS_H
#define SIMULATIONSTATS_H

#include <QWidget>

class SpeedDistributionPlot;
class Simulation;

class SimulationStats : public QWidget
{
  Q_OBJECT
public:
  explicit SimulationStats(QWidget *parent = 0);
  void setSimulation(Simulation *sim);

private slots:
  void simulationUpdated();

private:
  SpeedDistributionPlot *m_sdPlot;
  Simulation *m_sim;
};

#endif // SIMULATIONSTATS_H

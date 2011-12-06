#ifndef SIMULATIONSTATS_H
#define SIMULATIONSTATS_H

#include <QWidget>

class SpeedDistributionPlot;

class SimulationStats : public QWidget
{
  Q_OBJECT
public:
  explicit SimulationStats(QWidget *parent = 0);

signals:

public slots:
private:
  SpeedDistributionPlot *m_sdPlot;
};

#endif // SIMULATIONSTATS_H

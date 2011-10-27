#include <QApplication>
#include <QGraphicsView>
#include <QTime>
#include <QDebug>

#include "Simulation.h"
#include "SimulationViewer.h"

int main(int argc, char **argv)
{
  QApplication app(argc, argv);

  Simulation::Params param;
  param.gridStep = 50;
  param.gridWidth = 5;
  param.gridHeight = 5;
  param.ionBaseRadius = 10;
  param.ionDeltaRadius = 2;
  param.ionOscillSpeed = 10;
  param.ionPhaseDistribution = Simulation::Uniform;
  param.electronBaseSpeed = 30;
  param.electronCount = 1000;

  Simulation sim(param);

  SimulationViewer sv(&sim);
  sv.resize(sv.sizeHint());
  sv.show();

  /*
  long long count = 0;
  QTime time;
  time.start();
  while (true)
  {
    sim.advanceTime(1.0/60);
    count++;
    if (count % 100000 == 0)
    {
      qDebug() << "simulation speed:" << (double(count)*1000.0/time.elapsed()) << "fps";
      count = 0;
      time.restart();
    }
  }
  */

  return app.exec();
}

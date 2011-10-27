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
  param.gridWidth = 4;
  param.gridHeight = 4;
  param.ionBaseRadius = 10;
  param.ionDeltaRadius = 5;
  param.ionOscillSpeed = 3;
  param.ionPhaseDistribution = Simulation::Uniform;
  param.electronBaseSpeed = 50;
  param.electronCount = 200;

  Simulation sim(param);

#if 1
  // Visual simulation
  SimulationViewer sv(&sim);
  sv.resize(sv.sizeHint());
  sv.show();

  return app.exec();
#else
  // Non-interactive simulation-only benchmark
  long long count = 0;
  QTime time;
  time.start();
  while (true)
  {
    sim.advanceTime(1.0/60);
    count++;
    if (count % 1000 == 0)
    {
      qDebug() << "simulation speed:" << (double(count)*1000.0/time.elapsed()) << "fps";
      count = 0;
      time.restart();
    }
  }
  return 0;
#endif
}

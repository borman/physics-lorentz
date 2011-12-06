#include <cstdlib>

#include <QHBoxLayout>
#include <QFont>
#include <QVector>

#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_intervalcurve.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_canvas.h>
#include <qwt_series_data.h>

#include "SimulationStats.h"

class SpeedDistributionPlot: public QwtPlot
{
  Q_OBJECT
public:
  SpeedDistributionPlot(QWidget *parent = 0)
    : QwtPlot(parent)
  {
    // Fonts
    QFont titleFont = font();
    titleFont.setPointSize(titleFont.pointSize()+2);
    titleFont.setBold(true);
    QFont axisTitleFont = font();
    axisTitleFont.setPointSize(axisTitleFont.pointSize()+1);
    axisTitleFont.setBold(true);
    QFont tickFont = font();

    // Title
    QwtText plotTitle(tr("Speed distribution"));
    plotTitle.setFont(titleFont);
    setTitle(plotTitle);

    QwtText axisTitle;
    axisTitle.setFont(axisTitleFont);

    // X axis
    axisTitle.setText(tr("Time"));
    setAxisTitle(QwtPlot::xBottom, axisTitle);
    setAxisFont(QwtPlot::xBottom, tickFont);

    // Y axis
    axisTitle.setText(tr("Speed"));
    setAxisTitle(QwtPlot::yLeft, axisTitle);
    setAxisFont(QwtPlot::yLeft, tickFont);

    // Grid
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->attach(this);

    // Curves
    m_rangeCurve = new QwtPlotIntervalCurve(tr("range"));
    m_rangeCurve->attach(this);
    m_rangeCurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    m_rangeCurve->setPen(QColor(30, 30, 255));
    m_rangeCurve->setBrush(QColor(200, 200, 255, 150));

    m_meanCurve = new QwtPlotCurve(tr("mean"));
    m_meanCurve->attach(this);
    m_meanCurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    m_meanCurve->setPen(QPen(QColor(30, 180, 30), 2));

    // Panning
    QwtPlotPanner *panner = new QwtPlotPanner(canvas());
    panner->setOrientations(Qt::Horizontal);
    panner->setMouseButton(Qt::LeftButton);

    // Zooming
    QwtPlotMagnifier *mag = new QwtPlotMagnifier(canvas());
    mag->setMouseButton(Qt::NoButton);
    mag->setAxisEnabled(QwtPlot::xBottom, true);
    mag->setAxisEnabled(QwtPlot::yLeft, false);


    // Canvas palette
    canvas()->setPalette(Qt::white);
    canvas()->setPaintAttribute(QwtPlotCanvas::ImmediatePaint, true);
  }

  void addData(double time, double min, double mean, double max)
  {
    m_timeData.push_back(time);
    m_meanData.push_back(mean);
    m_rangeData.push_back(QwtIntervalSample(time, min, max));
    m_meanCurve->setSamples(m_timeData, m_meanData);
    m_rangeCurve->setSamples(m_rangeData);
  }
private:
  QwtPlotCurve *m_meanCurve;
  QwtPlotIntervalCurve *m_rangeCurve;
  QVector<double> m_timeData;
  QVector<double> m_meanData;
  QVector<QwtIntervalSample> m_rangeData;
};

SimulationStats::SimulationStats(QWidget *parent) :
    QWidget(parent)
{
  m_sdPlot = new SpeedDistributionPlot(this);

  for (int i=0; i<2000; i++)
  {
    double mean = 50 + double(rand())/RAND_MAX;
    double d1 = 0.8 + 0.4*double(rand())/RAND_MAX;
    double d2 = 0.8 + 0.4*double(rand())/RAND_MAX;
    m_sdPlot->addData(i/60.0, mean-d1, mean, mean+d2);
  }

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(m_sdPlot);
  setLayout(l);

  setMinimumHeight(200);
}

#include "SimulationStats.moc"

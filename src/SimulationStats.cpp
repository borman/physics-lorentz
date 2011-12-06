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
#include <qwt_plot_layout.h>
#include <qwt_series_data.h>

#include "Simulation.h"
#include "SimulationStats.h"

template<class T>
class LogSeries: public QwtSeriesData<T>
{
public:
  virtual void addData(const T &d)
  {
    m_data.push_back(d);
  }

  virtual T sample(size_t i) const { return m_data[i]; }
  virtual size_t size() const { return m_data.size(); }
private:
  QVector<T> m_data;
};

class PointLogSeries: public LogSeries<QPointF>
{
public:
  PointLogSeries(): m_minX(0), m_maxX(0), m_minY(0), m_maxY(0) {}
  virtual void addData(const QPointF &d)
  {
    if (size() == 0)
    {
      m_minX = m_maxX = d.x();
      m_minY = m_maxY = d.y();
    }
    else
    {
      m_minX = qMin(m_minX, d.x());
      m_minY = qMin(m_minY, d.y());
      m_maxX = qMax(m_maxX, d.x());
      m_maxY = qMax(m_maxY, d.y());
    }
    LogSeries<QPointF>::addData(d);
  }
  virtual QRectF boundingRect() const { return QRectF(m_minX, m_minY, m_maxX-m_minX, m_maxY-m_minY); }
private:
  double m_minX, m_maxX, m_minY, m_maxY;
};

class IntervalLogSeries: public LogSeries<QwtIntervalSample>
{
public:
  IntervalLogSeries(): m_minX(0), m_maxX(0), m_minY(0), m_maxY(0) {}
  virtual void addData(const QwtIntervalSample &d)
  {
    if (size() == 0)
    {
      m_minX = m_maxX = d.value;
      m_minY = d.interval.minValue();
      m_maxY = d.interval.maxValue();
    }
    else
    {
      m_minX = qMin(m_minX, d.value);
      m_minY = qMin(m_minY, d.interval.minValue());
      m_maxX = qMax(m_maxX, d.value);
      m_maxY = qMax(m_maxY, d.interval.maxValue());
    }
    LogSeries<QwtIntervalSample>::addData(d);
  }
  virtual QRectF boundingRect() const { return QRectF(m_minX, m_minY, m_maxX-m_minX, m_maxY-m_minY); }
private:
  double m_minX, m_maxX, m_minY, m_maxY;
};

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
    m_rangeCurve->setData(&m_rangeData);
    m_rangeCurve->attach(this);
    m_rangeCurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    m_rangeCurve->setPen(QColor(30, 30, 255));
    m_rangeCurve->setBrush(QColor(200, 200, 255, 150));

    m_meanCurve = new QwtPlotCurve(tr("mean"));
    m_meanCurve->setData(&m_meanData);
    m_meanCurve->attach(this);
    m_meanCurve->setRenderHint(QwtPlotItem::RenderAntialiased, true);
    m_meanCurve->setPen(QPen(QColor(30, 180, 30), 2));

    // Panning
    QwtPlotPanner *panner = new QwtPlotPanner(canvas());
    //panner->setOrientations(Qt::Horizontal);
    panner->setMouseButton(Qt::LeftButton);

    // Zooming
    QwtPlotMagnifier *mag = new QwtPlotMagnifier(canvas());
    mag->setMouseButton(Qt::NoButton);
    //mag->setAxisEnabled(QwtPlot::xBottom, true);
    //mag->setAxisEnabled(QwtPlot::yLeft, true);

    // Canvas
    canvas()->setPalette(Qt::white);
    canvas()->setPaintAttribute(QwtPlotCanvas::ImmediatePaint, true);

    plotLayout()->setSpacing(0);
  }

  void addData(double time, double min, double mean, double max)
  {
    m_meanData.addData(QPointF(time, mean));
    m_rangeData.addData(QwtIntervalSample(time, min, max));
    replot();
  }
private:
  QwtPlotCurve *m_meanCurve;
  QwtPlotIntervalCurve *m_rangeCurve;
  PointLogSeries m_meanData;
  IntervalLogSeries m_rangeData;
};

SimulationStats::SimulationStats(QWidget *parent) :
    QWidget(parent), m_sim(0)
{
  m_sdPlot = new SpeedDistributionPlot(this);

  QHBoxLayout *l = new QHBoxLayout();
  l->addWidget(m_sdPlot);
  setLayout(l);

  setMinimumHeight(200);
}

void SimulationStats::setSimulation(Simulation *sim)
{
  m_sim = sim;
  connect(sim, SIGNAL(advanced()), SLOT(simulationUpdated()));
}

void SimulationStats::simulationUpdated()
{
  double max, min;
  max = min = m_sim->electron(0).vel.length();
  double mean = 0;
  for (int i=0; i<m_sim->electronCount(); i++)
  {
    double v = m_sim->electron(i).vel.length();
    mean += v;
    max = qMax(max, v);
    min = qMin(min, v);
  }
  mean /= m_sim->electronCount();
  m_sdPlot->addData(m_sim->time(), min, mean, max);
}

#include "SimulationStats.moc"

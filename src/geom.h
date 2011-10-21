#ifndef GEOM_H
#define GEOM_H

#include <cmath>
#include <QPointF>
#include <QRectF>

namespace Geom
{

typedef qreal Scalar;

typedef QPointF Point;
typedef QRectF Rect;

class Line
{
  public:
    Line(Scalar _a, Scalar _b, Scalar _c)
      : a(_a), b(_b), c(_c) {}

    Scalar operator * (const Point &p) { return a*p.x() + b*p.y() + c; }
    void normalize()
    {
      Scalar l = sqrt(a*a + b*b);
      a /= l;
      b /= l;
      c /= l;
    }
  private:
    Scalar a, b, c;
};

}

#endif // GEOM_H

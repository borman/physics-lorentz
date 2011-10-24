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
    Line(Scalar _a=0, Scalar _b=0, Scalar _c=0)
      : a(_a), b(_b), c(_c) {}
    Line(const Point &p1, const Point &p2)
    {
      a = p1.y() - p2.y();
      b = p2.x() - p1.x();
      c = -(a*p1.x() + b*p1.y());
    }

    Scalar operator *(const Point &p) const { return a*p.x() + b*p.y() + c; }
    void normalize()
    {
      Scalar l = sqrt(a*a + b*b);
      a /= l;
      b /= l;
      c /= l;
    }
    Point normal() const { return Point(a, b); }

    bool crossedBy(const Point &p1, const Point &p2) const
    {
      Scalar d1 = (*this) * p1;
      Scalar d2 = (*this) * p2;
      //return (d1 < 0 && d2 > 0) || (d1 > 0 && d2 < 0);
      return d1*d2 < 0;
    }
  private:
    Scalar a, b, c;
};

}

#endif // GEOM_H

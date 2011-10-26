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

Scalar operator *(const Point &p1, const Point &p2);

class Line
{
  public:
    Line(Scalar _a=0, Scalar _b=0, Scalar _c=0)
      : a(_a), b(_b), c(_c) {}
    Line(const Point &p1, const Point &p2);

    Scalar operator *(const Point &p) const { return a*p.x() + b*p.y() + c; }
    void normalize();
    Point normal() const { return Point(a, b); }
    bool crossedBy(const Point &p1, const Point &p2) const;
  private:
    Scalar a, b, c;
};

}

#endif // GEOM_H

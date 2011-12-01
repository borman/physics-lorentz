#ifndef GEOM_H
#define GEOM_H

#include <cmath>
#include <cfloat>
#include <QPointF>
#include <QRectF>
#include <QDebug>

namespace Geom
{

typedef double Scalar;

const Scalar eps = DBL_EPSILON;

class Point
{
  public:
    Point(Scalar _x=0, Scalar _y=0): x(_x), y(_y) {}

    Point operator +(const Point &p) const { return Point(x+p.x, y+p.y); }
    Point operator -(const Point &p) const { return Point(x-p.x, y-p.y); }
    Point &operator +=(const Point &p) { x+=p.x; y+=p.y; return *this; }

    Point normalized() const { Scalar l=length(); return Point(x/l, y/l); }

    Scalar dot(const Point &p) const { return x*p.x + y*p.y; }
    Scalar sqr() const { return x*x + y*y; }
    Scalar length() const { return sqrt(x*x + y*y); }

    operator QPointF() const { return QPointF(x, y); }

    Scalar x, y;
};

static inline Point operator *(Scalar k, const Point &p)
{
  return Point(k*p.x, k*p.y);
}

static inline Point operator *(const Point &p, Scalar k)
{
  return k*p;
}


class Line
{
  public:
    Line(Scalar _a=0, Scalar _b=0, Scalar _c=0)
      : a(_a), b(_b), c(_c) {}
    Line(const Point &p1, const Point &p2);

    Scalar operator *(const Point &p) const { return a*p.x + b*p.y + c; }
    void normalize();
    Point normal() const { return Point(a, b); }
    bool crossedBy(const Point &p1, const Point &p2) const;
    Point cross(const Line &l) const;


    Scalar a, b, c;
};

}


// Debug output
static inline QDebug operator<<(QDebug dbg, const Geom::Point &p)
{
  dbg.nospace() << "(" << p.x << "," << p.y << ")";
  return dbg.space();
}

#endif // GEOM_H

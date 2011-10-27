#include "Geom.h"

namespace Geom
{

Line::Line(const Point &p1, const Point &p2)
{
  a = p1.y - p2.y;
  b = p2.x - p1.x;
  c = -(a*p1.x + b*p1.y);
}

void Line::normalize()
{
  Scalar l = sqrt(a*a + b*b);
  a /= l;
  b /= l;
  c /= l;
}

bool Line::crossedBy(const Point &p1, const Point &p2) const
{
  Scalar d1 = (*this) * p1;
  Scalar d2 = (*this) * p2;
  return d1*d2 < -eps;
}

Point Line::cross(const Line &l) const
{
  double d = a*l.b - b*l.a;
  return Point((b*l.c - c*l.b)/d,
               (c*l.a - a*l.c)/d);
}

}

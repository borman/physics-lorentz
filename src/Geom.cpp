#include "Geom.h"
namespace Geom
{

Scalar operator *(const Point &p1, const Point &p2)
{
  return p1.x()*p2.x() + p1.y()*p2.y();
}

Line::Line(const Point &p1, const Point &p2)
{
  a = p1.y() - p2.y();
  b = p2.x() - p1.x();
  c = -(a*p1.x() + b*p1.y());
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
  //return (d1 < 0 && d2 > 0) || (d1 > 0 && d2 < 0);
  return d1*d2 < 0;
}

}

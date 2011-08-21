#include "vector2d.h"

double Vector2D::dotProduct(const Vector2D *vec) const
{
	return x*vec->x + y*vec->y;
}

double Vector2D::length() const
{
	return std::sqrt(x*x + y*y);
}

Vector2D Vector2D::perpendicular() const
{
	return Vector2D(y,-x);
}

Vector2D Vector2D::unit() const
{
	return *this/length();
}
	
Vector2D& Vector2D::operator/=(const double scalar)
{
	*this = *this / scalar;
	return *this;
}

Vector2D& Vector2D::operator*=(const double scalar)
{
	*this = *this * scalar;
	return *this;
}

Vector2D& Vector2D::operator+=(const Vector2D &v)
{
	*this = *this + v;
	return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D &v)
{
	*this = *this - v;
	return *this;
}

Vector2D Vector2D::operator/(double scalar) const
{
	return Vector2D(x/scalar,y/scalar);
}

Vector2D Vector2D::operator*(double scalar) const
{
	return Vector2D(x*scalar,y*scalar);
}

Vector2D Vector2D::operator-(const Vector2D &v) const
{
	return Vector2D(x-v.x, y-v.y);
}

Vector2D Vector2D::operator+(const Vector2D &v) const
{
	return Vector2D(x+v.x, y+v.y);
}

bool Vector2D::operator!=(const Vector2D &v) const
{
	return (x != v.x) || (y != v.y);
}

std::ostream& operator <<(std::ostream &os, const Vector2D &v)
{
	os << "(" << v.x << ", " << v.y << ")";
	return os;
}
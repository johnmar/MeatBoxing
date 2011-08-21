#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>
#include <iostream>

class Vector2D
{
private:

public:
	double x,y;
	
	Vector2D(const double x, const double y) : x(x), y(y) {}
	Vector2D() : x(0), y(0) {}
	double dotProduct(const Vector2D *vec) const;
	double length() const;
	Vector2D perpendicular() const;
	Vector2D unit() const;
	
	Vector2D& operator/=(const double scalar);
	Vector2D& operator*=(const double scalar);
	Vector2D& operator+=(const Vector2D &v);
	Vector2D& operator-=(const Vector2D &v);

	Vector2D operator/(const double scalar) const;
	Vector2D operator*(const double scalar) const;
	Vector2D operator-(const Vector2D &v) const;
	Vector2D operator+(const Vector2D &v) const;
	bool operator!=(const Vector2D &v) const;
};

std::ostream& operator <<(std::ostream &os, const Vector2D &v);

#endif //VECTOR2D_H
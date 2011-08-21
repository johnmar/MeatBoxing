#ifndef SOLID_H
#define SOLID_H

#include "drawable.h"
#include "vector2d.h"

class Solid : public Drawable
{
public:
	virtual void draw( Vector2D currentDrawingPosition, Vector2D currentScale ) = 0;
	virtual void generateList() = 0;
	virtual char* drawableType() { return "Solid"; }

	virtual char* solidType() const { return "Solid"; }

	Vector2D position;
	Vector2D velocity;
	Vector2D hitboxSize;


	Solid( GLuint *texture = NULL ) : Drawable(texture) {}
};

#endif // SOLID_H
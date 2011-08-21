#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "solid.h"
#include "vector2d.h"

class Projectile : public Solid
{
public:
	virtual void draw( Vector2D currentDrawingPosition, Vector2D currentScale );
	virtual void generateList();

	virtual char* solidType() const { return "Projectile"; }

	Projectile( double xPos, double yPos );
};

#endif // PROJECTILE_H
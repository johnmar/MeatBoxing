#include "drawable.h"

#ifndef BACKGROUND_H
#define BACKGROUND_H

class Background : public Drawable
{
public:
	virtual void draw( Vector2D currentDrawingPosition, Vector2D currentScale );
	virtual void generateList();
	virtual char* drawableType() { return "Scenery"; }

	Background(char* filepath, double relativeWidth, double relativeHeight);

private:
	int width, height;
};

#endif //BACKGROUND_H
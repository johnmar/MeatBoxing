#ifndef CONTROLLABLE_H
#define CONTROLLABLE_H

#include "solid.h"
#include "vector2d.h"
#include "SDL.h"
#include <map>
#include <string>

using std::map;
using std::string;
using std::pair;

class Controllable : public Solid
{
public:
	virtual void draw( Vector2D currentDrawingPosition, Vector2D currentScale );
	virtual void generateList();
	
	virtual char* solidType() const { return "Controllable"; }
	virtual void applyControls( double deltaTime );
	virtual Drawable* getCreation();
	
	double imageNumber;
	double aimAngle;
	Vector2D bodyOffset;
	map<string, BodyPart*> bodyParts;
	map<string, int> numberOfImages;
	map<string, int> textureIndex;
	map<string, Vector2D> imageSize;
	map<pair<string,string>, Vector2D> connectionPoint;
	map<string, bool> key;
	map<SDLKey, string> keyBind;
	Controllable(double xPos, double yPos);
};

#endif // CONTROLLABLE_H
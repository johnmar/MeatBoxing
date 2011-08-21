#ifndef DRAWABLE_H
#define DRAWABLE_H

#include "SDL_opengl.h"
#include "SDL_image.h"
#include "vector2d.h"

#include <vector>
#include <queue>
#include <set>

using std::pair;
using std::vector;
using std::priority_queue;
using std::multiset;

class Pair
{
public:
	double depth;
	GLuint list;

	Pair( double depth, GLuint list ) : depth(depth), list(list) {}
	bool operator< ( const Pair &b ) const {return depth > b.depth;}
};

class Drawable
{
public:
	static const int defaultScreenWidth = 1024, defaultScreenHeight = 768;

	virtual void draw( Vector2D currentDrawingPosition, Vector2D currentScale ) = 0;
	virtual void generateList() = 0;
	virtual char* drawableType() { return "Drawable"; }

	static void setScreenSize(int width, int height);
	static GLuint loadTexture(char* filepath);
	static GLuint* loadTexture(vector<char*> images);
	static void addListElement(double depth, GLuint list);
	static void showAllElements();

	Drawable( GLuint *texture = NULL );
	
	GLuint *texture;
	bool scheduledForRemoval;
	GLuint listReference;
	static int screenWidth, screenHeight;
	static priority_queue<Pair> elementsToDraw;
};

class BodyPart : public Drawable
{
public:
	BodyPart( GLuint *texture, vector<pair<Vector2D,BodyPart*>> connectedParts, Vector2D imageSize, double depth ) : Drawable(texture), connectedParts(connectedParts), imageSize(imageSize), imageIndex(0), depth(depth) {}

	virtual void draw( Vector2D currentDrawingPosition, Vector2D currentScale );
	virtual void generateList();
	void setImageToDraw( unsigned int i ) { imageIndex = i; }

	double depth;
	unsigned int imageIndex;
	Vector2D imageSize;
	vector<pair<Vector2D,BodyPart*>> connectedParts;
};

#endif //DRAWABLE_H
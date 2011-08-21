#ifndef WORLD_H
#define WORLD_H

#include "drawable.h"
#include <vector>

using std::vector;

struct Block
{
	int x, y, type;
};

struct Line
{
	Vector2D point1, point2;
	
	Line( double x1, double y1, double x2, double y2 ) : point1(x1,y1), point2(x2,y2) {}
	Line() {}
};

struct LineVector
{
	vector<Line> lines;
};

class World : public Drawable
{
public:
	static const float gridSize;

	virtual void draw( Vector2D currentDrawingPosition, Vector2D currentScale );
	virtual void generateList();
	virtual char* drawableType() { return "World"; }
	
	vector<Block> getBlocks() const;
	bool isSolid( int gridX, int gridY );

	World(char* filePath);

private:
	vector<Block> blocks;
	int blockSize;
};

#endif // WORLD_H
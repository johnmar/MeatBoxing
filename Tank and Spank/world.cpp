#include "world.h"

#include <iostream>
#include <fstream>

using std::ifstream;

const float World::gridSize = 25.0f;

vector<Block> World::getBlocks() const
{
	return blocks;
}

void World::draw( Vector2D currentDrawingPosition, Vector2D currentScale )
{
	if (listReference != 0)
	{
		GLuint list = glGenLists(1);
		glNewList(list, GL_COMPILE);
		{
			glCallList(listReference);
		}
		glEndList();
		Drawable::addListElement(0.9, list);
	}
}
void World::generateList()
{
	listReference = glGenLists(1);
	glNewList(listReference, GL_COMPILE);
	{
		for( unsigned int i = 0; i < blocks.size(); ++i)
		{
			glBindTexture(GL_TEXTURE_2D, *texture);
			glBegin(GL_QUADS);
			{
				float x = blocks[i].x * gridSize;
				float y = blocks[i].y * gridSize;

				float size = blockSize * gridSize;

				glTexCoord2f(0.0f, 0.0f); glVertex3f(x,y,0.9f);
				glTexCoord2f(0.0f, 1.0f); glVertex3f(x,y+size,0.9f);
				glTexCoord2f(1.0f, 1.0f); glVertex3f(x+size,y+size,0.9f);
				glTexCoord2f(1.0f, 0.0f); glVertex3f(x+size,y,0.9f); 
			}
			glEnd();
		}
	}
	glEndList();
}

World::World(char* filePath)
{
	texture = new GLuint();
	*texture = Drawable::loadTexture("media/PHdirt.png");

	ifstream worldFile(filePath);

	worldFile >> blockSize;
	
	int tempX, tempY, tempType;
	Block insert;
	while( !worldFile.eof() )
	{
		worldFile >> tempType >> tempX >> tempY;

		insert.type = tempType;
		insert.x = tempX;
		insert.y = tempY;

		blocks.push_back(insert);
	}
}

bool World::isSolid( int gridX, int gridY )
{
	for( unsigned int i = 0; i < blocks.size(); ++i )
	{
		if( blocks[i].type = 1 && ( blocks[i].x == gridX || blocks[i].x == gridX-1 ) && ( blocks[i].y == gridY || blocks[i].y == gridY-1 ) )
			return true;
	}
	return false;
}
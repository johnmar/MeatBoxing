#include "background.h"
#include <iostream>

void Background::draw( Vector2D currentDrawingPosition, Vector2D currentScale )
{
	if (listReference != 0)
	{
		GLuint list = glGenLists(1);
		glNewList(list, GL_COMPILE);
		{
			glCallList(listReference);
		}
		glEndList();
		Drawable::addListElement(0.0, list);
	}
}
void Background::generateList()
{
	listReference = glGenLists(1);
	glNewList(listReference, GL_COMPILE);
	{
		glBindTexture(GL_TEXTURE_2D, *texture);
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3f(0.0f,0.0f,0.0f);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(0.0f,(float)(height),0.0f);
			glTexCoord2f(1.0f, 1.0f); glVertex3f((float)(width),(float)(height),0.0f);
			glTexCoord2f(1.0f, 0.0f); glVertex3f((float)(width),0.0f,0.0f); 
		}
		glEnd();
	}
	glEndList();
}

Background::Background(char* filepath, double relativeWidth, double relativeHeight)
	: Drawable()
{
	texture = new GLuint();
	*texture = Drawable::loadTexture(filepath);
	width = (int)(screenWidth * relativeWidth);
	height = (int)(screenHeight * relativeHeight);
}
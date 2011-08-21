#include "drawable.h"
#include <iostream>

using std::make_pair;

int Drawable::screenWidth = 0;
int Drawable::screenHeight = 0;
priority_queue<Pair> Drawable::elementsToDraw;

void Drawable::setScreenSize(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
}

GLuint Drawable::loadTexture(char* filepath)
{
	vector<char*> f;
	f.push_back(filepath);
	return *Drawable::loadTexture(f);
}

GLuint* Drawable::loadTexture(vector<char*> images)
{
	GLuint *texture = new GLuint[images.size()];
	bool wrap = false;
	
	glGenTextures(images.size(), texture); // glDeleteTextures( 1, texture ); for å frigi
	for( unsigned int i = 0; i < images.size(); ++i )
	{
		SDL_Surface* surface = IMG_Load(images[i]);
		if (!surface)
		{
			std::cout << IMG_GetError() << std::endl << "filename: " << images[i];
			exit(1);
		}
		glPixelStorei(GL_UNPACK_ALIGNMENT,4);
		glBindTexture(GL_TEXTURE_2D, texture[i]);
		SDL_PixelFormat *format = surface->format;
		if (format->Amask)
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, 4,
			surface->w, surface->h, GL_RGBA,GL_UNSIGNED_BYTE, surface->pixels);
		}
		else
		{
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3,
			surface->w, surface->h, GL_RGB, GL_UNSIGNED_BYTE, surface->pixels);
		}
		SDL_FreeSurface(surface);
	}
	return texture;
}

void Drawable::addListElement(double depth, GLuint list)
{
	elementsToDraw.push( Pair( depth, list ) );
}

void Drawable::showAllElements()
{
	while( !elementsToDraw.empty() )
	{
		glCallList( elementsToDraw.top().list );
		glDeleteLists( elementsToDraw.top().list, 1 );
		elementsToDraw.pop();
	}
}

Drawable::Drawable( GLuint *texture ) : texture(texture)
{
	scheduledForRemoval = false;
	listReference = 0;
}

void BodyPart::draw( Vector2D currentDrawingPosition, Vector2D currentScale )
{
	if (listReference != 0)
	{
		GLuint list = glGenLists(1);
		glNewList(list, GL_COMPILE);
		{
			glTranslated( currentDrawingPosition.x, currentDrawingPosition.y, 0.0 );
			glScaled( currentScale.x, currentScale.y, 1.0 );
			glBindTexture( GL_TEXTURE_2D, texture[imageIndex] );
			glTranslated( 0.0, 0.0, depth );
			glCallList( listReference );
			glTranslated( 0.0, 0.0, -depth );
			glScaled( 1/currentScale.x, 1/currentScale.y, 1.0 );
			glTranslated( -currentDrawingPosition.x, -currentDrawingPosition.y, 0.0 );
		}
		glEndList();
		
		Drawable::addListElement( depth, list );
	}
	for( unsigned int i = 0; i < connectedParts.size(); ++i )
	{
		currentDrawingPosition += Vector2D( currentScale.x*connectedParts[i].first.x, currentScale.y*connectedParts[i].first.y );
		connectedParts[i].second->draw( currentDrawingPosition, currentScale );
		currentDrawingPosition -= Vector2D( currentScale.x*connectedParts[i].first.x, currentScale.y*connectedParts[i].first.y );
	}
}

void BodyPart::generateList()
{
	listReference = glGenLists(1);
	glNewList(listReference, GL_COMPILE);
	{
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3d(0.0,0.0,0.0);
			glTexCoord2f(0.0f, 1.0f); glVertex3d(0.0,imageSize.y,0.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3d(imageSize.x,imageSize.y,0.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3d(imageSize.x,0.0,0.0); 
		}
		glEnd();
	}
	glEndList();
}
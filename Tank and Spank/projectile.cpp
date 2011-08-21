#include "projectile.h"

void Projectile::draw( Vector2D currentDrawingPosition, Vector2D currentScale )
{
	if (listReference != 0)
	{
		GLuint list = glGenLists(1);
		glNewList(list, GL_COMPILE);
		{
			glTranslated( currentDrawingPosition.x, currentDrawingPosition.y, 0.0 );
			glScaled( currentScale.x, currentScale.y, 1.0 );

			glTranslated( position.x, position.y, 0.0 );
			glBindTexture(GL_TEXTURE_2D, *texture);
			glCallList( listReference );
			glTranslated( -position.x, -position.y, 0.0 );

			glScaled( 1/currentScale.x, 1/currentScale.y, 1.0 );
			glTranslated( -currentDrawingPosition.x, -currentDrawingPosition.y, 0.0 );
		}
		glEndList();
		Drawable::addListElement(0.01, list);
	}
}

void Projectile::generateList()
{
	listReference = glGenLists(1);
	glNewList(listReference, GL_COMPILE);
	{
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3d(0.0,0.0,0.0);
			glTexCoord2f(0.0f, 1.0f); glVertex3d(0.0,hitboxSize.y,0.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3d(hitboxSize.x,hitboxSize.y,0.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3d(hitboxSize.x,0.0,0.0); 
		}
		glEnd();
	}
	glEndList();
}

Projectile::Projectile(double xPos, double yPos)
	: Solid()
{
	position.x = xPos;
	position.y = yPos;

	velocity.x = 0.0;
	velocity.y = 0.0;

	hitboxSize.x = 20.0;
	hitboxSize.y = 20.0;

	texture = new GLuint;
	*texture = Drawable::loadTexture("media/projectile.png");
}
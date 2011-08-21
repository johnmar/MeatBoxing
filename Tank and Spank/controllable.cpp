#include "controllable.h"
#include "projectile.h"

#include <fstream>
using std::ifstream;
using std::make_pair;

void Controllable::draw( Vector2D currentDrawingPosition, Vector2D currentScale )
{
	if (listReference != 0)
	{
			
		currentDrawingPosition += Vector2D( position.x + bodyOffset.x, position.y + bodyOffset.y );
		if( velocity.x > 0 )
		{
			currentDrawingPosition += Vector2D( bodyParts["upperBody"]->imageSize.x, 0);
			currentScale.x *= -1;
		}
		
		bodyParts["leftLeg"]->setImageToDraw( (unsigned int)imageNumber );
		bodyParts["rightLeg"]->setImageToDraw( ((unsigned int)imageNumber + 6)%numberOfImages["leftLeg"] );
		bodyParts["upperBody"]->draw( currentDrawingPosition, currentScale );

		GLuint list = glGenLists(1);
		glNewList(list, GL_COMPILE);
		{
			glTranslated( currentDrawingPosition.x, currentDrawingPosition.y, 0.0 );
			glScaled( currentScale.x, currentScale.y, 1.0 );

			glTranslated( -50*cos( aimAngle ) + hitboxSize.x/2, -50*sin( aimAngle ) + hitboxSize.y/2, 0.0 );
			glBindTexture( GL_TEXTURE_2D, texture[textureIndex["aimDot"]] );
			glCallList( listReference+2 );
			glTranslated( 50*cos( aimAngle ) - hitboxSize.x/2, 50*sin( aimAngle ) - hitboxSize.y/2, 0.0 );

			glScaled( 1/currentScale.x, 1/currentScale.y, 1.0 );
			glTranslated( -currentDrawingPosition.x, -currentDrawingPosition.y, 0.0 );
		}
		glEndList();
		Drawable::addListElement( 0.0, list );

		imageNumber += 0.001*abs(velocity.x);
		if( imageNumber > 11 )
			imageNumber -= 11;
	}
}

void Controllable::generateList()
{
	double dist = 20.0;
	listReference = glGenLists(3);
	glNewList(listReference, GL_COMPILE);
	{
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3d(0.0,0.0,0.0);
			glTexCoord2f(0.0f, 1.0f); glVertex3d(0.0,hitboxSize.y,0.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3d(hitboxSize.x-dist,hitboxSize.y,0.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3d(hitboxSize.x-dist,0.0,0.0); 
		}
		glEnd();
	}
	glEndList();
	glNewList(listReference+1, GL_COMPILE);
	{
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3d(dist,0.0,0.0);
			glTexCoord2f(0.0f, 1.0f); glVertex3d(dist,hitboxSize.y,0.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3d(hitboxSize.x,hitboxSize.y,0.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3d(hitboxSize.x,0.0,0.0); 
		}
		glEnd();
	}
	glEndList();
	glNewList(listReference+2, GL_COMPILE);
	{
		glBegin(GL_QUADS);
		{
			glTexCoord2f(0.0f, 0.0f); glVertex3d(0.0,0.0,0.0);
			glTexCoord2f(0.0f, 1.0f); glVertex3d(0.0,5.0,0.0);
			glTexCoord2f(1.0f, 1.0f); glVertex3d(5.0,5.0,0.0);
			glTexCoord2f(1.0f, 0.0f); glVertex3d(5.0,0.0,0.0); 
		}
		glEnd();
	}
	glEndList();
}

void Controllable::applyControls( double deltaTime )
{
	velocity.y -= key["jump"] * 500;
	key["jump"] = false;
	if( key["moveLeft"] )
		velocity.x -= 1000 * deltaTime;
	else if( key["moveRight"] )
		velocity.x += 1000 * deltaTime;
	else
		velocity.x -= 5 * velocity.x * deltaTime;

	if( key["aimUp"] )
		aimAngle += 2*deltaTime;
	else if( key["aimDown"] )
		aimAngle -= 2*deltaTime;
	if( aimAngle > 1.5 )
		aimAngle = 1.5;
	if( aimAngle < -1.5 )
		aimAngle = -1.5;
}

Drawable* Controllable::getCreation()
{
	if( key["shoot"] )
	{
		Solid *ret = new Projectile( position.x + hitboxSize.x/2, position.y + hitboxSize.y/2);
		if( velocity.x > 0 )
			ret->velocity.x = 500*cos( aimAngle );
		else
			ret->velocity.x = -500*cos( aimAngle );
		ret->velocity.y = -500*sin( aimAngle );
		key["shoot"] = false;
		ret->generateList();
		return ret;
	}
	return NULL;
}

Controllable::Controllable(double xPos, double yPos)
	: Solid()
{
	key["moveRight"] = false;
	key["moveLeft"] = false;
	key["jump"] = false;
	key["shoot"] = false;
	key["aimUp"] = false;
	key["aimDown"] = false;
	
	keyBind[SDLK_RIGHT] = "moveRight";
	keyBind[SDLK_LEFT] = "moveLeft";
	keyBind[SDLK_SPACE] = "jump";
	keyBind[SDLK_LCTRL] = "shoot";
	keyBind[SDLK_UP] = "aimUp";
	keyBind[SDLK_DOWN] = "aimDown";

	position.x = xPos;
	position.y = yPos;

	velocity.x = 0.0;
	velocity.y = 0.0;

	hitboxSize.x = 110;
	hitboxSize.y = 165;

	bodyOffset.x = 10;
	bodyOffset.y = -20;

	aimAngle = 0.0;
	imageNumber = 0.0;

	textureIndex["leftLeg"] = 0;
	numberOfImages["leftLeg"] = 11;
	textureIndex["aimDot"] = 11;
	textureIndex["leftArm"] = 12;
	textureIndex["rightArm"] = 13;
	textureIndex["rightHand"] = 14;
	numberOfImages["rightHand"] = 3;
	textureIndex["upperBody"] = 17;

	vector<char*> images;
	images.push_back("media/leftleg/1.png");
	images.push_back("media/leftleg/2.png");
	images.push_back("media/leftleg/3.png");
	images.push_back("media/leftleg/4.png");
	images.push_back("media/leftleg/5.png");
	images.push_back("media/leftleg/6.png");
	images.push_back("media/leftleg/7.png");
	images.push_back("media/leftleg/8.png");
	images.push_back("media/leftleg/9.png");
	images.push_back("media/leftleg/10.png");
	images.push_back("media/leftleg/11.png");
	images.push_back("media/projectile.png");
	images.push_back("media/static/skold.png");
	images.push_back("media/static/armn.png");
	images.push_back("media/punchn/1.png");
	images.push_back("media/punchn/2.png");
	images.push_back("media/punchn/3.png");
	images.push_back("media/static/overkropp.png");

	texture = Drawable::loadTexture(images);
	
	double scale = 0.5;
	BodyPart *leftLeg = new BodyPart( &texture[textureIndex["leftLeg"]], vector<pair<Vector2D,BodyPart*>>(), Vector2D(550.0, 400.0)*scale, 0.4 );
	BodyPart *rightLeg = new BodyPart( &texture[textureIndex["leftLeg"]], vector<pair<Vector2D,BodyPart*>>(), Vector2D(550.0, 400.0)*scale, 0.6 );
	BodyPart *leftArm = new BodyPart( &texture[textureIndex["leftArm"]], vector<pair<Vector2D,BodyPart*>>(), Vector2D(200.0, 300.0)*scale, 0.45 );
	BodyPart *rightArm = new BodyPart( &texture[textureIndex["rightArm"]], vector<pair<Vector2D,BodyPart*>>(), Vector2D(68.0, 67.0)*scale, 0.4 );
	BodyPart *rightHand = new BodyPart( &texture[textureIndex["rightHand"]], vector<pair<Vector2D,BodyPart*>>(), Vector2D(300.0, 100.0)*scale, 0.6 );
	BodyPart *upperBody = new BodyPart( &texture[textureIndex["upperBody"]], vector<pair<Vector2D,BodyPart*>>(), Vector2D(200.0, 300.0)*scale, 0.5 );
	
	upperBody->connectedParts.push_back( make_pair( (Vector2D(72, 199) - Vector2D(307, 231))*scale, leftLeg ) );
	upperBody->connectedParts.push_back( make_pair( (Vector2D(120, 199) - Vector2D(307, 231))*scale, rightLeg ) );
	upperBody->connectedParts.push_back( make_pair( (Vector2D(65, 115) - Vector2D(119, 111))*scale, leftArm ) );
	upperBody->connectedParts.push_back( make_pair( (Vector2D(136, 105) - Vector2D(14, 14))*scale, rightArm ) );
	upperBody->connectedParts.push_back( make_pair( (Vector2D(179, 159) - Vector2D(245, 45))*scale, rightHand ) );
	
	bodyParts["leftLeg"] = leftLeg;
	bodyParts["rightLeg"] = rightLeg;
	bodyParts["leftArm"] = leftArm;
	bodyParts["rightArm"] = rightArm;
	bodyParts["rightHand"] = rightHand;
	bodyParts["upperBody"] = upperBody;
	
	leftLeg->generateList();
	rightLeg->generateList();
	leftArm->generateList();
	rightArm->generateList();
	rightHand->generateList();
	upperBody->generateList();
}
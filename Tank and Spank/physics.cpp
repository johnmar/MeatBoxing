#include "physics.h"
#include "solid.h"
#include "controllable.h"
#include <cstring>
#include <cmath>
#include "vector2d.h"
#include "world.h"
#include "projectile.h"

using std::cout;
using std::endl;

const float Physics::MAX_DELTA_TIME = 0.01f;
const float Physics::MAX_SPEED = 500.0f;

struct ImpactInfo
{
	bool cornerNW, cornerNE, cornerSW, cornerSE; // which of the corners have collided, used to calculate how to handle the collision

	ImpactInfo() : cornerNW(false), cornerNE(false), cornerSW(false), cornerSE(false) {}
};

Physics::Physics()
{
	 lastClock = 0;
	 gravity = 1000;
}

void Physics::limitVelocity( Vector2D *velocity)
{
	if( velocity->length() > MAX_SPEED )
		*velocity = *velocity * 500 / velocity->length();
}

void Physics::calculateNewPositions(vector<Drawable*> *drawables)
{
	if (lastClock == 0)
		lastClock = clock();

	clock_t tempClock = clock();
	deltaTime = (float)(tempClock - lastClock) / CLOCKS_PER_SEC;
	lastClock = tempClock;

	Solid *solid;
	ImpactInfo info;
	for (int i = drawables->size()-1; i >= 0; --i)
	{
		if( drawables->at(i)->scheduledForRemoval )
		{
			delete drawables->at( i );
			drawables->erase( drawables->begin()+i );
			continue;
		}
		if( strcmp( drawables->at(i)->drawableType(), "Solid" ) == 0 )
		{
			solid = (Solid*) drawables->at(i);

			if (deltaTime > MAX_DELTA_TIME)
				deltaTime = MAX_DELTA_TIME;

			solid->velocity.y += gravity * deltaTime;

			if( strcmp( solid->solidType(), "Controllable" ) == 0 )
			{
				((Controllable*)solid)->applyControls( deltaTime );
				Drawable *creation = ((Controllable*)solid)->getCreation();
				if( creation != NULL )
					drawables->push_back( creation );
			}

			limitVelocity(&solid->velocity);

			solid->position.x += solid->velocity.x * deltaTime;
			solid->position.y += solid->velocity.y * deltaTime;

			for (unsigned int j = 0; j < drawables->size(); ++j)
			{
				if( strcmp( drawables->at(j)->drawableType(), "World" ) == 0 )
				{
					info = checkForCollision( solid, (World*)drawables->at(j) );
					if( strcmp( solid->solidType(), "Projectile" ) == 0 && ( info.cornerNW || info.cornerNE || info.cornerSW || info.cornerSE ) )
						solid->scheduledForRemoval = true;
					else
						handleCollision( solid, info, (World*)drawables->at(j) );
				}
				else if( strcmp( drawables->at(j)->drawableType(), "Solid" ) == 0 )
				{
					info = checkForCollision( solid, (Solid*)drawables->at(j) );
					handleCollision( solid, info, (Solid*)drawables->at(j) );
				}
			}
		}
	}
}

double Physics::timeOfImpact( Vector2D *lineNormal, const Vector2D *linePoint, const Solid *solid, const Vector2D *cornerPoint )
{
	Vector2D point(linePoint->x - cornerPoint->x, linePoint->y - cornerPoint->y);
	return lineNormal->dotProduct(&point) / lineNormal->dotProduct(&solid->velocity);
}

ImpactInfo Physics::checkForCollision( const Solid *solid, const World *world )
{
	ImpactInfo info;
	vector<Block> blocks = world->getBlocks();
	
	double blockLeft, blockRight, blockTop, blockBottom;
	double solidLeft, solidRight, solidTop, solidBottom;
	for( unsigned int i = 0; i < blocks.size(); ++i )
	{
		blockLeft = blocks[i].x * World::gridSize;
		blockTop = blocks[i].y * World::gridSize;
		blockRight = blocks[i].x * World::gridSize + 2*World::gridSize;
		blockBottom = blocks[i].y * World::gridSize + 2*World::gridSize;
		
		solidLeft = solid->position.x;
		solidTop = solid->position.y;
		solidRight = solid->position.x + solid->hitboxSize.x;
		solidBottom = solid->position.y + solid->hitboxSize.y;

		if( blockRight > solidLeft && blockLeft < solidRight )
		{
			if( blockBottom > solidTop && blockTop < solidBottom )
			{
				if( blockLeft < solidLeft )
				{
					if( blockTop < solidTop )
						info.cornerNW = true;
					else if( blockBottom > solidBottom )
						info.cornerSW = true;
					else
					{
						info.cornerSW = true;
						info.cornerNW = true;
					}
				}
				else if( blockRight > solidRight )
				{
					if( blockTop < solidTop )
						info.cornerNE = true;
					else if( blockBottom > solidBottom )
						info.cornerSE = true;
					else
					{
						info.cornerSE = true;
						info.cornerNE = true;
					}
				}
				else if( blockTop < solidTop )
				{
					info.cornerNE = true;
					info.cornerNW = true;
				}
				else if( blockBottom > solidBottom )
				{
					info.cornerSE = true;
					info.cornerSW = true;
				}
				else
				{
					info.cornerSE = true;
					info.cornerNE = true;
					info.cornerSW = true;
					info.cornerNW = true;
				}
			}
		}
	}
	return info;
}

ImpactInfo Physics::checkForCollision( const Solid *solid, const Solid *solid2 )
{
	ImpactInfo info;
	
	double s1Left, s1Right, s1Top, s1Bottom;
	double s2Left, s2Right, s2Top, s2Bottom;

	s1Left = solid->position.x;
	s1Top = solid->position.y;
	s1Right = solid->position.x + solid->hitboxSize.x;
	s1Bottom = solid->position.y + solid->hitboxSize.y;
		
	s2Left = solid2->position.x;
	s2Top = solid2->position.y;
	s2Right = solid2->position.x + solid2->hitboxSize.x;
	s2Bottom = solid2->position.y + solid2->hitboxSize.y;

	/*if( blockRight > solidLeft && blockLeft < solidRight )
	{
		if( blockBottom > solidTop && blockTop < solidBottom )
		{
			if( blockLeft < solidLeft )
			{
				if( blockTop < solidTop )
					info.cornerNW = true;
				else if( blockBottom > solidBottom )
					info.cornerSW = true;
				else
				{
					info.cornerSW = true;
					info.cornerNW = true;
				}
			}
			else if( blockRight > solidRight )
			{
				if( blockTop < solidTop )
					info.cornerNE = true;
				else if( blockBottom > solidBottom )
					info.cornerSE = true;
				else
				{
					info.cornerSE = true;
					info.cornerNE = true;
				}
			}
			else if( blockTop < solidTop )
			{
				info.cornerNE = true;
				info.cornerNW = true;
			}
			else if( blockBottom > solidBottom )
			{
				info.cornerSE = true;
				info.cornerSW = true;
			}
			else
			{
				info.cornerSE = true;
				info.cornerNE = true;
				info.cornerSW = true;
				info.cornerNW = true;
			}
		}
	}*/
	return info;
}

void Physics::handleCollision( Solid *solid, ImpactInfo info, World* world )
{
	double solidLeft = solid->position.x;
	double solidTop = solid->position.y;
	double solidRight = solid->position.x + solid->hitboxSize.x;
	double solidBottom = solid->position.y + solid->hitboxSize.y;

	if( hasCrashedBottom( solid, info, world ) )
	{
		if( solid->velocity.y > 0.0 )
			solid->velocity.y = 0.0;
		solid->position.y = (double)(floor(solidBottom/25)*25.0) - solid->hitboxSize.y;
	}
	if( hasCrashedTop( solid, info, world ) )
	{
		if( solid->velocity.y < 0.0 )
			solid->velocity.y = 0.0;
		solid->position.y = (double)(floor(solidTop/25)*25+25.0);
	}
	if( hasCrashedLeft( solid, info, world ) )
	{
		if( solid->velocity.x < 0.0 )
			solid->velocity.x = 0.0;
		solid->position.x = (double)(floor(solidLeft/25)*25+25.0);
	}
	if( hasCrashedRight( solid, info, world ) )
	{
		if( solid->velocity.x > 0.0 )
			solid->velocity.x = 0.0;
		solid->position.x = (double)(floor(solidRight/25)*25.0) - solid->hitboxSize.x;
	}
}

void Physics::handleCollision( Solid *solid, ImpactInfo info, Solid* solid2 )
{
}

bool Physics::hasCrashedBottom( Solid *solid, ImpactInfo info, World* world )
{
	if( info.cornerSE && info.cornerSW )
		return true;
	if( info.cornerSE )
	{
		// Test the South East corner
		int SEcoordX = (int)floor( ( solid->position.x + solid->hitboxSize.x )/World::gridSize );
		int SEcoordY = (int)floor( ( solid->position.y + solid->hitboxSize.y )/World::gridSize );
		if( world->isSolid( SEcoordX, SEcoordY ) )
		{
			if( !world->isSolid( SEcoordX-1, SEcoordY-1 ) && world->isSolid( SEcoordX-1 , SEcoordY ) )
				return true;
			if( !world->isSolid( SEcoordX, SEcoordY-1 ) )
				return true;
		}
	}
	if( info.cornerSW )
	{
		// Test the South West corner
		int SWcoordX = (int)floor( ( solid->position.x )/World::gridSize );
		int SWcoordY = (int)floor( ( solid->position.y + solid->hitboxSize.y )/World::gridSize );
		if( world->isSolid( SWcoordX, SWcoordY ) )
		{
			if( !world->isSolid( SWcoordX+1, SWcoordY-1 ) && world->isSolid( SWcoordX+1 , SWcoordY ) )
				return true;
			if( !world->isSolid( SWcoordX, SWcoordY-1 ) )
				return true;
		}
	}
	return false;
}

bool Physics::hasCrashedTop( Solid *solid, ImpactInfo info, World* world )
{
	if( info.cornerNE && info.cornerNW )
		return true;
	if( info.cornerNE )
	{
		// Test the North East corner
		int NEcoordX = (int)floor( ( solid->position.x + solid->hitboxSize.x )/World::gridSize );
		int NEcoordY = (int)floor( ( solid->position.y )/World::gridSize );
		if( world->isSolid( NEcoordX, NEcoordY ) )
		{
			if( !world->isSolid( NEcoordX-1, NEcoordY+1 ) && world->isSolid( NEcoordX-1 , NEcoordY ) )
				return true;
			if( !world->isSolid( NEcoordX, NEcoordY+1 ) )
				return true;
		}
	}
	if( info.cornerNW )
	{
		// Test the North West corner
		int NWcoordX = (int)floor( ( solid->position.x )/World::gridSize );
		int NWcoordY = (int)floor( ( solid->position.y )/World::gridSize );
		if( world->isSolid( NWcoordX, NWcoordY ) )
		{
			if( !world->isSolid( NWcoordX+1, NWcoordY+1 ) && world->isSolid( NWcoordX+1 , NWcoordY ) )
				return true;
			if( !world->isSolid( NWcoordX, NWcoordY+1 ) )
				return true;
		}
	}
	return false;
}

bool Physics::hasCrashedLeft( Solid *solid, ImpactInfo info, World* world )
{
	if( info.cornerNW && info.cornerSW )
		return true;
	if( info.cornerNW )
	{
		// Test the North West corner
		int NWcoordX = (int)floor( ( solid->position.x )/World::gridSize );
		int NWcoordY = (int)floor( ( solid->position.y )/World::gridSize );
		if( world->isSolid( NWcoordX, NWcoordY ) )
		{
			if( world->isSolid( NWcoordX, NWcoordY+1 ) )
				return true;
		}
	}
	if( info.cornerSW )
	{
		// Test the South West corner
		int SWcoordX = (int)floor( ( solid->position.x )/World::gridSize );
		int SWcoordY = (int)floor( ( solid->position.y + solid->hitboxSize.y )/World::gridSize );
		if( world->isSolid( SWcoordX, SWcoordY ) )
		{
			if( world->isSolid( SWcoordX, SWcoordY-1 ) )
				return true;
		}
	}
	return false;
}

bool Physics::hasCrashedRight( Solid *solid, ImpactInfo info, World* world )
{
	if( info.cornerSE && info.cornerNE )
		return true;
	if( info.cornerSE )
	{
		// Test the South East corner
		int SEcoordX = (int)floor( ( solid->position.x + solid->hitboxSize.x )/World::gridSize );
		int SEcoordY = (int)floor( ( solid->position.y + solid->hitboxSize.y )/World::gridSize );
		if( world->isSolid( SEcoordX, SEcoordY ) )
		{
			if( world->isSolid( SEcoordX, SEcoordY-1 ) )
				return true;
		}
	}
	if( info.cornerNE )
	{
		// Test the North East corner
		int NEcoordX = (int)floor( ( solid->position.x + solid->hitboxSize.x )/World::gridSize );
		int NEcoordY = (int)floor( ( solid->position.y )/World::gridSize );
		if( world->isSolid( NEcoordX, NEcoordY ) )
		{
			if( world->isSolid( NEcoordX, NEcoordY+1 ) )
				return true;
		}
	}
	return false;
}
#ifndef PHYSICS_H
#define PHYSICS_H

#include <vector>
#include <ctime>

using std::vector;

class Drawable;
class World;
class Solid;
class Vector2D;
struct ImpactInfo;
struct Line;

class Physics
{
public:
	void Physics::calculateNewPositions(vector<Drawable*> *drawables);

	Physics();
private:
	static const float MAX_DELTA_TIME;
	static const float MAX_SPEED;

	clock_t lastClock;
	float deltaTime;
	float gravity;
	
	double timeOfImpact( Vector2D *lineNormal, const Vector2D *linePoint, const Solid *solid, const Vector2D *cornerPoint );
	ImpactInfo checkForCollision( const Solid *solid, const World *world );
	void handleCollision( Solid *solid, ImpactInfo info, World* world );
	ImpactInfo checkForCollision( const Solid *solid, const Solid *solid2 );
	void handleCollision( Solid *solid, ImpactInfo info, Solid* solid2 );
	void limitVelocity(Vector2D *velocity);
	bool hasCrashedBottom( Solid *solid, ImpactInfo info, World* world );
	bool hasCrashedTop( Solid *solid, ImpactInfo info, World* world );
	bool hasCrashedLeft( Solid *solid, ImpactInfo info, World* world );
	bool hasCrashedRight( Solid *solid, ImpactInfo info, World* world );
};

#endif // PHYSICS_H
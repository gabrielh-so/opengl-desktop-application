#pragma once
#include "box2d/box2d.h"
#include <vector>

struct PositionAngle {
	float X, Y;
	float Angle;
};

class Game {

public:
	Game();

	void Initialise();

	void Update(float deltaTime);

	std::vector<PositionAngle> GetPositionAngles();

private:

	// objects for creating the world
	b2World* world;
	b2Vec2 gravity;

	// values for simulation steps
	int32 velocityIterations = 6;
	int32 positionIterations = 2;

	// stage 1: Define a body with position, damping, etc.

	std::vector<b2BodyDef> bodyDefs;


	// stage 2: Use the world object to create the body.

	std::vector<b2PolygonShape> dynamicPolygons;
	std::vector<b2CircleShape> dynamicCircles;


	// stage 3: Define fixtures with a shape, friction, density, etc.

	std::vector<b2Body*> bodies;


	// stage 4: Create fixtures on the body.

	std::vector<b2FixtureDef> fixtureDefs;

	

};
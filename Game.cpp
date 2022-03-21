#include "Include/Game.h"

Game::Game()
	: gravity(b2Vec2(0.0f, -10.0f))
{
	bool gw = true;
}

void Game::Initialise() {


	// initialise the world

	gravity = b2Vec2(0.0f, -10.0f);
	world = new b2World(gravity);


	// create ground body

	b2BodyDef groundBodyDef = b2BodyDef();
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.fixedRotation = true;
	groundBodyDef.position.Set(0.0f, -1.0f);

	b2Body* groundBody = world->CreateBody(&groundBodyDef);

	b2PolygonShape groundBox = b2PolygonShape();

	groundBox.SetAsBox(0.5f, 0.5f);

	// creates a fixture, but this body is static so we don't need to assign anything
	groundBody->CreateFixture(&groundBox, 0.0f);


	bodyDefs.push_back(groundBodyDef);
	bodies.push_back(groundBody);
	dynamicPolygons.push_back(groundBox);


	// create box body

	for (int i = 0; i < 5; i++) {
		b2BodyDef boxBodyDef = b2BodyDef();

		boxBodyDef.type = b2_dynamicBody;

		b2Body* boxBody;

		boxBodyDef.position.Set(0.0f, 3.0f + i * 3.0f);
		boxBody = world->CreateBody(&boxBodyDef);

		b2PolygonShape dynamicBoxShape;
		dynamicBoxShape.SetAsBox(0.5f, 0.5f);

		b2FixtureDef dynamicBoxFixture;

		dynamicBoxFixture.shape = &dynamicBoxShape;
		dynamicBoxFixture.density = 1.0f;
		dynamicBoxFixture.friction = 0.3f;

		boxBody->CreateFixture(&dynamicBoxFixture);

		//boxBody->ApplyAngularImpulse(-1.0f, false);

		bodyDefs.push_back(boxBodyDef);
		bodies.push_back(boxBody);
		dynamicPolygons.push_back(dynamicBoxShape);
		fixtureDefs.push_back(dynamicBoxFixture);
	}

	b2BodyDef boxBodyDef = b2BodyDef();

	boxBodyDef.type = b2_dynamicBody;

	b2Body* boxBody;

	boxBodyDef.position.Set(-3.0f, 5.0f);
	boxBody = world->CreateBody(&boxBodyDef);

	b2PolygonShape dynamicBoxShape;
	dynamicBoxShape.SetAsBox(0.5f, 0.5f);

	b2FixtureDef dynamicBoxFixture;

	dynamicBoxFixture.shape = &dynamicBoxShape;
	dynamicBoxFixture.density = 1.0f;
	dynamicBoxFixture.friction = 0.3f;

	boxBody->CreateFixture(&dynamicBoxFixture);

	//boxBody->ApplyAngularImpulse(-1.0f, false);

	boxBody->ApplyForceToCenter(b2Vec2(5.0f, 5.0f), true);

	bodyDefs.push_back(boxBodyDef);
	bodies.push_back(boxBody);
	dynamicPolygons.push_back(dynamicBoxShape);
	fixtureDefs.push_back(dynamicBoxFixture);


}

void Game::Update(float deltaTime) {

	world->Step(deltaTime, velocityIterations, positionIterations);

}

std::vector<PositionAngle> Game::GetPositionAngles() {

	std::vector<PositionAngle> PositionAngles;
	for (b2Body* b : bodies) {

		b2Vec2 pos = b->GetPosition();
		PositionAngles.push_back( { pos.x, pos.y, b->GetAngle() } );

	}

	return PositionAngles;
}
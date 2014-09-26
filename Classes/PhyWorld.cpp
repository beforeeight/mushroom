/*
 * PhyWorld.cpp
 *
 *  Created on: Jun 25, 2014
 *      Author: zhangchen
 */

#include "PhyWorld.h"

#include <Box2D/Box2D.h>

static b2World *world = NULL;

PhyWorld::PhyWorld() {

}

PhyWorld::~PhyWorld() {
	// TODO Auto-generated destructor stub
}

b2World* PhyWorld::shareWorld(void) {
	if (!world) {
		b2Vec2 gravity(0.0f, -9.8f);
		//b2Vec2 *gravity = (b2Vec2*) malloc(sizeof(b2Vec2));
		//gravity->Set(0.0f, -10.0f);
		world = new b2World(gravity);
		world->SetAllowSleeping(true);
		world->SetContinuousPhysics(true);
		// Prepare for simulation. Typically we use a time step of 1/60 of a
		// second (60Hz) and 10 iterations. This provides a high quality simulation
		// in most game scenarios.
	}
	return world;
}


/*
 * PhyWorld.cpp
 *
 *  Created on: Jun 25, 2014
 *      Author: zhangchen
 */

#include "PhyWorld.h"
#include "MushroomContactListener.h"
#include <Box2D/Box2D.h>
#include "GLES-Render.h"

static b2World *world = NULL;
static GLESDebugDraw *_debugDraw = NULL;

//static GLESDebugDraw *_debugDraw = NULL;
PhyWorld::PhyWorld() {

}

PhyWorld::~PhyWorld() {
	// TODO Auto-generated destructor stub
}

b2World* PhyWorld::shareWorld(void) {
	if (!world) {
		b2Vec2 gravity(0.0f, -WORLD_GRAVITY);
		//b2Vec2 *gravity = (b2Vec2*) malloc(sizeof(b2Vec2));
		//gravity->Set(0.0f, -10.0f);
		world = new b2World(gravity);
		world->SetAllowSleeping(true);
		world->SetContactListener(new MushroomContactListener());

		world->SetContinuousPhysics(true);

//		b2DebugDraw
//		_debugDraw = new GLESDebugDraw(32.0f);
//		uint32 flags = b2Draw::e_shapeBit | b2Draw::e_aabbBit
//				| b2Draw::e_jointBit;
//		_debugDraw->SetFlags(flags);
//		world->SetDebugDraw(_debugDraw);
	}
	return world;
}

void PhyWorld::purgeB2World() {
	if (world) {
		delete (world);
		world = NULL;
	}
	if (_debugDraw) {
		delete (_debugDraw);
		_debugDraw = NULL;
	}
}

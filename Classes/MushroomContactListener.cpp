/*
 * MushroomContactListener.cpp
 *
 *  Created on: Oct 11, 2014
 *      Author: zhangchen
 */

#include "cocos2d.h"
#include "MushroomContactListener.h"

USING_NS_CC;

MushroomContactListener::MushroomContactListener() {
	// TODO Auto-generated constructor stub

}

MushroomContactListener::~MushroomContactListener() {
	// TODO Auto-generated destructor stub

}

Mushroom* MushroomContactListener::findMushroom(b2Contact*& contact) {
	Mushroom* mushroom = 0;
	PhySprite* phyA =
			(PhySprite*) (contact->GetFixtureA()->GetBody()->GetUserData());
	PhySprite* phyB =
			(PhySprite*) (contact->GetFixtureB()->GetBody()->GetUserData());
	if (phyA && phyA->getPhyType() == MUSHROOM) {
		mushroom = ((Mushroom*) (phyA));
	} else if (phyB && phyB->getPhyType() == MUSHROOM) {
		mushroom = ((Mushroom*) (phyB));
	}
	return mushroom;
}

Bricks* MushroomContactListener::findBricks(b2Contact*& contact) {
	Bricks* bricks = 0;
	PhySprite* phyA =
			(PhySprite*) (contact->GetFixtureA()->GetBody()->GetUserData());
	PhySprite* phyB =
			(PhySprite*) (contact->GetFixtureB()->GetBody()->GetUserData());
	if (phyA && phyA->getPhyType() == BRICK) {
		bricks = ((Bricks*) (phyA));
	} else if (phyB && phyB->getPhyType() == BRICK) {
		bricks = ((Bricks*) (phyB));
	}
	return bricks;
}

void MushroomContactListener::BeginContact(b2Contact* contact) {
	PhySprite* phyA =
			(PhySprite*) (contact->GetFixtureA()->GetBody()->GetUserData());
	PhySprite* phyB =
			(PhySprite*) (contact->GetFixtureB()->GetBody()->GetUserData());
	if (phyA && phyB) {
		phyA->beginContact(phyB, contact);
		phyB->beginContact(phyA, contact);
	}
}

void MushroomContactListener::PreSolve(b2Contact* contact,
		const b2Manifold* oldManifold) {
	PhySprite* phyA =
			(PhySprite*) (contact->GetFixtureA()->GetBody()->GetUserData());
	PhySprite* phyB =
			(PhySprite*) (contact->GetFixtureB()->GetBody()->GetUserData());
	if (phyA && phyB) {
		phyA->PreSolve(phyB, contact, oldManifold);
		phyB->PreSolve(phyA, contact, oldManifold);
	}
}

void MushroomContactListener::EndContact(b2Contact* contact) {
	PhySprite* phyA =
			(PhySprite*) (contact->GetFixtureA()->GetBody()->GetUserData());
	PhySprite* phyB =
			(PhySprite*) (contact->GetFixtureB()->GetBody()->GetUserData());
	if (phyA && phyB) {
		phyA->endContact(phyB, contact);
		phyB->endContact(phyA, contact);
	}
}

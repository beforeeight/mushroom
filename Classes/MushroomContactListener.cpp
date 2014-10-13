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
//	Mushroom *mushroom = findMushroom(contact);
//	Bricks *bricks = findBricks(contact);
//	//CCLog("bbb  %ld,%ld", (long) mushroom, (long) bricks);
//	if (mushroom && bricks) {
//		float bricksTop = bricks->getPositionY()
//				+ bricks->getContentSize().height / 2;
//		float mushBottom = mushroom->getPositionY()
//				- mushroom->getContentSize().height / 2;
//		if (mushBottom < bricksTop) {
//			//contact->SetEnabled(false);
//		}
//	}
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

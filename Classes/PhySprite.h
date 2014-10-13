/*
 * PhySprite.h
 *
 *  Created on: Sep 29, 2014
 *      Author: zhangchen
 */

#ifndef PHYSPRITE_H_
#define PHYSPRITE_H_

#include "cocos2d.h"
#include "PhyWorld.h"

#define POS_JUNCTION 0

enum PHY_TYPE {
	MUSHROOM, BRICK
};

class PhySprite: public cocos2d::CCSprite {
public:
	PhySprite();

	virtual ~PhySprite();

	virtual void update(float delta);

	static void initPhySprite(PhySprite &sprite);

	virtual PHY_TYPE getPhyType() = 0;

	b2Body* getB2Body();

	virtual void beginContact(PhySprite *other, b2Contact* contact) {
	}

	virtual void endContact(PhySprite *other, b2Contact* contact) {
	}

protected:
	b2Body *b2body;

	virtual void createPhyBody() = 0;

	virtual void initPhyBody();
};

#endif /* PHYSPRITE_H_ */

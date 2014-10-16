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
	MUSHROOM, BRICK, WATER_EDGE, BROADSIDE
};

class PhySprite: public cocos2d::CCSprite {

	PHY_TYPE phyType;

public:
	PhySprite();

	virtual ~PhySprite();

	virtual bool initWithBody(b2Body *body);

	static PhySprite* create(b2Body *body);

	static void initPhySprite(PhySprite &sprite);

	static void initPhySprite(PhySprite &sprite, bool update);

	virtual void update(float delta);

	virtual PHY_TYPE getPhyType() {
		return phyType;
	}

	virtual void setPhyType(PHY_TYPE type) {
		phyType = type;
	}

	virtual b2Body* getB2Body();

	virtual void beginContact(PhySprite *other, b2Contact* contact) {
	}

	virtual void endContact(PhySprite *other, b2Contact* contact) {
	}

protected:
	b2Body *b2PhyBody;

	virtual void createPhyBody() {
	}

	virtual void initPhyBody();
};

#endif /* PHYSPRITE_H_ */

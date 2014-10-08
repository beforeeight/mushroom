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

class PhySprite: public cocos2d::CCSprite {
public:
	PhySprite();

	virtual ~PhySprite();

	virtual void update(float delta);

	static void initPhySprite(PhySprite &sprite);

protected:
	b2Body *b2body;

	virtual void createPhyBody() = 0;

	virtual void initPhyBody();
};

#endif /* PHYSPRITE_H_ */

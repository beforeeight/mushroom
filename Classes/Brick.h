/*
 * Brick.h
 *
 *  Created on: Sep 25, 2014
 *      Author: zhangchen
 */

#ifndef BRICK_H_
#define BRICK_H_

#include "cocos2d.h"
#include "PhyWorld.h"

#define TEXTURE_BRICK "item_cubic.png"

class Brick: public cocos2d::CCSprite {
public:
	Brick();

	virtual ~Brick();

	virtual bool init();

	CREATE_FUNC(Brick)

private:
	b2Body *b2body;

	void createPhyBody();
};

#endif /* BRICK_H_ */

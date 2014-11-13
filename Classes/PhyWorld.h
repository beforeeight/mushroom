/*
 * PhyWorld.h
 *
 *  Created on: Jun 25, 2014
 *      Author: zhangchen
 */

#ifndef PHYWORLD_H_
#define PHYWORLD_H_
#include <Box2D/Box2D.h>
#include "Resources.h"

#define VELOCITY_ITERATIONS 6 //不要改
#define POSITION_ITERATIONS 2 //不要改
#define WORLD_GRAVITY 25 //世界的重力加速度，地球上是9.8

class PhyWorld {
public:
	PhyWorld();

	~PhyWorld();

	static b2World* shareWorld(void);

	static void purgeB2World();
};

#define b2c(__x__) __x__*32
#define c2b(__x__) __x__/32

#endif /* PHYWORLD_H_ */

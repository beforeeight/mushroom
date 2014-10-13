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


class PhyWorld {
public:
	PhyWorld();

	~PhyWorld();

	static b2World* shareWorld(void);
};

#define p2c(__x__) __x__*32
#define c2p(__x__) __x__/32

#endif /* PHYWORLD_H_ */

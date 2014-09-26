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

#define PHYSICAL_WIDTH 5

class PhyWorld {
public:
	PhyWorld();

	~PhyWorld();

	static b2World* shareWorld(void);
};

#define p2c(__x__) __x__*(LOCAL_RESOLUTION_WIDTH/PHYSICAL_WIDTH)
#define c2p(__x__) __x__/(LOCAL_RESOLUTION_WIDTH/PHYSICAL_WIDTH)

#endif /* PHYWORLD_H_ */
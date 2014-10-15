/*
 * WaterEdge.cpp
 *
 *  Created on: Oct 14, 2014
 *      Author: zhangchen
 */

#include "WaterEdge.h"

WaterEdge::WaterEdge() {
	// TODO Auto-generated constructor stub

}

WaterEdge::~WaterEdge() {
	// TODO Auto-generated destructor stub
}

bool WaterEdge::init() {
	if (PhySprite::init()) {
		return true;
	} else {
		return false;
	}
}

PHY_TYPE WaterEdge::getPhyType() {
	return WATER_EDGE;
}

void WaterEdge::beginContact(PhySprite *other, b2Contact* contact) {

	b2Body *otherBody = other->getB2Body();
	b2Fixture * fixture = otherBody->GetFixtureList();
	float density = fixture->GetDensity();
	float speedY = otherBody->GetLinearVelocity().y;
	float mass = otherBody->GetMass();
	const b2Vec2 impulse(0.0f, -speedY * mass / 2);
	//otherBody->SetFixedRotation(false);
	otherBody->ApplyLinearImpulse(impulse, otherBody->GetLocalCenter());
	otherBody->SetGravityScale((density - 1) / density);
}

void WaterEdge::createPhyBody() {
	CCPoint lp = ccpp(-0.5,0);
	CCPoint rp = ccpp(0.5,0);
	b2BodyDef b2BodyDef;
	b2BodyDef.position.Set(c2p(this->getPositionX()),
	c2p(this->getPositionY()));
	b2BodyDef.type = b2_staticBody;
	this->b2PhyBody = PhyWorld::shareWorld()->CreateBody(&b2BodyDef);
	b2EdgeShape shape;
	shape.Set(b2Vec2(c2p(lp.x), c2p(lp.y)), b2Vec2(c2p(rp.x), c2p(rp.y)));
	b2Fixture* fixture = this->b2PhyBody->CreateFixture(&shape, 0.0f);
	fixture->SetSensor(true);
	b2PhyBody->SetUserData(this);
}

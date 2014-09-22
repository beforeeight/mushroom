/*
 * BgLayer.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: zhangchen
 */

#include "BgLayer.h"
#include "Resources.h"

USING_NS_CC;

BgLayer::BgLayer() {

}

BgLayer::~BgLayer() {
}

bool BgLayer::init() {
	if (CCLayer::init()) {
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		this->ignoreAnchorPointForPosition(true);
		setAnchorPoint(ccp(0.5f, 0.5f));
		this->setContentSize(s);
		setPosition(ccp(s.width / 2, s.height / 2));
//		float width = s.width / 2;
//		float height = s.height / 2;
		/* initialize the counter */
		CCTextureCache *cache = CCTextureCache::sharedTextureCache();
		/*-- 背景 --*/
		CCSprite *bg = CCSprite::create("background.png");
		this->addChild(bg);

		this->cloudBigAnimation();
		this->cloudSmallAnimation();
		createBigCloud();
		return true;
	} else {
		return false;
	}
}

void BgLayer::cloudBigAnimation() {
	this->runAction(
			CCSequence::create(
					CCCallFunc::create(this,
							callfunc_selector(BgLayer::createBigCloud)),
					CCDelayTime::create(CCRANDOM_0_1() * 5 + 2),
					CCCallFunc::create(this,
							callfunc_selector(BgLayer::cloudBigAnimation)),
					NULL));
}

void BgLayer::cloudSmallAnimation() {
	this->runAction(
			CCSequence::create(
					CCCallFunc::create(this,
							callfunc_selector(BgLayer::createSmallCloud)),
					CCDelayTime::create(CCRANDOM_0_1() * 5 + 2),
					CCCallFunc::create(this,
							callfunc_selector(BgLayer::cloudSmallAnimation)),
					NULL));
}

void BgLayer::createBigCloud() {
	CCSprite* cloud = CCSprite::create("bg_cloud.png");
	cloud->setAnchorPoint(ccp(0, 1));
	cloud->setPosition(ccpp(0.5, 0.4)+ccp(cloud->getContentSize().width, 0.4));
	this->addChild(cloud);
	cloud->runAction(CCSequence::create(CCMoveBy::create(8.0f, ccpp(-1, 0)-ccp(cloud->getContentSize().width*2,0)),CCCallFunc::create(cloud,callfunc_selector(CCSprite::removeFromParent)), NULL));
}

void BgLayer::createSmallCloud() {
	CCSprite* cloud = CCSprite::create("bg_cloud_small.png");
	cloud->setAnchorPoint(ccp(0, 1));
	cloud->setPosition(ccpp(0.5, 0.33)+ccp(cloud->getContentSize().width, 0.4));
	this->addChild(cloud);
	cloud->runAction(CCSequence::create(CCMoveBy::create(10.0f, ccpp(-1, 0)-ccp(cloud->getContentSize().width*2,0)),CCCallFunc::create(cloud,callfunc_selector(CCSprite::removeFromParent)), NULL));
}


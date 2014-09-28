/*
 * GameScene.cpp
 *
 *  Created on: Sep 16, 2014
 *      Author: zhangchen
 */

#include "GameScene.h"
#include "BgLayer.h"
#include "EffectUtils.h"
#include "Menu.h"
#include "FinishScene.h"

#define TAG_LAYER_GAME 0
#define TAG_LAYER_PAUSE 1
#define TAG_SCORE 3

GameLayer::GameLayer() :
		running(true) {
	batchBrick = BatchBrick::create();
	if (batchBrick) {
		this->addChild(batchBrick);
	}
}

GameLayer::~GameLayer() {
}

bool GameLayer::init() {
	if (CCLayer::init()) {
		LOCAL_CONTEXT->clearScore();

		this->setTouchMode(kCCTouchesOneByOne);
		this->setTouchEnabled(true);
		this->scheduleUpdate();
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		this->ignoreAnchorPointForPosition(true);
		setAnchorPoint(ccp(0.5f, 0.5f));
		this->setContentSize(s);
		setPosition(ccp(s.width / 2, s.height / 2));

		/*-- 计分 --*/
		CCLabelTTF *score = CCLabelTTF::create(CCString::createWithFormat("%d", LOCAL_CONTEXT->getScore())->getCString(),LOCAL_RESOURCES->valueByKey("font")->getCString(),LOCAL_RESOURCES->valueByKey("font_size")->floatValue());
		score->setColor(LOCAL_CONTEXT->getFontColor());
		score->setAnchorPoint(ccp(1, 1));
		score->setPosition(ccpp(0.5,0.5));
		this->addChild(score,0,TAG_SCORE);

		this->batchBrick->initBricks();
		return true;

	} else {
		return false;
	}
}

CCScene * GameLayer::scene() {
	CCScene *scene = CCScene::create();
	if (scene && scene->init()) {
		if (scene) {
			/*-- 背景 --*/
			scene->addChild(BgLayer::create());
			GameLayer *layer = GameLayer::create();
			scene->addChild(layer, TAG_LAYER_GAME);
			scene->addChild(layer->createPauseButtonLayer());
		}
		return scene;
	} else {
		CC_SAFE_DELETE(scene);
		return NULL;
	}

}

bool GameLayer::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {
	//CCDirector::sharedDirector()->replaceScene(FinishLayer::scene());
	return true;
}

void GameLayer::onPauseItem(CCObject *object) {
	if (running) {
		disable();
		effect::clickButton((CCMenuItem*) object);
		createPauseLayer();
	}
}

void GameLayer::resume() {
	CCDirector::sharedDirector()->getRunningScene()->removeChildByTag(
	TAG_LAYER_PAUSE);
	enable();
}

void GameLayer::onResumeItem(CCObject *object) {
	if (!running) {
		effect::clickButton((CCMenuItem*) object, this,
				callfunc_selector(GameLayer::resume));
	}
}

void GameLayer::backMenu() {
	CCDirector::sharedDirector()->replaceScene(MenuLayer::scene());
}

void GameLayer::onBackItem(CCObject *object) {
	effect::clickButton((CCMenuItem*) object, this,
			callfunc_selector(GameLayer::backMenu));
}

CCLayer * GameLayer::createPauseButtonLayer() {
	CCMenu *layer = CCMenu::create();
	//暂停
	CCMenuItemImage *pausebg = CCMenuItemImage::create(("btn_small.png"),
			("btn_small.png"));
	CCSprite *pausetxt = CCSprite::create("btn_pause.png");
	pausebg->setTarget(this, menu_selector(GameLayer::onPauseItem));
	pausebg->setAnchorPoint(ccp(0, 1));
	pausebg->setPosition(ccpp(-0.49, 0.49));
	pausetxt->setPosition(
			ccp(pausebg->getContentSize().width / 2,
					pausebg->getContentSize().height / 2));
	pausetxt->setAnchorPoint(ccp(0.5, 0.5));
	pausebg->addChild(pausetxt);
	layer->addChild(pausebg);
	return layer;
}

void GameLayer::createPauseLayer() {
	Context *context = Context::sharedContext();
	CCSize s = CCDirector::sharedDirector()->getWinSize();
	/* 半透明层 */
	CCLayerColor* pauseLayer = CCLayerColor::create();
	pauseLayer->setContentSize(CCSizeMake(s.width, s.height));
	pauseLayer->setColor(ccc3(0, 0, 0));
	pauseLayer->setOpacity(150);
	CCMenu *menu = CCMenu::create();
//回到游戏
	CCMenuItemImage *resumebg = CCMenuItemImage::create(("btn_big.png"),
			("btn_big.png"));
	resumebg->setTarget(this, menu_selector(GameLayer::onResumeItem));
	CCMenuItemFont *resumetxt = CCMenuItemFont::create(LOCAL_RESOURCES->valueByKey("i18n_resume")->getCString());
	resumebg->setAnchorPoint(ccp(0.5, 0.5));
	resumebg->setPosition(ccpp(0, 0));
	resumetxt->setPosition(
			ccp(resumebg->getContentSize().width / 2,
					resumebg->getContentSize().height / 2));
	resumetxt->setAnchorPoint(ccp(0.5, 0.5));
	resumetxt->setColor(context->getFontColor());
	resumebg->addChild(resumetxt);
	menu->addChild(resumebg);

//回到菜单
	CCMenuItemImage *backbg = CCMenuItemImage::create(("btn_big.png"),
			("btn_big.png"));
	backbg->setTarget(this, menu_selector(GameLayer::onBackItem));
	CCMenuItemFont *backtxt = CCMenuItemFont::create(LOCAL_RESOURCES->valueByKey("i18n_back")->getCString());
	backbg->setAnchorPoint(ccp(0.5, 0.5));
	backbg->setPosition(ccpp(0, -0.12));
	backtxt->setPosition(
			ccp(backbg->getContentSize().width / 2,
					backbg->getContentSize().height / 2));
	backtxt->setAnchorPoint(ccp(0.5, 0.5));
	backtxt->setColor(context->getFontColor());
	backbg->addChild(backtxt);
	menu->addChild(backbg);

	pauseLayer->addChild(menu);
	CCDirector::sharedDirector()->getRunningScene()->addChild(pauseLayer, 0,
	TAG_LAYER_PAUSE);
}

void GameLayer::disable() {
	this->running = false;
}

void GameLayer::enable() {
	this->running = true;
}

void GameLayer::gameover() {
	LOCAL_CONTEXT->save();
	CCDirector::sharedDirector()->replaceScene(CCTransitionFadeDown::create(0.5f, FinishLayer::scene()));
}

void GameLayer::update(float delta) {

	float32 timeStep = CCDirector::sharedDirector()->getAnimationInterval();
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	b2World *world = PhyWorld::shareWorld();
	world->Step(timeStep, velocityIterations, positionIterations);
}

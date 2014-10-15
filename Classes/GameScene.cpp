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
#include "Mushroom.h"
#include "WaterEdge.h"

#define TAG_LAYER_GAME 0
#define TAG_LAYER_PAUSE 1
#define TAG_SCORE 3

#define TAG_MUSHROOM 10
#define TAG_BTN_LEFT 11
#define TAG_BTN_RIGHT 12
#define TAG_BTN_JUMP 13

GameLayer::GameLayer() :
		running(true), leftEdge(0), rightEdge(0) {
	brickEmitter = new BrickEmitter(*this);
}

GameLayer::~GameLayer() {
	CC_SAFE_DELETE(brickEmitter);
	if (leftEdge) {
		PhyWorld::shareWorld()->DestroyBody(leftEdge);
	}
	if (rightEdge) {
		PhyWorld::shareWorld()->DestroyBody(rightEdge);
	}
}

void GameLayer::draw() {
	//glDisable(GL_TEXTURE_2D);
	//glDisableClientState(GL_COLOR_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	PhyWorld::shareWorld()->DrawDebugData(); //这个一定要写
	//glEnable(GL_TEXTURE_2D);
	//glEnableClientState(GL_COLOR_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

bool GameLayer::init() {
	if (CCLayer::init()) {
		LOCAL_CONTEXT->clearScore();

		this->setTouchMode(kCCTouchesAllAtOnce);
		this->setTouchEnabled(true);
		this->scheduleUpdate();
		CCSize s = CCDirector::sharedDirector()->getWinSize();
		this->ignoreAnchorPointForPosition(true);
		setAnchorPoint(ccp(0.5f, 0.5f));
		this->setContentSize(s);
		setPosition(ccp(s.width / 2, s.height / 2));

		/*-- 计分 --*/
		CCLabelTTF *score =
				CCLabelTTF::create(
						CCString::createWithFormat("%d",
								LOCAL_CONTEXT->getScore())->getCString(),
						LOCAL_RESOURCES->valueByKey("font")->getCString(),
						LOCAL_RESOURCES->valueByKey("font_size")->floatValue());
		score->setColor(LOCAL_CONTEXT->getFontColor());
		score->setAnchorPoint(ccp(1, 1));
		score->setPosition(ccpp(0.5,0.5));
		this->addChild(score, 0, TAG_SCORE);
		LOCAL_CONTEXT->setScoreTarget(score,
				(ScoreFunc) (&GameLayer::increateScore));

		/*-- 水 --*/
		WaterEdge *water = WaterEdge::create();
		water->setAnchorPoint(ccp(0.5, 0.5));
		water->setPosition(ccpp(0,-0.32));
		PhySprite::initPhySprite(*water, false);
		this->addChild(water);

		CCPoint ltp = ccpp(-0.5,0.5);
		CCPoint lbp = ccpp(-0.5,-0.5);
		CCPoint rtp = ccpp(0.5,0.5);
		CCPoint rbp = ccpp(0.5,-0.5);
//
		b2BodyDef leftBodyDef;
		leftBodyDef.position.Set(0, 0);
		leftBodyDef.type = b2_staticBody;
		this->leftEdge = PhyWorld::shareWorld()->CreateBody(&leftBodyDef);
		b2EdgeShape leftShape;
		leftShape.Set(b2Vec2(c2p(ltp.x), c2p(ltp.y)),
				b2Vec2(c2p(lbp.x), c2p(lbp.y)));
		leftEdge->CreateFixture(&leftShape, 0.0f);
		//leftBody->SetLinearDamping(0.0f);

		b2BodyDef rightBodyDef;
		rightBodyDef.position.Set(0, 0);
		this->rightEdge = PhyWorld::shareWorld()->CreateBody(&rightBodyDef);
		b2EdgeShape rightShape;
		rightShape.Set(b2Vec2(c2p(rtp.x), c2p(rtp.y)),
				b2Vec2(c2p(rbp.x), c2p(rbp.y)));
		rightEdge->CreateFixture(&rightShape, 0.0f);
		//rightBody->SetLinearDamping(0.0f);

		Mushroom *mushroom = Mushroom::create();
		mushroom->setPosition(ccpp(-0.2,0.5));
		PhySprite::initPhySprite(*mushroom);
		this->addChild(mushroom, 10, TAG_MUSHROOM);

		this->brickEmitter->initBricks();

		/* control button */
		CCSprite *leftBtn = CCSprite::create("btn_left.png");
		leftBtn->setAnchorPoint(ccp(0.5, 0.5));
		leftBtn->setPosition(ccpp(-0.35,-0.2));
		this->addChild(leftBtn, 9, TAG_BTN_LEFT);
		CCSprite *rightBtn = CCSprite::create("btn_right.png");
		rightBtn->setAnchorPoint(ccp(0.5, 0.5));
		rightBtn->setPosition(ccpp(-0.1,-0.2));
		this->addChild(rightBtn, 9, TAG_BTN_RIGHT);
		CCSprite *jumpBtn = CCSprite::create("btn_jump.png");
		jumpBtn->setAnchorPoint(ccp(0.5, 0.5));
		jumpBtn->setPosition(ccpp(0.35,-0.2));
		this->addChild(jumpBtn, 9, TAG_BTN_JUMP);
		draw();
		return true;

	} else {
		return false;
	}
}

void GameLayer::ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent) {
	if (running) {
		Mushroom *mushroom = (Mushroom*) this->getChildByTag(TAG_MUSHROOM);
		for (CCSetIterator it = pTouches->begin(); it != pTouches->end();
				it++) {
			CCTouch* touch = (CCTouch*) *it;
			CCPoint point = touch->getLocation()
					- ccp(LOCAL_RESOLUTION.width / 2,
							LOCAL_RESOLUTION.height / 2);
			if (this->getChildByTag(TAG_BTN_RIGHT)->boundingBox().containsPoint(
					point)) {
				mushroom->setVec(vec_forward);
			} else if (this->getChildByTag(TAG_BTN_LEFT)->boundingBox().containsPoint(
					point)) {
				mushroom->setVec(vec_back);
			} else if (this->getChildByTag(TAG_BTN_JUMP)->boundingBox().containsPoint(
					point)) {
				mushroom->jump();
			}
		}
	}
}

void GameLayer::ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent) {
	if (running) {
		Mushroom *mushroom = (Mushroom*) this->getChildByTag(TAG_MUSHROOM);
		for (CCSetIterator it = pTouches->begin(); it != pTouches->end();
				it++) {
			CCTouch* touch = (CCTouch*) *it;
			CCPoint point = touch->getLocation()
					- ccp(LOCAL_RESOLUTION.width / 2,
							LOCAL_RESOLUTION.height / 2);
			CCPoint startPoint = touch->getStartLocation()
					- ccp(LOCAL_RESOLUTION.width / 2,
							LOCAL_RESOLUTION.height / 2);
			if (!this->getChildByTag(TAG_BTN_JUMP)->boundingBox().containsPoint(
					startPoint)) {
				if (this->getChildByTag(TAG_BTN_RIGHT)->boundingBox().containsPoint(
						point) && !mushroom->isVec(vec_forward)) {
					mushroom->setVec(vec_forward);
				} else if (this->getChildByTag(TAG_BTN_LEFT)->boundingBox().containsPoint(
						point) && !mushroom->isVec(vec_back)) {
					mushroom->setVec(vec_back);
				}
			}
		}
	}
}
void GameLayer::ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent) {
	if (running) {
		Mushroom *mushroom = (Mushroom*) this->getChildByTag(TAG_MUSHROOM);
		for (CCSetIterator it = pTouches->begin(); it != pTouches->end();
				it++) {
			CCTouch* touch = (CCTouch*) *it;
			CCPoint startPoint = touch->getStartLocation()
					- ccp(LOCAL_RESOLUTION.width / 2,
							LOCAL_RESOLUTION.height / 2);
			if (!this->getChildByTag(TAG_BTN_JUMP)->boundingBox().containsPoint(
					startPoint)) {
				mushroom->setVec(vec_stop);
			}
		}
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
	CCMenuItemFont *resumetxt = CCMenuItemFont::create(
			LOCAL_RESOURCES->valueByKey("i18n_resume")->getCString());
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
	CCMenuItemFont *backtxt = CCMenuItemFont::create(
			LOCAL_RESOURCES->valueByKey("i18n_back")->getCString());
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
	this->brickEmitter->pause();
}

void GameLayer::enable() {
	this->running = true;
	this->brickEmitter->resume();
}

void GameLayer::gameover() {
	LOCAL_CONTEXT->save();
	CCDirector::sharedDirector()->replaceScene(
			CCTransitionFadeDown::create(0.5f, FinishLayer::scene()));
}

void GameLayer::update(float delta) {

	float32 timeStep = CCDirector::sharedDirector()->getAnimationInterval();
	int32 velocityIterations = 6;
	int32 positionIterations = 2;
	b2World *world = PhyWorld::shareWorld();
	world->Step(timeStep, velocityIterations, positionIterations);
}

void GameLayer::increateScore(CCObject *p_sender, unsigned int score) {
	CCLabelTTF *scoreLabel = (CCLabelTTF*) p_sender;
	scoreLabel->setString(
			CCString::createWithFormat("%d", score)->getCString());
}

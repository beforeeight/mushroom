
#include "GB2ShapeCache.h"
#include "Box2D/Box2D.h"

class FixtureDef {
public:
    FixtureDef(): next(NULL) {}
    ~FixtureDef() {
        delete next;
        delete fixture.shape;
    }
    FixtureDef *next;
    b2FixtureDef fixture;
    int callbackData;
};

class BodyDef {
public:
    BodyDef()
    : fixtures(NULL) {}
    ~BodyDef() {
        if (fixtures)
            delete fixtures;
    }
    FixtureDef *fixtures;
    CCPoint anchorPoint;
};
static GB2ShapeCache *_sharedShapeCache = NULL;
GB2ShapeCache* GB2ShapeCache::sharedGB2ShapeCache(void) {
    if (!_sharedShapeCache) {
        _sharedShapeCache = new GB2ShapeCache();
        _sharedShapeCache->init();
    }
    return _sharedShapeCache; }
bool GB2ShapeCache::init() {
    return true;
}
void GB2ShapeCache::reset() {
    std::map<std::string, BodyDef *>::iterator iter;
    for(iter = shapeObjects.begin();iter != shapeObjects.end();++iter){
        delete iter->second;
    }
    shapeObjects.clear(); }
void GB2ShapeCache::addFixturesToBody
(b2Body *body, const std::string &shape) {
    std::map<std::string, BodyDef *>::iterator pos
    = shapeObjects.find(shape);
    assert(pos != shapeObjects.end());
    BodyDef *so = (*pos).second;
    FixtureDef *fix = so->fixtures;
    while (fix) {
        body->CreateFixture(&fix->fixture);
        fix = fix->next;
    }
}
cocos2d::CCPoint GB2ShapeCache::anchorPointForShape
(const std::string &shape) {
    std::map<std::string, BodyDef *>::iterator pos
    = shapeObjects.find(shape);
    assert(pos != shapeObjects.end());
    BodyDef *bd = (*pos).second;
    return bd->anchorPoint;
}
void GB2ShapeCache::addShapesWithFile(const std::string &plist) {
    const char *fullName = CCFileUtils::sharedFileUtils()
    ->fullPathForFilename(plist.c_str()).c_str();
    CCDictionary *dict = CCDictionary::createWithContentsOfFile(fullName);
    CCAssert(dict != NULL, "Shape-file not found");
    // not triggered - cocos2dx delivers empty dict if non was found
    CCAssert(dict->count() != 0, "plist file empty or not existing");
    CCDictionary *metadataDict = (CCDictionary *)dict->
    objectForKey("metadata");
    int format = static_cast<CCString *>(metadataDict->
                                         objectForKey("format"))->intValue();
    ptmRatio = static_cast<CCString *>(metadataDict
                                       ->objectForKey("ptm_ratio"))->floatValue();
    CCAssert(format == 1, "Format not supported");
    CCDictionary *bodyDict = (CCDictionary *)dict
    ->objectForKey("bodies");
    b2Vec2 vertices[b2_maxPolygonVertices];
    CCDictElement* pElement = NULL;
    CCDICT_FOREACH(bodyDict, pElement){
        CCDictionary *bodyData = (CCDictionary *)pElement->getObject();
        BodyDef *bodyDef = new BodyDef();
        bodyDef->anchorPoint = CCPointFromString(
                                                 static_cast<CCString *>(bodyData
                                                                         ->objectForKey("anchorpoint"))->getCString());
        CCArray *fixtureList = (CCArray*)bodyData->objectForKey("fixtures");
        FixtureDef **nextFixtureDef = &(bodyDef->fixtures);
        CCObject *obj;
        CCARRAY_FOREACH(fixtureList, obj){
            b2FixtureDef basicData;
            CCDictionary *fixtureData = (CCDictionary *)obj;
            basicData.filter.categoryBits =
            static_cast<CCString *>(fixtureData
                                    ->objectForKey("filter_categoryBits"))->intValue();
            basicData.filter.maskBits = static_cast<CCString *>
            (fixtureData->objectForKey("filter_maskBits"))->intValue();
            basicData.filter.groupIndex = static_cast<CCString *>
            (fixtureData->objectForKey("filter_groupIndex"))->intValue();
            basicData.friction = static_cast<CCString *>
            (fixtureData->objectForKey("friction"))->floatValue();
            basicData.density = static_cast<CCString *>
            (fixtureData->objectForKey("density"))->floatValue();
            basicData.restitution = static_cast<CCString *>
            (fixtureData->objectForKey("restitution"))->floatValue();
            basicData.isSensor = (bool)static_cast<CCString *>
            (fixtureData->objectForKey("isSensor"))->floatValue();
            CCString *cb = static_cast<CCString *>
            (fixtureData->objectForKey("userdataCbValue"));
            int callbackData = 0;
            if (cb)
                callbackData = cb->intValue();
            std::string fixtureType = static_cast<CCString *>
            (fixtureData->objectForKey("fixture_type"))->getCString();
            if (fixtureType == "POLYGON") {
                CCArray *polygonsArray = (CCArray *)(fixtureData->
                                                     objectForKey("polygons"));
                CCObject *obj1;
                CCARRAY_FOREACH(polygonsArray, obj1){
                    FixtureDef *fix = new FixtureDef();
                    fix->fixture = basicData; // copy basic data
                    fix->callbackData = callbackData;
                    b2PolygonShape *polyshape = new b2PolygonShape();
                    int vindex = 0;
                    CCArray *polygonArray = (CCArray *)(obj1);
                    assert(polygonArray->count() <= b2_maxPolygonVertices);
                    CCObject *obj2;
                    CCARRAY_FOREACH(polygonArray, obj2){
                        CCPoint offset = CCPointFromString(((CCString *)obj2)
                                                           ->getCString());
                        vertices[vindex].x = (offset.x / ptmRatio) ;
                        vertices[vindex].y = (offset.y / ptmRatio) ;
                        vindex++;
                    }
                    polyshape->Set(vertices, vindex);
                    fix->fixture.shape = polyshape;
                    // create a list
                    *nextFixtureDef = fix;
                    nextFixtureDef = &(fix->next);
                }
            } else if (fixtureType == "CIRCLE") {
                FixtureDef *fix = new FixtureDef();
                fix->fixture = basicData; // copy basic data
                fix->callbackData = callbackData;
                CCDictionary *circleData = (CCDictionary *)fixtureData
                ->objectForKey("circle");
                b2CircleShape *circleShape = new b2CircleShape();
                circleShape->m_radius = static_cast<CCString *>
                (circleData->objectForKey("radius"))->floatValue() / ptmRatio;
                CCPoint p = CCPointFromString(static_cast<CCString *>
                                              (circleData->objectForKey("position"))->getCString());
                circleShape->m_p = b2Vec2(p.x / ptmRatio, p.y / ptmRatio);
                fix->fixture.shape = circleShape;
                // create a list
                *nextFixtureDef = fix;
                nextFixtureDef = &(fix->next);
            } else {
                CCAssert(0, "Unknown fixtureType");
            }
        }
        // add the body element to the hash
        shapeObjects[pElement->getStrKey()] = bodyDef;
    }
}

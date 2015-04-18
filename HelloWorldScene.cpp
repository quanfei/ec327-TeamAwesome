#include "HelloWorldScene.h"
#include "Enemys.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    
    // 'scene' is an autorelease object
    Scene *scene = Scene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    GameMenu *menu = new GameMenu();
    menu->init();
    scene->addChild(menu);
    layer->_menu = menu;
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance

bool HelloWorld::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    

    _tileMap = TMXTiledMap::create("ai_map.tmx");
    //_tileMap->initWithTMXFile("ai_map.tmx");
    _background = _tileMap->getLayer("Background");
    
    _foreground_1 = _tileMap->getLayer("Foreground_1");
    _foreground_2 = _tileMap->getLayer("Foreground_2");
    _meta = _tileMap->getLayer("Meta");
    _meta->setVisible(false);
    
    this->addChild(_tileMap);
    
    
    TMXObjectGroup *objectGroup = _tileMap->getObjectGroup("Objects");
    CCASSERT(NULL != objectGroup, "SpawnPoint object not found");
    auto spawnPoint = objectGroup->getObject("SpawnPoint");
    CCASSERT(!spawnPoint.empty(), "SpawnPoint object not found");
    //CCDictionary *spawnPoint = objectGroup->objectNamed("SpawnPoint");
    
    //int x = ((CCString)spawnPoint.valueForKey("x")).intValue();
    //int y = ((CCString)spawnPoint.valueForKey("y")).intValue();
    int x = spawnPoint["x"].asInt();
    int y = spawnPoint["y"].asInt();
    
    // player
    _player = Sprite::create("egg.png");
    _player->setTag(5);
    _player->setPosition(x,y);
    addChild(_player);
    setViewPointCenter(_player->getPosition());
    
    // patrol enemy
    searching_enemy = Enemys::createWithLayer(this);
    searching_enemy->setGameLayer(this);
    auto enemy = objectGroup->getObject("EnemySpawn1");
    int x_1 = enemy["x"].asInt();
    int y_1 = enemy["y"].asInt();
    addEnemyAtPos(Point(x_1,y_1));
    _enemies.pushBack(searching_enemy);
    
    // archer
    archer = Sprite::create("Kiwi.png");
    archer->setPosition(positionForTileCoord(Point(12, 4)));
    archer->setTag(23);
    archer->setScale(1.0);
    this->addChild(archer,1);
    _enemies.pushBack(archer);
    
    //boss
    boss = Sprite::create("patrol.png");
    boss->setPosition(positionForTileCoord(Point(26,9)));
    boss->setTag(26);
    boss->setScale(1.7);
    this->addChild(boss);

    _enemies.pushBack(boss);
    
    //a princess
    princess = Sprite::create("Princess.png");
    princess->setPosition(positionForTileCoord(Point(28, 10)));
    princess->setScale(0.6);
    this->addChild(princess);
    
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan     = [&](Touch* touch, Event* unused_event)->bool { return true;  };
    listener->onTouchEnded     = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    this->_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    
    // a pause button
    auto pauseItem = MenuItemImage::create("pause.png","",CC_CALLBACK_1(HelloWorld::onPause, this));
    auto menu = Menu::create(pauseItem,NULL);
    
    pauseItem->setScale(0.2);
    pauseItem->setPosition(positionForTileCoord(Point(27, 1)));
    
    menu->setPosition(Point::ZERO);
    this->addChild(menu,1,5);

    // add background music
    //CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic("SummerNight.wav");
    
    // navigation button:up, down, left and right and an attacking button
    auto left_btn = Sprite::create("left.png");
    left_btn->setScale(0.3);
    left_btn->setPosition(Point(100, 125));
    left_btn->setTag(TAG_LEFT);
    this->addChild(left_btn,1);
    
    auto right_btn = Sprite::create("right.png");
    right_btn->setScale(0.3);
    right_btn->setPosition(Point(250, 125));
    right_btn->setTag(TAG_RIGHT);
    this->addChild(right_btn,1);
    
    auto up_btn = Sprite::create("up.png");
    up_btn->setScale(0.3);
    up_btn->setPosition(Point(175, 200));
    up_btn->setTag(TAG_UP);
    this->addChild(up_btn,1);
    
    auto down_btn = Sprite::create("down.png");
    down_btn->setScale(0.3);
    down_btn->setPosition(Point(175, 50));
    down_btn->setTag(TAG_DOWN);
    this->addChild(down_btn,1);

    auto attack_btn = Sprite::create("attack.png");
    attack_btn->setScale(0.5);
    attack_btn->setPosition(positionForTileCoord(Point(25,16)));
    attack_btn->setTag(ATTACK);
    this->addChild(attack_btn,1);
    
    //hero hp
    Sprite *heroEmHP = Sprite::create("empty.png");
    heroEmHP->setPosition(Point(150, 600));
    heroEmHP->setScale(1.5);
    this->addChild(heroEmHP);
    Sprite* heroFuHP = Sprite::create("full.png");
    ProgressTimer *pBlood = ProgressTimer::create(heroFuHP);
    pBlood->setType(kCCProgressTimerTypeBar);
    pBlood->setMidpoint(Point(0, 0));
    pBlood->setBarChangeRate(Point(1, 0));
    pBlood->setPercentage(100.0f);
    pBlood->setPosition(Point(150, 600));
    pBlood->setScale(1.5);
    this->addChild(pBlood, 1, 6);
    auto hero =Sprite::create("hero.png");
    hero->setPosition(Point(50, 600));
    hero->setScale(0.3);
    this->addChild(hero);
    
    //patrol monster hp
    Sprite *enEmHP = Sprite::create("empty.png");
    enEmHP->setPosition(Point(150, 550));
    enEmHP->setScale(1.5);
    this->addChild(enEmHP,1,7);
    Sprite* enFuHP = Sprite::create("full.png");
    ProgressTimer *enpBlood = ProgressTimer::create(enFuHP);
    enpBlood->setType(kCCProgressTimerTypeBar);
    enpBlood->setMidpoint(Point(0, 0));
    enpBlood->setBarChangeRate(Point(1, 0));
    enpBlood->setPercentage(100.0f);
    enpBlood->setPosition(Point(150, 550));
    enpBlood->setScale(1.5);
    this->addChild(enpBlood, 1,8);
    auto monster = Sprite::create("monster.png");
    monster->setPosition(Point(50,550));
    monster->setScale(0.1);
    monster->setTag(9);
    this->addChild(monster);

    //archer
    Sprite *arEmHP = Sprite::create("empty.png");
    arEmHP->setPosition(Point(350, 600));
    arEmHP->setScale(1.5);
    this->addChild(arEmHP,1,10);
    Sprite* arFuHP = Sprite::create("full.png");
    ProgressTimer *arpBlood = ProgressTimer::create(arFuHP);
    arpBlood->setType(kCCProgressTimerTypeBar);
    arpBlood->setMidpoint(Point(0, 0));
    arpBlood->setBarChangeRate(Point(1, 0));
    arpBlood->setPercentage(100.0f);
    arpBlood->setPosition(Point(350, 600));
    arpBlood->setScale(1.5);
    this->addChild(arpBlood, 1,11);
    auto armonster = Sprite::create("archer_icon.png");
    armonster->setPosition(Point(250,600));
    armonster->setScale(0.1);
    armonster->setTag(12);
    this->addChild(armonster);

    //boss
    Sprite *boEmHP = Sprite::create("empty.png");
    boEmHP->setPosition(Point(350, 550));
    boEmHP->setScale(1.5);
    this->addChild(boEmHP,1,13);
    Sprite* boFuHP = Sprite::create("full.png");
    ProgressTimer *bopBlood = ProgressTimer::create(boFuHP);
    bopBlood->setType(kCCProgressTimerTypeBar);
    bopBlood->setMidpoint(Point(0, 0));
    bopBlood->setBarChangeRate(Point(1, 0));
    bopBlood->setPercentage(100.0f);
    bopBlood->setPosition(Point(350, 550));
    bopBlood->setScale(1.5);
    this->addChild(bopBlood, 1,14);
    auto bomonster = Sprite::create("boss_icon.png");
    bomonster->setPosition(Point(250,550));
    bomonster->setScale(0.2);
    bomonster->setTag(15);
    this->addChild(bomonster);

    this->schedule(schedule_selector(HelloWorld::shooting), 2.5f);
    this->schedule(schedule_selector(HelloWorld::bulletCollision));
    this->schedule(schedule_selector(HelloWorld::getPrincess));
    this->schedule(schedule_selector(HelloWorld::bossAttacking),1.0f);
    return true;
}

void HelloWorld::onTouchEnded(Touch* touch, Event* unused_event) {
    auto beginPos = touch->getLocationInView();
    beginPos = Director::getInstance()->convertToGL(beginPos);
    auto playerPos = _player->getPosition();
    for (int i=0;i<5;i++){
        auto dir = (Sprite*)this->getChildByTag(i);
        if(dir->getBoundingBox().containsPoint(beginPos)){
            switch (i) {
                case TAG_LEFT:playerPos.x -= _tileMap->getTileSize().width;break;
                case TAG_RIGHT:playerPos.x += _tileMap->getTileSize().width;break;
                case TAG_UP:playerPos.y += _tileMap->getTileSize().height;break;
                case TAG_DOWN:playerPos.y -= _tileMap->getTileSize().height;break;
                case ATTACK:HelloWorld::heroAttacking();break;
                default:
                    break;
            }
        }
            
    }
    
    if (playerPos.x <= (_tileMap->getMapSize().width * _tileMap->getTileSize().width) &&
        playerPos.y <= (_tileMap->getMapSize().height * _tileMap->getTileSize().height) &&
        playerPos.y >= 0 &&
        playerPos.x >= 0 )
    {
        this->setPlayerPosition(playerPos);
    }
    
    this->setViewPointCenter(_player->getPosition());
 
    
}


void HelloWorld::setPlayerPosition(Point position) {
    
    //CCLOG("player position: %f, %f", position.x, position.y);
    Point tileCoord = this->tileCoordForPosition(position);
    //CCLOG("player tileCoord: %f, %f", tileCoord.x,tileCoord.y);
    int tileGid = _meta->getTileGIDAt(tileCoord);
    if (tileGid) {
        auto properties = _tileMap->getPropertiesForGID(tileGid).asValueMap();
        if (!properties.empty()) {
            auto collision = properties["Collidable"].asString();
            if ("True" == collision) {
                return;
            }
            auto collectible = properties["Collectable"].asString();
            if ("True" == collectible) {
                //_numCollected++;
                //_hud->numCollectedChanged(_numCollected);
                _meta->removeTileAt(tileCoord);
                _foreground_2->removeTileAt(tileCoord);
                _player->setPosition(position);
                return;
            }
        }
    }

    
    _player->setPosition(position);
}



void HelloWorld::setViewPointCenter(Point position) {
    
    auto winSize = Director::getInstance()->getWinSize();
    
    int x = MAX(position.x, winSize.width/2);
    int y = MAX(position.y, winSize.height/2);
    x = MIN(x, (_tileMap->getMapSize().width * this->_tileMap->getTileSize().width) - winSize.width / 2);
    y = MIN(y, (_tileMap->getMapSize().height * _tileMap->getTileSize().height) - winSize.height/2);
    auto actualPosition = Point(x, y);
    
    auto centerOfView = Point(winSize.width/2, winSize.height/2);
    auto viewPoint = centerOfView - actualPosition;
    this->setPosition(viewPoint);
}




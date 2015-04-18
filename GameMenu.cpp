#include "GameMenu.h"
#include "MainMenu.h"
#include "HelloWorldScene.h"
#include "HudLayer.h"
USING_NS_CC;

Scene* GameMenu::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameMenu::create();
    
    // add layer as a child to scene
    scene->addChild(layer);
    
    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameMenu::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();
    
    
    //////////// set the background
    auto bg = Sprite::create("bg.jpg");
    bg->setScale(2.0);
    // position the sprite on the center of the screen
    bg->setPosition(Point(origin.x + visibleSize.width/2,
                          origin.y + visibleSize.height - bg->getContentSize().height));
    
    // add the sprite as a child to this layer
    this->addChild(bg, 0, 0);
    
    ///////////  create game menu
    // a start game button
    auto startItem = MenuItemImage::create("buttonStart.png","",CC_CALLBACK_1(GameMenu::StartCallback, this));
    startItem->setScale(1.5);
    startItem->setPosition(Point(origin.x + visibleSize.width - startItem->getContentSize().width/2-375 ,
                                 origin.y + startItem->getContentSize().height/2+150));
    // a exit button
    auto closeItem = MenuItemImage::create("buttonExit.png","", CC_CALLBACK_1(GameMenu::menuCloseCallback, this));
    closeItem->setScale(1.5);
    closeItem->setPosition(Point(origin.x + visibleSize.width - startItem->getContentSize().width/2-375 ,
                                 origin.y + startItem->getContentSize().height/2+50));
    // create menu, it's an autorelease object
    auto menu = Menu::create(startItem,closeItem,NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu, 1, 3);
    
    
    ////////// create and initialize a label
    auto logo = Sprite::create("rev_logo.png");
    logo->setScale(1.5);
    this->addChild(logo,4,4);
    logo->setPosition(Point(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - logo->getContentSize().height));
    
    //auto label = LabelTTF::create("Revenge of Kiwi", "Arial", 24);
    
    // position the label on the center of the screen
    //label->setPosition(Point(origin.x + visibleSize.width/2,
      //                       origin.y + visibleSize.height - label->getContentSize().height));
    
    // add the label as a child to this layer
    //this->addChild(label, 2, 2);
    
    
    
    return true;
}


void GameMenu::StartCallback(Object *pSender)
{
    // start a new scene
    auto newscene = Scene::create();
    auto layer = HelloWorld::create();
    
    newscene->addChild(layer);

    Director::getInstance()->replaceScene(newscene);
    
    
}


void GameMenu::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


#include <_main.hpp>
#include <_updater.hpp>

std::string defaultEffect(std::string name) {
    return GameToolbox::saveParticleToString(
        CCParticleSystemQuad::create(
            (name + ".plist!original").c_str(), 0
        )
    );
}

class ParticlePopup : public CreateParticlePopup {
public:
    inline static Ref<CreateParticlePopup> sharedObject = nullptr;
    static auto get(std::string data) {
        if (!sharedObject.data()) sharedObject = CreateParticlePopup::create(nullptr, nullptr);
        sharedObject->removeAllChildrenWithCleanup(1);
        sharedObject->init(nullptr, nullptr, data);
        return sharedObject;
    }
};

class ParticlePreview : public CCLayerColor {
public:
    Ref<CCParticleSystemQuad> m_particleRef;
    static ParticlePreview* create(std::string fileName) {
        ParticlePreview* pRet = new ParticlePreview();
        if (pRet && pRet->init(CCParticleSystemQuad::create(fileName.c_str(), 0))) {
            pRet->autorelease(); return pRet;
        }
        else {
            delete pRet; pRet = 0; return 0;
        }
    };
    static ParticlePreview* createFromString(std::string data) {
        ParticlePreview* pRet = new ParticlePreview();
        if (pRet && pRet->init(GameToolbox::particleFromString(data, CCParticleSystemQuad::create(), 0))) {
            pRet->autorelease(); return pRet;
        }
        else {
            delete pRet; pRet = 0; return 0;
        }
    };
    bool init(CCParticleSystemQuad* particleRef) {

        m_particleRef = particleRef;

        CCLayerColor::init();
        setContentSize({ 110.f, 90.f });
        setColor(cocos2d::ccBLACK);
        setOpacity(cocos2d::ccWHITE.r);
        
#ifndef __APPLE__
        auto scroll = CCScrollView::create(this->getContentSize(), m_particleRef);
#else
        auto scroll = CCLayer::create();
        scroll->setContentSize(this->getContentSize());
        scroll->addChild(m_particleRef);
#endif // !__APPLE__

        addChild(scroll);

        m_particleRef->setContentSize(this->getContentSize());
        m_particleRef->setAnchorPoint({ -0.5f, -0.5f });

        return 1;
    }
};

#include <Geode/modify/GJGarageLayer.hpp>
class $modify(GJGarageLayerCustomPlayerParticles, GJGarageLayer) {
    $override bool init() {
        if (!GJGarageLayer::init()) return 0;
            
        auto image_icon = GameObject::createWithKey(2065);// "edit_eCParticleBtn_001.png");
        auto image = CircleButtonSprite::create(image_icon, CircleBaseColor::Gray);

        image_icon->setScale(0.930);
        image->setScale(0.920);

        auto openupbtn = CCMenuItemExt::createSpriteExtra(
            image, [this](CCMenuItem* item) {

                auto createSavedEffectsPopup = [item](std::string effectName)
                    {
                        auto popup = createQuickPopup(
                            effectName.c_str(),
                            "\n \n \n \n \n \n \n \n ",
                            "Go Back", nullptr,
                            [item](auto, auto) { item->activate(); }
                        );
                        popup->setID("savedEffectsPopup"_spr);
                        popup->setZOrder(CCScene::get()->getChildrenCount());
                        popup->m_mainLayer->setPositionX(56.f);
                        popup->m_buttonMenu->setPositionX(100.f + popup->m_buttonMenu->getPositionX());

                        auto subbg = CCScale9Sprite::create("GJ_square07.png");
                        subbg->setPosition(popup->getContentSize() / 2);
                        subbg->setAnchorPoint({ 0.625f, 0.5f });
                        subbg->setContentSize({ 479.f, 292.f });
                        popup->m_mainLayer->addChild(subbg);

                        auto subbgbackdrop = CCScale9Sprite::create("GJ_square06.png");
                        subbgbackdrop->setPosition(subbg->getPosition());
                        subbgbackdrop->setAnchorPoint(subbg->getAnchorPoint());
                        subbgbackdrop->setContentSize(subbg->getContentSize());
                        subbgbackdrop->setColor(ccBLACK);
                        subbgbackdrop->setOpacity(120);
                        popup->m_mainLayer->addChild(subbgbackdrop, -2);

                        auto resetFileParticleImage = ButtonSprite::create(
                            "Restart", 141.000f, 1, "bigFont.fnt", "GJ_button_05.png", 0, 0.7f
                        );
                        resetFileParticleImage->setScale(0.7f);
                        auto resetFileParticle = CCMenuItemExt::createSpriteExtra(
                            resetFileParticleImage, [popup, effectName](auto) {
                                popup->m_buttonMenu->removeChildByTag(5719);
                                popup->m_buttonMenu->removeChildByTag(5719);

                                auto selectedPrev = ParticlePreview::create(effectName + ".plist");
                                selectedPrev->setPosition({ -392.f, 4.f });
                                popup->m_buttonMenu->addChild(selectedPrev, 0, 5719);

                                auto selectedPrevTitle = CCLabelBMFont::create(
                                    (effectName + ".plist").c_str(), "chatFont.fnt"
                                );
                                selectedPrevTitle->setPosition({ -338.000f, 14.000f });
                                selectedPrevTitle->limitLabelWidth(selectedPrev->getContentWidth() - 22.f, 0.850f, 0.1f);
                                popup->m_buttonMenu->addChild(selectedPrevTitle, 0, 5719);

                                handleTouchPriority(popup);
                            }
                        );
                        resetFileParticle->activate();
                        resetFileParticle->setPosition({ -338.000f, -15.000f });
                        popup->m_buttonMenu->addChild(resetFileParticle, 1);

                        //card{ 142.000f, 182.000f }
                        auto scroll = ScrollLayer::create({ 292.000f, 182.000f });
                        scroll->setPosition({ -242.000f, 24.000f });
                        scroll->setTouchEnabled(0);
                        popup->m_buttonMenu->addChild(scroll);

                        scroll->m_contentLayer->setOpacity(90.f);

                        auto scrollOutline = CCScale9Sprite::create("GJ_square07.png");
                        scrollOutline->setPositionX(scroll->getPosition().x + scroll->getContentWidth() / 2);
                        scrollOutline->setPositionY(scroll->getPosition().y + scroll->getContentHeight() / 2);
                        scrollOutline->setContentSize(scroll->getContentSize() + CCSizeMake(1,1) * 3);
                        popup->m_buttonMenu->addChild(scrollOutline);

                        auto sbar = Scrollbar::create(scroll);
                        sbar->setTouchEnabled(0);
                        sbar->setPosition(-151.f, 11.f);
                        sbar->setRotation(-90.f);
                        popup->m_buttonMenu->addChild(sbar);

                        auto goLeft = CCMenuItemExt::createSpriteExtra(
                            CCLabelBMFont::create(">", "bigFont.fnt"), [popup, scroll](auto) {
                                scroll->scrollLayer(scroll->getContentHeight());
                            }
                        );
                        goLeft->setSizeMult(1.3f);
                        goLeft->setPosition(56.000f, scrollOutline->getPositionY());
                        popup->m_buttonMenu->addChild(goLeft);

                        auto goRight = CCMenuItemExt::createSpriteExtra(
                            CCLabelBMFont::create("<", "bigFont.fnt"), [popup, scroll](auto) {
                                scroll->scrollLayer(-scroll->getContentHeight());
                            }
                        );
                        goRight->setSizeMult(1.3f);
                        goRight->setPosition(-246.000, scrollOutline->getPositionY());
                        popup->m_buttonMenu->addChild(goRight);

                        auto createParticleCard = [popup, scroll, resetFileParticle, effectName](std::string data, int id)
                            {
                                auto menu = CCMenu::create();
                                menu->setContentSize({ 142.000f, 182.000f });
                                menu->setPosition(CCPointZero);

                                menu->addChild(cocos::createDataNode("data", data, id));

                                auto bg = CCScale9Sprite::create("geode.loader/black-square.png");
                                bg->setAnchorPoint(CCPointZero);
                                bg->setContentSize(menu->getContentSize());
                                menu->addChild(bg, -1);

                                auto restartParticleImage = ButtonSprite::create(
                                    "Restart", "bigFont.fnt", "GJ_button_05.png", 0.8f
                                );
                                restartParticleImage->setScale(0.6f);
                                restartParticleImage->m_BGSprite->setContentWidth(184.000f);
                                restartParticleImage->m_BGSprite->setContentHeight(32.000f);
                                auto restartParticle = CCMenuItemExt::createSpriteExtra(
                                    restartParticleImage, [popup, menu](auto) {
                                        menu->removeChildByTag(13384);
                                        auto data = std::string(findDataNode(menu, "data")->getString());
                                        auto selectedPrev = ParticlePreview::createFromString(data);
                                        selectedPrev->setPosition({ 16.f, 80.f });
                                        menu->addChild(selectedPrev, 0, 13384);
                                        handleTouchPriority(popup);
                                    }
                                );
                                restartParticle->activate();
                                restartParticle->setPosition({ menu->getContentWidth() / 2, 66.f });
                                menu->addChild(restartParticle, 1);

                                auto idlabel = CCLabelBMFont::create(fmt::format("# {}", id).c_str(), "chatFont.fnt");
                                idlabel->setPosition({ menu->getContentWidth() / 2, 96.f });
                                idlabel->limitLabelWidth(110.f, 0.5f, 0.1f);
                                menu->addChild(idlabel, 1);

                                auto copyParticleStrLabel = CCLabelBMFont::create("- copy string -", "chatFont.fnt");
                                copyParticleStrLabel->setScale(0.6);
                                auto copyParticleStr = CCMenuItemExt::createSpriteExtra(
                                    copyParticleStrLabel, [menu, restartParticle, resetFileParticle, effectName, id](auto) {
                                        auto data = std::string(findDataNode(menu, "data")->getString());
                                        geode::utils::clipboard::write(data);
                                        auto ntfy = Notification::create("particle string was wrote to clipboard!");
                                        ntfy->show();
                                    }
                                );
                                copyParticleStr->setPosition({ menu->getContentWidth() / 2, 88.f });
                                menu->addChild(copyParticleStr, 1);

                                auto useParticleImage = ButtonSprite::create(
                                    "Use This", "bigFont.fnt", "GJ_button_03.png", 0.8f
                                );
                                useParticleImage->setScale(0.6f);
                                useParticleImage->m_BGSprite->setContentWidth(184.000f);
                                useParticleImage->m_BGSprite->setContentHeight(32.000f);
                                auto useParticle = CCMenuItemExt::createSpriteExtra(
                                    useParticleImage, [menu, restartParticle, resetFileParticle, effectName](auto) {
                                        auto data = std::string(findDataNode(menu, "data")->getString());
                                        Mod::get()->setSavedValue<std::string>(effectName, data);
                                        Mod::get()->saveData();
                                        restartParticle->activate();//menu, restartParticle, resetFileParticle
                                        resetFileParticle->activate();
                                    }
                                );
                                useParticle->setPosition({ menu->getContentWidth() / 2, 42.000f });
                                menu->addChild(useParticle, 1);

                                auto deleteParticleImage = ButtonSprite::create(
                                    "Delete", "bigFont.fnt", "GJ_button_06.png", 0.8f
                                );
                                deleteParticleImage->setScale(0.6f);
                                deleteParticleImage->m_BGSprite->setContentWidth(184.000f / 1.8f);
                                deleteParticleImage->m_BGSprite->setContentHeight(32.000f);
                                auto deleteParticle = CCMenuItemExt::createSpriteExtra(
                                    deleteParticleImage, [menu, scroll, resetFileParticle, effectName, id](auto) {

                                        auto data = std::string(findDataNode(menu, "data")->getString());
                                        auto arr = Mod::get()->getSavedValue<matjson::Value>(effectName + ".saved").asArray().unwrapOrDefault();
                                        auto newarr = matjson::Value().array().asArray().unwrapOrDefault();
                                        for (auto val : arr) {
                                            if (val.asArray().unwrap()[0] != id) {
                                                newarr.push_back(val);
                                            }
                                        }
                                        if (Mod::get()->getSavedValue<std::string>(effectName) == data) {
                                            Mod::get()->getSaveContainer().erase(effectName);
                                            resetFileParticle->activate();
                                        }
                                        Mod::get()->setSavedValue(effectName + ".saved", newarr);
                                        Mod::get()->saveData();

                                        auto parent = menu->getParent();
                                        menu->removeMeAndCleanup();
                                        parent->updateLayout();
                                        scroll->scrollLayer(0.f);
                                    }
                                );
                                deleteParticle->setAnchorPoint({ 1.f, 0.5f });
                                deleteParticle->setPosition({ menu->getContentWidth() / 1.8f, 18.000f });
                                menu->addChild(deleteParticle, 1);

                                auto editParticleImage = ButtonSprite::create(
                                    "Edit", "bigFont.fnt", "GJ_button_02.png", 0.8f
                                );
                                editParticleImage->setScale(0.6f);
                                editParticleImage->m_BGSprite->setContentWidth(184.000f / 2.5f);
                                editParticleImage->m_BGSprite->setContentHeight(32.000f);
                                auto editParticle = CCMenuItemExt::createSpriteExtra(
                                    editParticleImage, [popup, menu, restartParticle, resetFileParticle, effectName, id](auto) {
                                        auto data = std::string(findDataNode(menu, "data")->getString());
                                        auto particle_popup = ParticlePopup::get(data);
                                        particle_popup->show();
                                        particle_popup->setZOrder(CCScene::get()->getChildrenCount());
                                        particle_popup->runAction(/*CCRepeatForever::create*/(CCSequence::create(
                                            CCDelayTime::create(0.1f),
                                            CCLambdaAction::create(
                                                [particle_popup]() {
                                                    //aaaaaaahhhh fuck this shit so much
                                                    cocos::setTouchPriority(particle_popup.data(), -506);
                                                    cocos::findFirstChildRecursive<CCLayer>(
                                                        particle_popup->m_mainLayer, [](CCLayer* node) {
                                                            cocos::setTouchPriority(node, -507);
                                                            return false;
                                                        }
                                                    );
                                                    //log::debug("{}", __FUNCTION__);
                                                }
                                            ), nullptr
                                        )));
                                        auto okBtn = cocos::findFirstChildRecursive<CCMenuItemSpriteExtra>(particle_popup, [](auto) {return true; });
                                        CCMenuItemExt::assignCallback<CCMenuItemSpriteExtra>(
                                            okBtn, [particle_popup, popup, menu, restartParticle, resetFileParticle, effectName, id](auto) {
                                                findDataNode(menu, "data")->setString(
                                                    GameToolbox::saveParticleToString(particle_popup->m_particle).c_str()
                                                );
                                                particle_popup->removeFromParentAndCleanup(0);

                                                auto data = std::string(findDataNode(menu, "data")->getString());
                                                auto arr = Mod::get()->getSavedValue<matjson::Value>(effectName + ".saved").asArray().unwrapOrDefault();
                                                auto newarr = matjson::Value().array().asArray().unwrapOrDefault();
                                                for (auto val : arr) {
                                                    if (val.asArray().unwrap()[0] == id) {
                                                        val.asArray().unwrap()[1] = (data);
                                                    }
                                                    newarr.push_back(val);
                                                }
                                                Mod::get()->setSavedValue(effectName + ".saved", newarr);
                                                Mod::get()->saveData();

                                                restartParticle->activate();//menu, restartParticle, resetFileParticle
                                                resetFileParticle->activate();
                                            }
                                        );
                                        handleTouchPriority(popup, 1);
                                        //handleTouchPriority(menu);
                                    }
                                );
                                editParticle->setAnchorPoint({ 0.f, 0.5f });
                                editParticle->setPosition({ menu->getContentWidth() / 1.74f, 18.000f });
                                menu->addChild(editParticle, 1);

                                return menu;
                            };

                        CCMenu* menu = CCMenu::create();

                        auto createDefaultLabel = CCLabelBMFont::create("From Default:", "goldFont.fnt");
                        createDefaultLabel->limitLabelWidth(110.f, 0.6f, 0.1f);
                        menu->addChild(createDefaultLabel);

                        auto createnewImage = ButtonSprite::create(
                            "Create", 141.000f, 1, "bigFont.fnt", "GJ_button_04.png", 0, 0.7f
                        );
                        createnewImage->setScale(0.7f);
                        auto createnew = CCMenuItemExt::createSpriteExtra(
                            createnewImage, [popup, effectName, scroll, createParticleCard](auto) {

                                auto arr = Mod::get()->getSavedValue<matjson::Value>(effectName + ".saved").asArray().unwrapOrDefault();
                                    
                                auto freeID = 1;
                                for (auto particle_arr : arr) {
                                    auto particle_id = particle_arr[0].asInt().unwrapOr(-1);
                                    if (particle_id >= freeID) freeID = (particle_id + 1);
                                }

                                auto item = matjson::Value().array();
                                item.asArray().unwrap().push_back(freeID);
                                item.asArray().unwrap().push_back(defaultEffect(effectName));

                                arr.push_back(item);

                                Mod::get()->setSavedValue(effectName + ".saved", arr);
                                Mod::get()->saveData();

                                auto card = createParticleCard(
                                    defaultEffect(effectName),
                                    freeID
                                );
                                scroll->m_contentLayer->addChild(card);
                                scroll->m_contentLayer->updateLayout();
                            }
                        );
                        menu->addChild(createnew);

                        auto createFromStringLabel = CCLabelBMFont::create("From String:", "goldFont.fnt");
                        createFromStringLabel->limitLabelWidth(110.f, 0.6f, 0.1f);
                        menu->addChild(createFromStringLabel);

                        auto inputParticleStr = TextInput::create(110.000f, "particle\nstring here");
                        inputParticleStr->setCommonFilter(CommonFilter::Any);
                        menu->addChild(inputParticleStr);

                        auto createFromStringImage = ButtonSprite::create(
                            "Create", 141.000f, 1, "bigFont.fnt", "GJ_button_04.png", 0, 0.7f
                        );
                        createFromStringImage->setScale(0.7f);
                        auto createFromString = CCMenuItemExt::createSpriteExtra(
                            createFromStringImage, [popup, inputParticleStr, effectName, scroll, createParticleCard](auto) {

                                auto arr = Mod::get()->getSavedValue<matjson::Value>(effectName + ".saved").asArray().unwrapOrDefault();

                                auto freeID = 1;
                                for (auto particle_arr : arr) {
                                    auto particle_id = particle_arr[0].asInt().unwrapOr(-1);
                                    if (particle_id >= freeID) freeID = (particle_id + 1);
                                }

                                auto item = matjson::Value().array();
                                item.asArray().unwrap().push_back(freeID);
                                item.asArray().unwrap().push_back(inputParticleStr->getString());

                                arr.push_back(item);

                                Mod::get()->setSavedValue(effectName + ".saved", arr);
                                Mod::get()->saveData();

                                auto card = createParticleCard(
                                    inputParticleStr->getString(),
                                    freeID
                                );
                                scroll->m_contentLayer->addChild(card);
                                scroll->m_contentLayer->updateLayout();

                            }
                        );
                        menu->addChild(createFromString);

                        menu->alignItemsVerticallyWithPadding(6.f);
                        menu->setPosition({ -338.000f, 174.000f });
                        popup->m_buttonMenu->addChild(menu, 1);

                        auto arr = Mod::get()->getSavedValue<matjson::Value>(effectName + ".saved").asArray().unwrapOrDefault();
                        for (auto particle_arr : arr) {
                            auto card = createParticleCard(
                                particle_arr.asArray().unwrap()[1].asString().unwrapOr(""),
                                particle_arr.asArray().unwrap()[0].asInt().unwrapOr(-1)
                            );
                            scroll->m_contentLayer->addChild(card);
                        }

                        scroll->m_contentLayer->setLayout(RowLayout::create()
                            ->setGap(0.f)
                            ->setGrowCrossAxis(1)
                            ->setAxisAlignment(AxisAlignment::Even)
                        );

                        handleTouchPriority(popup);

                    };

                auto selectorPopup = createQuickPopup(
                    "Select Effect", 
                    "\n \n \n \n \n \n \n \n \n",
                    "Close", nullptr, 
                    nullptr
                );
                selectorPopup->setID("selectorPopup"_spr);

                CCMenu* menu = CCMenu::create();
                auto createAndAddItem = [selectorPopup, createSavedEffectsPopup, menu](std::string title, std::string effectName)
                    {
                        auto image = ButtonSprite::create(
                            title.c_str(), "bigFont.fnt", "GJ_button_05.png", 0.7f
                        );
                        image->setScale(0.6f);
                        //image->m_BGSprite->setContentWidth(392.000f);
                        auto item = CCMenuItemExt::createSpriteExtra(
                            image, [selectorPopup, createSavedEffectsPopup, effectName](auto) {
                                selectorPopup->keyBackClicked();
                                createSavedEffectsPopup(effectName);
                            }
                        );
                        menu->addChild(item);
                    };

                createAndAddItem("Drag", "dragEffect");
                createAndAddItem("Ship Drag", "shipDragEffect");
                createAndAddItem("Land", "landEffect");
                createAndAddItem("Dash", "dashEffect");

                createAndAddItem("Burst", "burstEffect");
                createAndAddItem("Burst 2", "burstEffect2");
                createAndAddItem("Swing Burst", "swingBurstEffect");

                createAndAddItem("Back Center Particles", "playerBackCenterParticles");
                createAndAddItem("Front Center Particles", "playerFrontCenterParticles");

                createAndAddItem("Explode", "explodeEffect");
                createAndAddItem("Explode Grav", "explodeEffectGrav");
                createAndAddItem("Explode Vortex", "explodeEffectVortex");

                createAndAddItem("End Portal", "endEffectPortal");
                createAndAddItem("Level Complete", "levelComplete01");
                createAndAddItem("firework", "firework");

                menu->setContentHeight(180.f);
                menu->setContentWidth(320.f);
                menu->setLayout(RowLayout::create()->setGrowCrossAxis(1));
                selectorPopup->m_mainLayer->addChild(menu, 1);

                auto infbtn = CCMenuItemExt::createSpriteExtraWithFrameName("GJ_infoIcon_001.png", 1.0,
                    [](auto) {
                        geode::openInfoPopup(getMod());
                    }
                );
                infbtn->setPosition(146.f, 222.f);
                infbtn->setID("infbtn"_spr);
                selectorPopup->m_buttonMenu->addChild(infbtn);

                handleTouchPriority(selectorPopup);

            }
        );
        openupbtn->setID("openupbtn"_spr);
        openupbtn->setPosition(-28, 28);

        Ref<CCParticleSystemQuad> particles = CCParticleSystemQuad::create("keyEffect.plist", 0);
        particles->setPosition(openupbtn->getContentSize() / 2);
        particles->setRotation(52.00f);
        particles->setScale(1.300f);
        openupbtn->addChild(particles, -3);

        auto menu = CCMenu::createWithItem(openupbtn);
        menu->setID("openupbtn-menu"_spr);
        menu->setPositionX(this->getContentWidth());
        menu->setPositionY(0.f);
        this->addChild(menu, 1);

        return 1;
    }
};

#include <Geode/modify/PlayerObject.hpp>
class $modify(PlayerObjectParticlesAddon, PlayerObject) {
    struct Fields {
        CCParticleSystemQuad* m_backCenter;
        CCParticleSystemQuad* m_frontCenter;
    };
    $override void update(float p0) {
        PlayerObject::update(p0);
        if (auto backCenter = m_fields->m_backCenter) {
            backCenter->setPosition(this->getPosition());
            backCenter->setZOrder(this->getZOrder() - 1);
            this->m_isHidden or this->m_isDead ? backCenter->stopSystem() : backCenter->resumeSystem();
        }
        if (auto frontCenter = m_fields->m_frontCenter) {
            frontCenter->setPosition(this->getPosition());
            frontCenter->setZOrder(this->getZOrder() + 1);
            this->m_isHidden or this->m_isDead ? frontCenter->stopSystem() : frontCenter->resumeSystem();
        }
    }
    $override bool init(int p0, int p1, GJBaseGameLayer * p2, cocos2d::CCLayer * p3, bool p4) {
        if (!PlayerObject::init(p0, p1, p2, p3, p4)) return false;

        if (auto playerBackCenterParticles = CCParticleSystemQuad::create("playerBackCenterParticles.plist", 0)) {
            m_fields->m_backCenter = playerBackCenterParticles;
            m_fields->m_backCenter->setID(this->getID() + "-back-center-particles"_spr);
            if (this->m_parentLayer) this->m_parentLayer->addChild(m_fields->m_backCenter, this->getZOrder() - 1);
        };

        if (auto playerFrontCenterParticles = CCParticleSystemQuad::create("playerFrontCenterParticles.plist", 0)) {
            m_fields->m_frontCenter = playerFrontCenterParticles;
            m_fields->m_frontCenter->setID(this->getID() + "-back-center-particles"_spr);
            if (this->m_parentLayer) this->m_parentLayer->addChild(m_fields->m_frontCenter, this->getZOrder() + 1);
        };

        return true;
    }
};

#include <Geode/modify/CCParticleSystemQuad.hpp>
class $modify(CCParticleSystemQuadCustomPlayerParticles, CCParticleSystemQuad) {
    void customUpdate(float dt = 1337.f) {

        if (this == nullptr) return; //fuck this
        if (!typeinfo_cast<CCParticleSystemQuad*>(this)) return; //hi

        if (dt == 1337.f) {
            this->schedule(
                schedule_selector(CCParticleSystemQuadCustomPlayerParticles::customUpdate)
            );
            //initial_str
            auto original = GameToolbox::particleFromString(
                GameToolbox::saveParticleToString(this), nullptr, 0
            );
            original->setVisible(0);
            original->setID("original"_spr);
            this->addChild(original);
        }
        if (dt == 1337.f) return; //asd

        if (auto original = typeinfo_cast<CCParticleSystemQuad*>(this->getChildByID("original"_spr))) {
            if (getMod()->getSettingValue<bool>("Force initial Gravity")) {
                this->setGravity(original->getGravity());
            }
            if (getMod()->getSettingValue<bool>("Force initial Colors")) {
                this->setStartColor(original->getStartColor());
                this->setStartColorVar(original->getStartColorVar());
                this->setEndColor(original->getEndColor());
                this->setEndColorVar(original->getEndColorVar());
            }
            if (getMod()->getSettingValue<bool>("Force initial Positioning")) {
                this->setPosVar(original->getPosVar());
                this->setPositionType(original->getPositionType());
            }
        }

    }
    static void updateImage(CCParticleSystemQuad* particle, const char* effectFile) {
        auto effectName = fs::path(effectFile).filename().replace_extension("").string();
        auto particleImageFileName = effectName + ".png";
        if (fileExistsInSearchPaths(particleImageFileName.c_str())) {
            auto sprite = CCSprite::create(particleImageFileName.c_str());
            if (sprite) particle->setDisplayFrame(sprite->displayFrame());
        }
    }
    $override static CCParticleSystemQuad* create(const char* file, bool idk) {

        if (string::contains(file, "!original")) return CCParticleSystemQuad::create(
            string::replace(file, "!original", "").c_str(), idk
        );

        auto orgone = CCParticleSystemQuad::create(file, idk);

        auto effectName = fs::path(file).filename().replace_extension("").string();

        auto stringDataFileName = effectName + ".txt";
        if (fileExistsInSearchPaths(stringDataFileName.c_str())) {
            //fs::read uses CCFileUtils::getFileData
            auto data = fs::read(stringDataFileName.c_str());
            auto rtn = GameToolbox::particleFromString(data, CCParticleSystemQuad::create(), idk);
            reinterpret_cast<CCParticleSystemQuadCustomPlayerParticles*>(rtn)->customUpdate();
            updateImage(rtn, file);
            return rtn;
        }

        auto saves = Mod::get()->getSaveContainer();
        if (saves.contains(effectName)) {
            auto saves_effect = saves.get(effectName);
            auto rtn = GameToolbox::particleFromString(
                saves_effect.isOk() ? saves_effect.unwrap().asString().unwrapOr("asd") : defaultEffect(effectName),
                CCParticleSystemQuad::create(), idk
            );
            reinterpret_cast<CCParticleSystemQuadCustomPlayerParticles*>(rtn)->customUpdate();
            updateImage(rtn, file);
            return rtn;
        }

        updateImage(orgone, file);

        return orgone;
    }
};

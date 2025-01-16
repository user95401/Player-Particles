
#include <_ImGui.hpp>
#include <_main.hpp>
#include <_updater.hpp>

std::string defaultEffect(std::string name) {
    return GameToolbox::saveParticleToString(
        CCParticleSystemQuad::create(
            (name + ".plist!original").c_str(), 0
        )
    );
}

// specify parameters for the setup function in the Popup<...> template
class ParticlePopup : public geode::Popup<std::string const&> {
public:
    CCParticleSystemQuad* m_particle;
    ParticleStruct* particleData;
    gd::string ps;

    bool setup(std::string const& value) override {
        this->setOpacity(210);
        m_mainLayer->setPositionX(m_mainLayer->getPositionX() + 110.f);
        m_mainLayer->getChildByType<CCScale9Sprite>(0)->setVisible(0);

        auto bg = CCSprite::create("game_bg_01_001.png");
        bg->setBlendFunc({ GL_ONE, GL_ONE });
        bg->setColor(ccc3(34, 34, 34));
        bg->setFlipY(true);
        bg->setScale(0.375f);
        bg->setPosition(CCPointMake(80.f, 90.f));
        m_mainLayer->addChild(bg, -1, 1337);

        m_particle = CCParticleSystemQuad::create();
        GameToolbox::particleFromString(value, m_particle, 0);
        m_mainLayer->addChildAtPosition(m_particle, Anchor::Center);

        auto scroll = ScrollLayer::create(this->getContentSize());
        m_mainLayer->removeFromParentAndCleanup(0);
        scroll->m_contentLayer->addChild(m_mainLayer);
        scroll->m_disableHorizontal = 0;
        scroll->m_disableVertical = 0;
        scroll->m_scrollLimitTop = 1110;
        scroll->m_scrollLimitBottom = 1110;
        this->addChild(scroll);

        particleData = new ParticleStruct;
        GameToolbox::particleStringToStruct(value, *particleData);
        ps = GameToolbox::saveParticleToString(m_particle);

        this->addChild(ImGuiDrawNode::create(
            [this]() {

#if 1

#define upd ; if (ImGui::IsItemEdited() || ImGui::IsItemDeactivatedAfterEdit()) { GameToolbox::particleFromStruct(*particleData, m_particle, 0); ps = GameToolbox::saveParticleToString(m_particle);  };

#define SLIDER_INT(label, var, min, max) \
    ImGui::Text(label); \
    ImGui::SliderInt("##" label "Slider", &particleData->var, min, max, "%d", ImGuiSliderFlags_AlwaysClamp) upd; \
    ImGui::SameLine(); \
    ImGui::InputInt("##" label "Input", &particleData->var) upd;

#define SLIDER_FLOAT(label, var, min, max, format) \
    ImGui::Text(label); \
    ImGui::SliderFloat("##" label "Slider", &particleData->var, min, max, format, ImGuiSliderFlags_AlwaysClamp) upd; \
    ImGui::SameLine(); \
    ImGui::InputFloat("##" label "Input", &particleData->var, 0.1f, 1.0f, format) upd;


#define COLOR_EDIT(label, r, g, b, a) \
    {float color[4] = { particleData->r, particleData->g, particleData->b, particleData->a }; \
    if (ImGui::ColorEdit4(label, color, ImGuiColorEditFlags_None)) { \
        particleData->r = color[0]; \
        particleData->g = color[1]; \
        particleData->b = color[2]; \
        particleData->a = color[3]; \
        GameToolbox::particleFromStruct(*particleData, m_particle, 0); \
    }}

                auto renderParticleDataSets = [this](int section) {
                    if (section == 0) {
                        // Motion Parameters
                        SLIDER_INT("Max Particles", TotalParticles, 0, 1000);
                        SLIDER_FLOAT("Duration", Duration, -10.0f, 10.0f, "%.2f");
                        SLIDER_FLOAT("Lifetime", Life, 0.0f, 10.0f, "%.2f");
                        SLIDER_FLOAT("Lifetime Variation", LifeVar, 0.0f, 10.0f, "%.2f");
                        SLIDER_INT("Emission", EmissionRate, 0, 1000);
                        SLIDER_INT("Angle", Angle, -180, 180);
                        SLIDER_INT("Angle Variation", AngleVar, 0, 180);
                        SLIDER_INT("Speed", Speed, 0, 100);
                        SLIDER_INT("Speed Variation", SpeedVar, 0, 100);
                        SLIDER_INT("Position Variation X", PosVarX, 0, 100);
                        SLIDER_INT("Position Variation Y", PosVarY, 0, 100);
                        SLIDER_INT("Gravity X", GravityX, -100, 100);
                        SLIDER_INT("Gravity Y", GravityY, -100, 100);
                        SLIDER_INT("Radial Acceleration", RadialAccel, -100, 100);
                        SLIDER_INT("Radial Variation", RadialAccelVar, 0, 100);
                        SLIDER_INT("Tangential Acceleration", TangentialAccel, -100, 100);
                        SLIDER_INT("Tangential Variation", TangentialAccelVar, 0, 100);
                    }
                    else if (section == 1) {
                        // Visual Parameters
                        SLIDER_INT("Start Size", StartSize, 0, 100);
                        SLIDER_INT("Start Size Variation", StartSizeVar, 0, 100);
                        SLIDER_INT("Start Spin", StartSpin, -180, 180);
                        SLIDER_INT("Start Spin Variation", StartSpinVar, 0, 180);
                        COLOR_EDIT("Start Color", StartColorR, StartColorG, StartColorB, StartColorA);
                        SLIDER_FLOAT("Start Color Variation R", StartColorVarR, 0.0f, 1.0f, "%.2f");
                        SLIDER_FLOAT("Start Color Variation G", StartColorVarG, 0.0f, 1.0f, "%.2f");
                        SLIDER_FLOAT("Start Color Variation B", StartColorVarB, 0.0f, 1.0f, "%.2f");
                        SLIDER_FLOAT("Start Color Variation A", StartColorVarA, 0.0f, 1.0f, "%.2f");
                        SLIDER_INT("End Size", EndSize, 0, 100);
                        SLIDER_INT("End Size Variation", EndSizeVar, 0, 100);
                        SLIDER_INT("End Spin", EndSpin, -180, 180);
                        SLIDER_INT("End Spin Variation", EndSpinVar, 0, 180);
                        COLOR_EDIT("End Color", EndColorR, EndColorG, EndColorB, EndColorA);
                        SLIDER_FLOAT("End Color Variation R", EndColorVarR, 0.0f, 1.0f, "%.2f");
                        SLIDER_FLOAT("End Color Variation G", EndColorVarG, 0.0f, 1.0f, "%.2f");
                        SLIDER_FLOAT("End Color Variation B", EndColorVarB, 0.0f, 1.0f, "%.2f");
                        SLIDER_FLOAT("End Color Variation A", EndColorVarA, 0.0f, 1.0f, "%.2f");
                    }
                    else if (section == 2) {
                        // Extra Parameters
                        SLIDER_FLOAT("Fade In Time", FadeInTime, 0.0f, 10.0f, "%.2f");
                        SLIDER_FLOAT("Fade In Time Variation", FadeInTimeVar, 0.0f, 10.0f, "%.2f");
                        SLIDER_FLOAT("Fade Out Time", FadeOutTime, 0.0f, 10.0f, "%.2f");
                        SLIDER_FLOAT("Fade Out Time Variation", FadeOutTimeVar, 0.0f, 10.0f, "%.2f");
                        SLIDER_INT("Start Radius", StartRadius, 0, 100);
                        SLIDER_INT("Start Radius Variation", StartRadiusVar, 0, 100);
                        SLIDER_INT("End Radius", EndRadius, 0, 100);
                        SLIDER_INT("End Radius Variation", EndRadiusVar, 0, 100);
                        SLIDER_INT("Rotation Per Second", RotatePerSecond, -100, 100);
                        SLIDER_INT("Rotation Variation", RotatePerSecondVar, 0, 100);
                        SLIDER_INT("Emitter Mode (Gravity/Radius)", EmitterMode, 0, 1);
                        SLIDER_INT("Position Type (Relative/Grouped)", PositionType, 0, 1);
                        ImGui::Checkbox("Blend Additive", &particleData->isBlendAdditive) upd;
                        ImGui::Checkbox("Start Spin Equal To End Spin", &particleData->startSpinEqualToEndSpin) upd;
                        ImGui::Checkbox("Rotation Is Direction", &particleData->rotationIsDir) upd;
                        ImGui::Checkbox("Dynamic Rotation Is Direction", &particleData->dynamicRotationIsDir) upd;

                        ImGui::Checkbox("Uniform Color Mode", &particleData->uniformColorMode) upd;
                        SLIDER_FLOAT("Friction Position", frictionPos, 0.0f, 10.0f, "%.2f");
                        SLIDER_FLOAT("Friction Position Variation", frictionPosVar, 0.0f, 10.0f, "%.2f");
                        SLIDER_FLOAT("Respawn", respawn, 0.0f, 10.0f, "%.2f");
                        SLIDER_FLOAT("Respawn Variation", respawnVar, 0.0f, 10.0f, "%.2f");
                        ImGui::Checkbox("Order Sensitive", &particleData->orderSensitive) upd;
                        ImGui::Checkbox("Start Size Equal To End Size", &particleData->startSizeEqualToEndSize) upd;
                        ImGui::Checkbox("Start Radius Equal To End Radius", &particleData->startRadiusEqualToEndRadius) upd;
                        ImGui::Checkbox("Start RGB Var Sync", &particleData->startRGBVarSync) upd;
                        ImGui::Checkbox("End RGB Var Sync", &particleData->endRGBVarSync) upd;
                        SLIDER_FLOAT("Friction Size", frictionSize, 0.0f, 10.0f, "%.2f");
                        SLIDER_FLOAT("Friction Size Variation", frictionSizeVar, 0.0f, 10.0f, "%.2f");
                        SLIDER_FLOAT("Friction Rotation", frictionRot, 0.0f, 10.0f, "%.2f");
                        SLIDER_FLOAT("Friction Rotation Variation", frictionRotVar, 0.0f, 10.0f, "%.2f");

                        ImGui::InputInt("Custom Particle Index", &particleData->customParticleIdx);
                        if (ImGui::IsItemEdited() || ImGui::IsItemDeactivatedAfterEdit()) {
                            particleData->sFrame = fmt::format("particle_{:02d}_001.png", particleData->customParticleIdx);
                            GameToolbox::particleFromStruct(*particleData, m_particle, 0);
                        }

                        auto particleData_sFrame = std::string(particleData->sFrame.data());
                        ImGui::InputText("Sprite Frame (Not Being Saved!)", &particleData_sFrame);
                        if (ImGui::IsItemEdited() || ImGui::IsItemDeactivatedAfterEdit()) {
                            particleData->sFrame = particleData_sFrame;
                            std::regex pattern(R"(particle_(\d+)_001\.png)");
                            std::smatch match;
                            auto fuckyou = std::string(particleData->sFrame.data());
                            if (std::regex_match(fuckyou, match, pattern)) {
                                particleData->customParticleIdx = std::stoi(match[1]);
                            }

                            GameToolbox::particleFromStruct(*particleData, m_particle, 0);
                        }

                    }
                    };

#endif

                ImGui::Begin("Params Setup", &m_bVisible, ImGuiWindowFlags_AlwaysAutoResize);
                ImGui::SetWindowFontScale(1.65f);

                auto str_ps = std::string(ps.data());
                if (ImGui::InputText("##ps-input", &str_ps)) {
                    ps = str_ps.data();
                    GameToolbox::particleFromString(ps, m_particle, 0);
                    GameToolbox::particleStringToStruct(ps, *particleData);
                }
                if (ImGui::SameLine(); ImGui::Button("Reset")) m_particle->resetSystem();
                if (ImGui::SameLine(); ImGui::Button("Stop")) m_particle->stopSystem();
                if (ImGui::SameLine(); ImGui::Button("Resume")) m_particle->resumeSystem();

                if (ImGui::BeginTabBar("Tabs", ImGuiTabBarFlags_None)) {
                    if (ImGui::BeginTabItem("Motion")) {
                        renderParticleDataSets(0);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Visual")) {
                        renderParticleDataSets(1);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Extra")) {
                        renderParticleDataSets(2);
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
                ImGui::ScrollWhenDragging();
                ImGui::End();

                if (!this->isVisible()) return this->onBtn1(this);

#undef SLIDER_INT
#undef SLIDER_FLOAT
#undef COLOR_EDIT
#undef upd
            }
        ));

        auto save = findFirstChildRecursive<CCMenuItemSpriteExtra>(this, [](CCNode*) {return true; });
        save->setPosition(CCPointMake(80.f, 23.f));

        auto saveImage = ButtonSprite::create(
            "Apply", 80.000f, 1, "bigFont.fnt", "GJ_button_03.png", 0, 0.7f
        );
        saveImage->setScale(0.7f);
        save->setSprite(saveImage);

        return true;
    }

    static ParticlePopup* create(std::string const& text) {
        auto ret = new ParticlePopup();
        if (ret->initAnchored(160.f, 180.f, text, "GJ_square05.png")) {
            ret->setTitle("Particle Preview");
            ret->m_noElasticity = 1;
            ret->autorelease();
            return ret;
        }

        delete ret;
        return nullptr;
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
                                        auto particle_popup = ParticlePopup::create(data);
                                        particle_popup->show();
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
                                        //handleTouchPriority(popup, 1);
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

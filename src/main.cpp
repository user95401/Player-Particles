
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
        this->m_bgSprite->setColor({ 42, 42, 42 });

        m_particle = CCParticleSystemQuad::create();
        GameToolbox::particleFromString(value.c_str(), m_particle, 0);
        m_mainLayer->addChildAtPosition(m_particle, Anchor::Center);

        particleData = new ParticleStruct;
        GameToolbox::particleStringToStruct(value.c_str(), *particleData);
        ps = GameToolbox::saveParticleToString(m_particle).c_str();

        auto save = findFirstChildRecursive<CCMenuItemSpriteExtra>(this, [](CCNode*) {return true; });

        auto size = this->getContentSize();
        Ref renderTexFlipped = CCRenderTexture::create(size.width, size.height);
        Ref renderTex = CCRenderTexture::create(size.width, size.height);
        this->runAction(CCRepeatForever::create(CCSpawn::create(CallFuncExt::create(
            [this, renderTexFlipped, renderTex] {
                this->m_mainLayer->setVisible(1);
                renderTexFlipped->beginWithClear(0, 0, 0, 0);
                this->m_mainLayer->visit();
                renderTexFlipped->end();
                this->m_mainLayer->setVisible(0);

                renderTexFlipped->getSprite()->setScaleY(1.f);
                renderTexFlipped->getSprite()->setAnchorPoint(CCPointZero);
                renderTex->beginWithClear(0, 0, 0, 0);
                renderTexFlipped->getSprite()->visit();
                renderTex->end();
            }
        ), nullptr)));
        this->m_mainLayer->setPosition({ 160.f, 150.f });
        this->m_mainLayer->setScale(4.f);
        this->m_mainLayer->setVisible(0);

        ImGuiCocosExt::m_drawings[this] = (
            [this, save, renderTex]() {

#if 1

#define upd ; if (ImGui::IsItemEdited() || ImGui::IsItemDeactivatedAfterEdit()) { GameToolbox::particleFromStruct(*particleData, m_particle, 0); ps = GameToolbox::saveParticleToString(m_particle);  };
#define iw1 (160.f * ImGui::GetIO().FontGlobalScale)
#define iw2 (360.f * ImGui::GetIO().FontGlobalScale)
#define SLIDER_INT(label, var, min, max) \
    ImGui::BeginGroup(); \
     \
    ImGui::SetNextItemWidth(0.f); \
    ImGui::BeginGroup(); \
    ImGui::InputFloat("##" label "InputDummy", &particleData->Life); \
    ImGui::EndGroup(); \
     \
    ImGui::SameLine(); \
     \
    ImGui::BeginGroup(); \
    ImGui::Text(label); \
    ImGui::SetNextItemWidth(iw1); \
    ImGui::InputInt("##" label "Input", &particleData->var) upd; \
    ImGui::SameLine(); \
    ImGui::SetNextItemWidth(iw2); \
    ImGui::SliderInt("##" label "Slider", &particleData->var, min, max, "%d", ImGuiSliderFlags_AlwaysClamp) upd; \
    ImGui::EndGroup(); \
     \
    ImGui::EndGroup(); \

#define SLIDER_FLOAT(label, var, min, max, format) \
    ImGui::BeginGroup(); \
     \
    ImGui::SetNextItemWidth(0.f); \
    ImGui::BeginGroup(); \
    ImGui::InputFloat("##" label "InputDummy", &particleData->Life); \
    ImGui::EndGroup(); \
     \
    ImGui::SameLine(); \
     \
    ImGui::BeginGroup(); \
    ImGui::Text(label); \
    ImGui::SetNextItemWidth(iw1); \
    ImGui::InputFloat("##" label "Input", &particleData->var, 0.1f, 1.0f, format) upd; \
    ImGui::SameLine(); \
    ImGui::SetNextItemWidth(iw2); \
    ImGui::SliderFloat("##" label "Slider", &particleData->var, min, max, format, ImGuiSliderFlags_AlwaysClamp) upd; \
    ImGui::EndGroup(); \
     \
    ImGui::EndGroup(); \


#define COLOR_EDIT(label, r, g, b, a) \
    ImGui::BeginGroup(); \
     \
    ImGui::SetNextItemWidth(0.001f); \
    ImGui::BeginGroup(); \
    ImGui::InputFloat("##" label "InputDummy", &particleData->Life); \
    ImGui::EndGroup(); \
     \
    ImGui::SameLine(); \
     \
    ImGui::SetNextItemWidth(iw2); \
    {float color[4] = { particleData->r, particleData->g, particleData->b, particleData->a }; \
    if (ImGui::ColorEdit4(label, color, ImGuiColorEditFlags_None)) { \
        particleData->r = color[0]; \
        particleData->g = color[1]; \
        particleData->b = color[2]; \
        particleData->a = color[3]; \
        GameToolbox::particleFromStruct(*particleData, m_particle, 0); \
    }} \
                \
    ImGui::EndGroup(); \

                auto renderParticleDataSets = [this](int section) {
                    ImGui::BeginChild(fmt::format("tabsub{}", section).c_str());
                    if (section == 0) {
                        // Motion Parameters
                        SLIDER_INT("Max Particles", TotalParticles, 0, 1000);
                        SLIDER_FLOAT("Duration", Duration, -10.0f, 10.0f, "%.2f");

                        ImGui::BeginGroup();
                        SLIDER_FLOAT("Lifetime", Life, 0.0f, 10.0f, "%.2f");
                        ImGui::SameLine();
                        SLIDER_FLOAT("Lifetime Variation", LifeVar, 0.0f, 10.0f, "%.2f");
                        ImGui::EndGroup();

                        SLIDER_INT("Emission", EmissionRate, 0, 1000);

                        ImGui::BeginGroup();
                        SLIDER_INT("Angle", Angle, -180, 180);
                        ImGui::SameLine();
                        SLIDER_INT("Angle Variation", AngleVar, 0, 180);
                        ImGui::EndGroup();

                        ImGui::BeginGroup();
                        SLIDER_INT("Speed", Speed, 0, 100);
                        ImGui::SameLine();
                        SLIDER_INT("Speed Variation", SpeedVar, 0, 100);
                        ImGui::EndGroup();

                        ImGui::BeginGroup();
                        SLIDER_INT("Position Variation X", PosVarX, 0, 100);
                        ImGui::SameLine();
                        SLIDER_INT("Position Variation Y", PosVarY, 0, 100);
                        ImGui::EndGroup();

                        ImGui::BeginGroup();
                        SLIDER_INT("Gravity X", GravityX, -100, 100);
                        ImGui::SameLine();
                        SLIDER_INT("Gravity Y", GravityY, -100, 100);
                        ImGui::EndGroup();

                        ImGui::BeginGroup();
                        SLIDER_INT("Radial Acceleration", RadialAccel, -100, 100);
                        ImGui::SameLine();
                        SLIDER_INT("Radial Variation", RadialAccelVar, 0, 100);
                        ImGui::EndGroup();

                        ImGui::BeginGroup();
                        SLIDER_INT("Tangential Acceleration", TangentialAccel, -100, 100);
                        ImGui::SameLine();
                        SLIDER_INT("Tangential Variation", TangentialAccelVar, 0, 100);
                        ImGui::EndGroup();
                    }
                    else if (section == 1) {
                        // Visual Parameters
                        ImGui::BeginGroup();
                        SLIDER_INT("Start Size", StartSize, 0, 100);
                        ImGui::SameLine();
                        SLIDER_INT("Start Size Variation", StartSizeVar, 0, 100);
                        ImGui::EndGroup();

                        ImGui::BeginGroup();
                        SLIDER_INT("Start Spin", StartSpin, -180, 180);
                        ImGui::SameLine();
                        SLIDER_INT("Start Spin Variation", StartSpinVar, 0, 180);
                        ImGui::EndGroup();

                        COLOR_EDIT("Start Color", StartColorR, StartColorG, StartColorB, StartColorA);
                        COLOR_EDIT("Start Color Variation", StartColorVarR, StartColorVarG, StartColorVarB, StartColorVarA);

                        ImGui::BeginGroup();
                        SLIDER_INT("End Size", EndSize, 0, 100);
                        ImGui::SameLine();
                        SLIDER_INT("End Size Variation", EndSizeVar, 0, 100);
                        ImGui::EndGroup();

                        ImGui::BeginGroup();
                        SLIDER_INT("End Spin", EndSpin, -180, 180);
                        ImGui::SameLine();
                        SLIDER_INT("End Spin Variation", EndSpinVar, 0, 180);
                        ImGui::EndGroup();

                        COLOR_EDIT("End Color", EndColorR, EndColorG, EndColorB, EndColorA);
                        COLOR_EDIT("End Color Variation", EndColorVarR, EndColorVarG, EndColorVarB, EndColorVarA);
                    }
                    else if (section == 2) {
                        // Extra Parameters
                        ImGui::BeginGroup();
                        SLIDER_FLOAT("Fade In Time", FadeInTime, 0.0f, 10.0f, "%.2f");
                        ImGui::SameLine();
                        SLIDER_FLOAT("Fade In Time Variation", FadeInTimeVar, 0.0f, 10.0f, "%.2f");
                        ImGui::EndGroup();

                        ImGui::BeginGroup();
                        SLIDER_FLOAT("Fade Out Time", FadeOutTime, 0.0f, 10.0f, "%.2f");
                        ImGui::SameLine();
                        SLIDER_FLOAT("Fade Out Time Variation", FadeOutTimeVar, 0.0f, 10.0f, "%.2f");
                        ImGui::EndGroup();

                        ImGui::BeginGroup();
                        SLIDER_INT("Start Radius", StartRadius, 0, 100);
                        ImGui::SameLine();
                        SLIDER_INT("Start Radius Variation", StartRadiusVar, 0, 100);
                        ImGui::EndGroup();

                        ImGui::BeginGroup();
                        SLIDER_INT("End Radius", EndRadius, 0, 100);
                        ImGui::SameLine();
                        SLIDER_INT("End Radius Variation", EndRadiusVar, 0, 100);
                        ImGui::EndGroup();

                        ImGui::BeginGroup();
                        SLIDER_INT("Rotation Per Second", RotatePerSecond, -100, 100);
                        ImGui::SameLine();
                        SLIDER_INT("Rotation Variation", RotatePerSecondVar, 0, 100);
                        ImGui::EndGroup();

                        SLIDER_INT("Emitter Mode (Gravity/Radius)", EmitterMode, 0, 1);
                        SLIDER_INT("Position Type (Relative/Grouped)", PositionType, 0, 1);

                        ImGui::Checkbox("Blend Additive", &particleData->isBlendAdditive) upd;
                        ImGui::Checkbox("Start Spin Equal To End Spin", &particleData->startSpinEqualToEndSpin) upd;
                        ImGui::Checkbox("Rotation Is Direction", &particleData->rotationIsDir) upd;
                        ImGui::Checkbox("Dynamic Rotation Is Direction", &particleData->dynamicRotationIsDir) upd;

                        ImGui::Checkbox("Uniform Color Mode", &particleData->uniformColorMode) upd;

                        ImGui::BeginGroup();
                        SLIDER_FLOAT("Friction Position", frictionPos, 0.0f, 10.0f, "%.2f");
                        ImGui::SameLine();
                        SLIDER_FLOAT("Friction Position Variation", frictionPosVar, 0.0f, 10.0f, "%.2f");
                        ImGui::EndGroup();

                        ImGui::BeginGroup();
                        SLIDER_FLOAT("Respawn", respawn, 0.0f, 10.0f, "%.2f");
                        ImGui::SameLine();
                        SLIDER_FLOAT("Respawn Variation", respawnVar, 0.0f, 10.0f, "%.2f");
                        ImGui::EndGroup();

                        ImGui::Checkbox("Order Sensitive", &particleData->orderSensitive) upd;
                        ImGui::Checkbox("Start Size Equal To End Size", &particleData->startSizeEqualToEndSize) upd;
                        ImGui::Checkbox("Start Radius Equal To End Radius", &particleData->startRadiusEqualToEndRadius) upd;
                        ImGui::Checkbox("Start RGB Var Sync", &particleData->startRGBVarSync) upd;
                        ImGui::Checkbox("End RGB Var Sync", &particleData->endRGBVarSync) upd;

                        ImGui::BeginGroup();
                        SLIDER_FLOAT("Friction Size", frictionSize, 0.0f, 10.0f, "%.2f");
                        ImGui::SameLine();
                        SLIDER_FLOAT("Friction Size Variation", frictionSizeVar, 0.0f, 10.0f, "%.2f");
                        ImGui::EndGroup();

                        ImGui::BeginGroup();
                        SLIDER_FLOAT("Friction Rotation", frictionRot, 0.0f, 10.0f, "%.2f");
                        ImGui::SameLine();
                        SLIDER_FLOAT("Friction Rotation Variation", frictionRotVar, 0.0f, 10.0f, "%.2f");
                        ImGui::EndGroup();

                        ImGui::InputInt("Custom Particle Index", &particleData->customParticleIdx);
                        if (ImGui::IsItemEdited() || ImGui::IsItemDeactivatedAfterEdit()) {
                            particleData->sFrame = fmt::format("particle_{:02d}_001.png", particleData->customParticleIdx);
                            GameToolbox::particleFromStruct(*particleData, m_particle, 0);
                        }

                        auto particleData_sFrame = std::string(particleData->sFrame.c_str());
                        ImGui::InputText("Sprite Frame (Not Being Saved!)", &particleData_sFrame);
                        if (ImGui::IsItemEdited() || ImGui::IsItemDeactivatedAfterEdit()) {
                            particleData->sFrame = particleData_sFrame;
                            std::regex pattern(R"(particle_(\d+)_001\.png)");
                            std::smatch match;
                            auto fuckyou = std::string(particleData->sFrame.c_str());
                            if (std::regex_match(fuckyou, match, pattern)) {
                                particleData->customParticleIdx = std::stoi(match[1]);
                            }

                            GameToolbox::particleFromStruct(*particleData, m_particle, 0);
                        }

                    }
                    ImGui::EndChild();
                    };

#endif

                ImGui::Begin("Params Setup", &m_bVisible, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse);
                ImGui::SetWindowFontScale(1.65f);

                auto offst = ImVec2(220, 120);
                ImGui::SetWindowSize(ImGui::GetMainViewport()->Size - offst);
                ImGui::SetWindowPos(offst / 2);

                auto str_ps = std::string(ps.c_str());
                auto psinpEdited = ImGui::InputText("##ps-input", &str_ps);
                if (psinpEdited) {
                    ps = str_ps.c_str();
                    GameToolbox::particleFromString(ps, m_particle, 0);
                    GameToolbox::particleStringToStruct(ps, *particleData);
                }
                if (ImGui::SameLine(); ImGui::Button("Reset")) m_particle->resetSystem();
                if (ImGui::SameLine(); ImGui::Button("Stop")) m_particle->stopSystem();
                if (ImGui::SameLine(); ImGui::Button("Resume")) m_particle->resumeSystem();

                if (ImGui::BeginChild("Params Setup Sub")) {

                    if (ImGui::BeginChild("ShowcaseChild", ImVec2(320, 0))) {
                        
                        if (this and this->m_bgSprite) {
                            auto rgb = this->m_bgSprite->getColor();
                            float color[3] = { rgb.r / 255.f, rgb.g / 255.f, rgb.b / 255.f };
                            if (ImGui::ColorEdit3("", color, ImGuiColorEditFlags_None)) {
                                this->m_bgSprite->setColor({
                                    static_cast<GLubyte>(color[0] * 255),
                                    static_cast<GLubyte>(color[1] * 255),
                                    static_cast<GLubyte>(color[2] * 255)
                                    });
                            }
                        }
                        if (renderTex and renderTex->m_pTexture) {
                            GLuint glTexID = renderTex->m_pTexture->getName();
                            auto sz = renderTex->m_pTexture->getContentSize();
                            if (glTexID) ImGui::Image(
                                (ImTextureID)(intptr_t)glTexID,
                                ImVec2(sz.width, sz.height) * ImGui::GetIO().FontGlobalScale
                            );
                        };
                        if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenBlockedByActiveItem)) {
                            ImGui::SetItemAllowOverlap();
                            ImGui::SetActiveID(ImGui::GetItemID(), ImGui::GetCurrentWindow());
                            auto mdt = ImGui::GetIO().MouseDelta / m_mainLayer->getScale();
                            if (ImGui::GetIO().MouseDownDuration[0] > 0.1) m_particle->setPosition(
                                m_particle->getPositionX() + mdt.x,
                                m_particle->getPositionY() + -mdt.y
                            );
                            if (fabs(ImGui::GetIO().MouseWheel) > 0.0) m_particle->setScale(
                                m_particle->getScale() + (ImGui::GetIO().MouseWheel / 10)
                            );
                            if (ImGui::IsMouseDoubleClicked(0)) m_particle->resetSystem();
                        };
                        ImGui::SetWindowFontScale(2.0f);
                        if (ImGui::Button("Save", ImVec2(-FLT_MIN, 0)))
                            if (save) save->activate();
                        ImGui::BeginDisabled();
                        std::string ctrl_inf = 
                            """" "Preview area controls:"
                            "\n" "- Hold and move to drag particle"
                            "\n" "- Scroll mouse wheel to change scale"
                            "\n" "- Double click to reset system (restart)"
                            ;
                        ImGui::TextWrapped("\n\n%s", ctrl_inf.c_str());
                        ImGui::SetItemTooltip("%s", ctrl_inf.c_str());
                        ImGui::EndDisabled();
                    }
                    ImGui::EndChild();

                    ImGui::SameLine();

                    ImGui::BeginChild("TabsChild", ImVec2(ImGui::GetWindowSize().x - 325, 0));
                    ImGui::SetWindowFontScale(2.f);
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
                    ImGui::EndChild();

                };
                ImGui::EndChild();

                ImGui::End();

                //use of this->m_bVisible in ImGui::Begin
                if (!this->isVisible()) return this->onBtn1(this);

#undef SLIDER_INT
#undef SLIDER_FLOAT
#undef COLOR_EDIT
#undef iw1
#undef iw2
#undef upd
            }
        );

        return true;
    }

    static ParticlePopup* create(std::string const& text) {
        auto ret = new ParticlePopup();
        if (ret->initAnchored(160.f, 180.f, text, "game_bg_13_001.png")) {
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
                                    restartParticleImage, [popup, menu](CCNode*) {
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
                                    copyParticleStrLabel, [menu, restartParticle, resetFileParticle, effectName, id](CCNode*) {
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
                                    useParticleImage, [menu, restartParticle, resetFileParticle, effectName](CCNode*) {
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
                                    deleteParticleImage, [menu, scroll, resetFileParticle, effectName, id](CCNode*) {

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
                                    editParticleImage, [popup, menu, restartParticle, resetFileParticle, effectName, id](CCNode*) {
                                        auto data = std::string(findDataNode(menu, "data")->getString());
                                        auto particle_popup = ParticlePopup::create(data);
                                        particle_popup->show();
                                        auto okBtn = cocos::findFirstChildRecursive<CCMenuItemSpriteExtra>(particle_popup, [](CCNode*) {return true; });
                                        CCMenuItemExt::assignCallback<CCMenuItemSpriteExtra>(
                                            okBtn, [particle_popup, popup, menu, restartParticle, resetFileParticle, effectName, id](CCNode*) {
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
                            createnewImage, [popup, effectName, scroll, createParticleCard](CCNode*) {

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
                            createFromStringImage, [popup, inputParticleStr, effectName, scroll, createParticleCard](CCNode*) {

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
    static void runEnforcer(CCParticleSystemQuad * particle, std::string initstr) {

        if (particle == nullptr) return; //fuck this
        if (!typeinfo_cast<CCParticleSystemQuad*>(particle)) return; //hi

        auto fGrv = getMod()->getSettingValue<bool>("Force Initial Gravity");
        auto fCol = getMod()->getSettingValue<bool>("Force Initial Colors");
        auto fPos = getMod()->getSettingValue<bool>("Force Initial Position Var");
        auto fPtp = getMod()->getSettingValue<bool>("Force Initial Position Type");

        if (!fGrv and !fCol and !fPos and !fPtp) return;

        auto org = GameToolbox::particleFromString(
            initstr, CCParticleSystemQuad::create(), 0
        );
        particle->setUserObject(org);

        particle->runAction(CCRepeatForever::create(CCSpawn::create(CallFuncExt::create(
            [particle, org, fGrv, fCol, fPos, fPtp] {
                if (particle == nullptr) return; //fuck this
                if (!typeinfo_cast<CCParticleSystemQuad*>(particle)) return; //hi

                if (fGrv) {
                    particle->setGravity(org->getGravity());
                }
                if (fCol) {
                    particle->setStartColor(org->getStartColor());
                    particle->setStartColorVar(org->getStartColorVar());
                    particle->setEndColor(org->getEndColor());
                    particle->setEndColorVar(org->getEndColorVar());
                }
                if (fPos) {
                    particle->setPosVar(org->getPosVar());
                }
                if (fPtp) {
                    particle->setPositionType(org->getPositionType());
                }
            }
        ), nullptr)));

    }
    static void updateImage(CCParticleSystemQuad * particle, const char* effectFile) {
        auto effectName = fs::path(effectFile).filename().replace_extension("").string();
        auto particleImageFileName = effectName + ".png";
        if (fileExistsInSearchPaths(particleImageFileName.c_str())) {
            auto sprite = CCSprite::create(particleImageFileName.c_str());
            if (sprite) particle->setDisplayFrame(sprite->displayFrame());
        }
    }
    $override static CCParticleSystemQuad* create(const char* file, bool idk) {

        if (string::endsWith(file, "!original")) return CCParticleSystemQuad::create(
            string::replace(file, "!original", "").c_str(), idk
        );

        auto orgone = CCParticleSystemQuad::create(file, idk);

        auto effectName = fs::path(file).filename().replace_extension("").string();

        auto stringDataFileName = effectName + ".txt";
        if (fileExistsInSearchPaths(stringDataFileName.c_str())) {
            auto read = file::readString(stringDataFileName.c_str());
            auto rtn = read.isOk() ? GameToolbox::particleFromString(
                read.unwrapOrDefault(), CCParticleSystemQuad::create(), idk
            ) : orgone;//!isOk read
            if (read.err()) log::error("error reading \"{}\", {}", stringDataFileName, read.err());
            runEnforcer(rtn, read.unwrapOrDefault());
            updateImage(rtn, file);
            return rtn;
        }

        auto saves = Mod::get()->getSaveContainer();
        if (saves.contains(effectName)) {
            auto saves_effect = saves.get(effectName);
            auto str = saves_effect.isOk() ? saves_effect.unwrap().asString().unwrapOr("") : defaultEffect(effectName);
            auto rtn = GameToolbox::particleFromString(str, CCParticleSystemQuad::create(), idk);
            runEnforcer(rtn, str);
            updateImage(rtn, file);
            return rtn;
        }

        updateImage(orgone, file);

        return orgone;
    }
};

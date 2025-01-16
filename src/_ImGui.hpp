#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <cocos2d.h>
#include <misc/cpp/imgui_stdlib.h>

using namespace cocos2d;

static std::ostream& operator<<(std::ostream& stream, ImVec2 const& vec) {
    return stream << vec.x << ", " << vec.y;
}

static std::ostream& operator<<(std::ostream& stream, ImVec4 const& vec) {
    return stream << vec.x << ", " << vec.y << ", " << vec.z << ", " << vec.w;
}

static ImVec2 operator+(ImVec2 const& a, ImVec2 const& b) {
    return {
        a.x + b.x,
        a.y + b.y
    };
}

static ImVec2 operator-(ImVec2 const& a, ImVec2 const& b) {
    return {
        a.x - b.x,
        a.y - b.y
    };
}

static ImVec2 operator-(ImVec2 const& a) {
    return { -a.x, -a.y };
}

static ImVec2& operator-=(ImVec2& a, ImVec2 const& b) {
    a.x -= b.x;
    a.y -= b.y;
    return a;
}

static ImVec2 operator/(ImVec2 const& a, ImVec2 const& b) {
    return {
        a.x / b.x,
        a.y / b.y
    };
}

static ImVec2 operator/(ImVec2 const& a, int b) {
    return {
        a.x / b,
        a.y / b
    };
}

static ImVec2 operator/(ImVec2 const& a, float b) {
    return {
        a.x / b,
        a.y / b
    };
}

static bool operator!=(ImVec2 const& a, ImVec2 const& b) {
    return a.x != b.x || a.y != b.y;
}

static ImVec2 operator*(ImVec2 const& v1, float multi) {
    return { v1.x * multi, v1.y * multi };
}

static ImVec2 toVec2(CCPoint const& a) {
    const auto size = ImGui::GetMainViewport()->Size;
    const auto winSize = CCDirector::get()->getWinSize();
    return {
        a.x / winSize.width * size.x,
        (1.f - a.y / winSize.height) * size.y
    };
}

static ImVec2 toVec2(CCSize const& a) {
    const auto size = ImGui::GetMainViewport()->Size;
    const auto winSize = CCDirector::get()->getWinSize();
    return {
        a.width / winSize.width * size.x,
        -a.height / winSize.height * size.y
    };
}

static CCPoint toCocos(const ImVec2& pos) {
    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    const auto size = ImGui::GetMainViewport()->Size;
    return CCPointMake(
        pos.x / size.x * winSize.width,
        (1.f - pos.y / size.y) * winSize.height
    );
};

namespace ImGui {
    inline void ScrollWhenDragging() {
        auto mouse_dt = ImGui::GetIO().MouseDelta;
        ImVec2 delta = ImGui::GetIO().MouseDownDuration[0] > 0.1 ? ImVec2(mouse_dt.x * -1, mouse_dt.y * -1) : ImVec2(0, 0);
        ImGuiContext& g = *ImGui::GetCurrentContext();
        ImGuiWindow* window = g.CurrentWindow;
        if (!window) return;
        //bool held = g.IO.MouseDown[0];
        bool hovered = false;
        bool held = false;
        ImGuiID id = window->GetID("##scrolldraggingoverlay");
        ImGui::KeepAliveID(id);
        ImGuiButtonFlags button_flags = ImGuiButtonFlags_MouseButtonLeft;
        if (g.HoveredId == 0) // If nothing hovered so far in the frame (not same as IsAnyItemHovered()!)
            ImGui::ButtonBehavior(window->Rect(), id, &hovered, &held, button_flags);
        if (held && fabs(delta.x) >= 0.1f)
            ImGui::SetScrollX(window, window->Scroll.x + delta.x);
        if (held && fabs(delta.y) >= 0.1f)
            ImGui::SetScrollY(window, window->Scroll.y + delta.y);
    }
}

class ImGuiDrawNode : public CCNode {
public:
    static inline std::set<ImGuiDrawNode*> DrawNodes;
    
    std::function<void()> m_drawFunc;

    static ImGuiDrawNode* create(std::function<void()> drawFunc) {
        ImGuiDrawNode* pRet = new ImGuiDrawNode();
        if (pRet && pRet->init()) {
            pRet->autorelease();
            pRet->m_drawFunc = (drawFunc);
            DrawNodes.insert(pRet);
            return pRet;
        }
        else {
            delete pRet; pRet = 0; return 0;
        }
    };

};

#include <imgui-cocos.hpp>
#include <Geode/modify/CCDirector.hpp>
class $modify(ImGuiMain, CCDirector) {
    $override void runWithScene(CCScene * pScene) {
        CCDirector::runWithScene(pScene);
        ImGuiCocos::get().setup(
            [] {
                auto& io = ImGui::GetIO();
                io.ConfigWindowsResizeFromEdges = true;
                io.FontAllowUserScaling = true;
                io.ConfigDragClickToInputText = true;
                io.ConfigScrollbarScrollByPage = true;
                io.MouseDoubleClickTime = 1.0f;
                io.ConfigFlags |= ImGuiConfigFlags_IsTouchScreen;
                GEODE_MACOS(io.ConfigMacOSXBehaviors = true);

                auto& style = ImGui::GetStyle();
                style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
                style.WindowBorderSize = 2.5;
                style.ColorButtonPosition = ImGuiDir_Left;
                style.FramePadding = { 6.f, 6.f };
                style.ScrollbarSize = 32.f;
                style.GrabMinSize = 30.f;
                style.TouchExtraPadding = { 5.f, 3.f };

                ImVec4* colors = ImGui::GetStyle().Colors;
                colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.34f);
            }
        ).draw(
            [] {

                auto* kb = CCDirector::sharedDirector()->getKeyboardDispatcher();
                ImGui::GetIO().KeyAlt = kb->getAltKeyPressed() || kb->getCommandKeyPressed(); // look
                ImGui::GetIO().KeyCtrl = kb->getControlKeyPressed();
                ImGui::GetIO().KeyShift = kb->getShiftKeyPressed();

                for (auto aDrawNode : ImGuiDrawNode::DrawNodes)
                    if (aDrawNode and aDrawNode->isRunning() and aDrawNode->m_drawFunc) aDrawNode->m_drawFunc();
                    else {
                        ImGuiDrawNode::DrawNodes.erase(aDrawNode);
                        aDrawNode->removeFromParent();
                    }

            }
        );
    };
};
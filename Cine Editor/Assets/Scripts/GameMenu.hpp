#pragma once
#include "include/ScriptCore.hpp"
#include "ImguiElement.hpp"
#include "imgui.h"

class GameMenu : public UI::ImGuiRenderable
{
public:
    GameMenu()
    {
        Display = false;
    }

    void Update(Timestep ts) override
    {
        if(Input::IsKeyDown(Key::Escape))
        {
            Display = !Display;
        }
    }

    void Draw() override
    {
        if(!Display) return;

        Focused = ImGui::IsWindowFocused();
        Hovered = ImGui::IsWindowHovered();

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.3f, 0.3f, 0.3f, 0.7f));
        ImGui::Begin("Menu");

        UI::ImGui::Button("This is a button", UI::DefaultMargin, UI::DefaultSize, UI::Align::AlignCenterBoth);
        UI::ImGui::Button("This is text", UI::DefaultMargin, UI::DefaultSize, UI::Align::AlignCenterBoth);
        UI::ImGui::Button("This is GameMenu derrived from ImGuiElement!", UI::DefaultMargin, UI::DefaultSize, UI::Align::AlignCenterBoth);

        Focused = ImGui::IsWindowFocused();
        Hovered = ImGui::IsWindowHovered();
        ImGui::End();

        ImGui::PopStyleColor();
    }
};
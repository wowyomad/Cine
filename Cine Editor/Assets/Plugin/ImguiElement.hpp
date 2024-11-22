#pragma once
#include <imgui.h>

namespace UI
{
    enum Align
    {
        AlignNone = 0,
        AlignCenterHorizontal = 1 << 0,
        AlignCenterVertical = 1 << 1,
        AlignCenterBoth = AlignCenterHorizontal | AlignCenterVertical
    };

    struct Margin
    {
        uint32_t left = 0;
        uint32_t right = 0;
        uint32_t top = 0;
        uint32_t bottom = 0;
    };

    struct Size
    {
        uint32_t width = 0;
        uint32_t height = 0;
    };

    struct Vec2
    {
        uint32_t x = 0;
        uint32_t y = 0;
    };

    static uint32_t FontSize = 30;
    static Margin DefaultMargin = { 10, 10, 10, 10 };
    static Size DefaultSize = { 0, 0 }; // Default is to fit content

    class ImGuiRenderable
    {
    public:
        virtual void Draw() = 0;
        bool Display = true;
    };

    namespace ImGui
    {
        inline bool Button(const std::string& text, Margin margin = DefaultMargin, Size size = DefaultSize, uint32_t align = AlignNone)
        {
            std::string displayedText = text;
            ImVec2 textSize = ::ImGui::CalcTextSize(text.c_str());

            ImVec2 buttonSize = ImVec2(
                (size.width > 0 ? size.width : textSize.x + margin.left + margin.right),
                (size.height > 0 ? size.height : FontSize + margin.top + margin.bottom)
            );

            if (textSize.x > buttonSize.x - margin.left - margin.right)
            {
                size_t len = text.length();
                while (textSize.x > buttonSize.x - margin.left - margin.right && len > 0)
                {
                    len--;
                    displayedText = text.substr(0, len) + "...";
                    textSize = ::ImGui::CalcTextSize(displayedText.c_str());
                }
            }

            ImVec2 parentSize = ::ImGui::GetContentRegionAvail();
            ImVec2 centeredPos = ::ImGui::GetCursorPos();
            if (align & AlignCenterHorizontal)
            {
                centeredPos.x = (parentSize.x - buttonSize.x) * 0.5f;
            }
            if (align & AlignCenterVertical)
            {
                centeredPos.y = (parentSize.y - buttonSize.y) * 0.5f;
            }
            ::ImGui::SetCursorPos(centeredPos);

            return ::ImGui::Button(displayedText.c_str(), buttonSize);
        }

        inline void Text(const std::string& text, Margin margin = DefaultMargin, Size size = DefaultSize, uint32_t align = AlignNone)
        {
            ImGuiIO& io = ::ImGui::GetIO();
            ImFont* font = nullptr;

            for (ImFont* f : io.Fonts->Fonts)
            {
                if (f->ConfigData[0].SizePixels == static_cast<float>(FontSize))
                {
                    font = f;
                    break;
                }
            }

            if (font != nullptr)
            {
                ::ImGui::PushFont(font);
            }

            ImVec2 textSize = ::ImGui::CalcTextSize(text.c_str());
            ImVec2 componentSize = ImVec2(
                (size.width > 0 ? size.width : textSize.x + margin.left + margin.right),
                (size.height > 0 ? size.height : FontSize + margin.top + margin.bottom)
            );

            ImVec2 parentSize = ::ImGui::GetContentRegionAvail();
            ImVec2 centeredPos = ::ImGui::GetCursorPos();
            if (align & AlignCenterHorizontal)
            {
                centeredPos.x = (parentSize.x - componentSize.x) * 0.5f;
            }
            if (align & AlignCenterVertical)
            {
                centeredPos.y = (parentSize.y - componentSize.y) * 0.5f;
            }
            ::ImGui::SetCursorPos(centeredPos);

            ::ImGui::Text("%s", text.c_str());

            if (font != nullptr)
            {
                ::ImGui::PopFont();
            }
        }

        inline void TextBox(const std::string& text, Margin margin = DefaultMargin, Size size = DefaultSize, uint32_t align = AlignNone)
        {
            ImVec2 textSize = ::ImGui::CalcTextSize(text.c_str());
            ImVec2 textBoxSize = ImVec2(
                (size.width > 0 ? size.width : textSize.x + margin.left + margin.right),
                (size.height > 0 ? size.height : FontSize + margin.top + margin.bottom)
            );

            ImVec2 parentSize = ::ImGui::GetContentRegionAvail();
            ImVec2 centeredPos = ::ImGui::GetCursorPos();
            if (align & AlignCenterHorizontal)
            {
                centeredPos.x = (parentSize.x - textBoxSize.x) * 0.5f;
            }
            if (align & AlignCenterVertical)
            {
                centeredPos.y = (parentSize.y - textBoxSize.y) * 0.5f;
            }
            ::ImGui::SetCursorPos(centeredPos);

            ::ImGui::BeginChildFrame(::ImGui::GetID(text.c_str()), textBoxSize);
            ::ImGui::TextWrapped("%s", text.c_str());
            ::ImGui::EndChildFrame();
        }
    }
}

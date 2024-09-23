#include "TestClearColor.hpp"
#include "glash/glash_pch.hpp"
#include "glash/Logger.hpp"
#include "imgui.h"

namespace glash
{
	namespace test
	{
		ClearColorTest::ClearColorTest(Window& window)
			: Test(window), m_ClearColor(0, 0, 0, 1), m_DefaultColor(window.GetClearColor())
		{
			
		}
		ClearColorTest::ClearColorTest(Window& window, const Color& color)
			: Test(window), m_ClearColor(color), m_DefaultColor(window.GetClearColor())
		{

		}
		ClearColorTest::~ClearColorTest()
		{
			m_Window.SetClearColor(m_DefaultColor);
		}

		void ClearColorTest::OnUpdate(float deltaTime)
		{

		}

		void ClearColorTest::OnRender()
		{
			m_Window.SetClearColor(m_ClearColor);
		}

		void ClearColorTest::OnImGuiRender()
		{
			ImGui::ColorPicker4("Color Picker", reinterpret_cast<float*>(& m_ClearColor));
		}

	}
}

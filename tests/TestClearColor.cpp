#include "TestClearColor.hpp"
#include "glash/glash_pch.hpp"
#include "glash/Logger.hpp"
#include "imgui.h"

namespace glash
{
	namespace test
	{
		ClearColorTest::ClearColorTest(Window& window)
			: ClearColorTest(window, color::GRAY)
		{
			
		}
		ClearColorTest::ClearColorTest(Window& window, const Color& color)
			: Test(window), m_ClearColor(color)
		{

		}
		ClearColorTest::~ClearColorTest()
		{

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

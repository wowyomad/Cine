#include "TestClearColor.hpp"
#include "glash/glash_pch.hpp"
#include "glash/Logger.hpp"
#include "imgui.h"

namespace glash
{
	namespace test
	{
		ClearColorTest::ClearColorTest(Window& window, const float color[4])
			: m_Window(window)
		{
			memcpy(m_ClearColor, color, 4 * sizeof(float));
		}
		ClearColorTest::~ClearColorTest()
		{

		}

		void ClearColorTest::OnUpdate(float deltaTime)
		{

		}

		void ClearColorTest::OnRender()
		{
			GLCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
		}

		void ClearColorTest::OnImGuiRender()
		{
			ImGui::ColorPicker4("Color Picker", m_ClearColor);
		}

	}
}

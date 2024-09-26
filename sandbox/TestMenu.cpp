#include "TestMenu.hpp"
#include "imgui.h"
namespace glash
{
	namespace test
	{
		TestMenu::TestMenu(Window_OLD& window)
			: Test(window), m_CurrentTest(this)
		{
			
		}
		void TestMenu::OnRender()
		{
			if (m_CurrentTest != this)
			{
				m_CurrentTest->OnRender();
			}
		}
		void TestMenu::OnImGuiRender()
		{
			assert(m_CurrentTest != nullptr);

			if (m_CurrentTest == this)
			{
				for (const auto&[name, createTestCallback] : m_Tests)
				{
					if (ImGui::Button(name.c_str()))
					{
						m_CurrentTest = createTestCallback();
					}
				}
			}
			else
			{
				if (ImGui::Button("<-"))
				{
					delete m_CurrentTest;
					m_CurrentTest = this;
				}
				else
				{
					m_CurrentTest->OnImGuiRender();
				}
			}

		}
		void TestMenu::OnUpdate(float deltaTime)
		{
			if (m_CurrentTest != this)
			{
				m_CurrentTest->OnUpdate(deltaTime);
			}
		}
	}
}
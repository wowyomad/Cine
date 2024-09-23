#pragma once

#include "Test.hpp"
#include "glash/glash_pch.hpp"
#include "glash/Window.hpp"

namespace glash
{
	namespace test
	{
		class TestMenu : public Test
		{
		public:
			TestMenu(Window& window);

			template <class InitialTest>
			TestMenu(Window& window, const std::string& testName)
				: Test(window)
			{
				RegisterTest<InitialTest>(testName);
				m_CurrentTest = m_Tests[0].second();
			}

			template <class T, class ...Args>
			inline void RegisterTest(const std::string& name, Args&& ...args)
			{
				m_Tests.push_back(std::make_pair(name, [&]() {
					return new T(m_Window, std::forward<Args>(args)...);
					}));
			}

			void OnRender() override;
			void OnImGuiRender() override;
			void OnUpdate(float deltaTime) override;

		private:
			Test* m_CurrentTest;
			std::vector <std::pair<std::string, std::function<Test* ()>>> m_Tests;
		};

	}
}
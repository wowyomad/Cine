#include "Window.hpp"



namespace glash
{
	class ImguiWindow : public Window
	{
	public:
		ImguiWindow(int width, int height, const std::string& title, const Color& clearColor = color::GRAY);

		void NewImguiFrame();
		void ImguiRender();

	};
}

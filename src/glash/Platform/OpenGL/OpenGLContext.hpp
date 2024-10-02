#pragma once
#include "glash/Core/Core.hpp"
#if defined (GLASH_PLATFORM_OPENGL) == 1


#include "glash/Renderer/GrahpicsContext.hpp"

struct GLFWwindow;

namespace glash
{
	class GLASH_API OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);

		void Init() override;
		void SwapBuffers() override;

	protected:
		GLFWwindow* m_WindowHandle;
	};
}

#endif //GLASH_PLATFORM_OPENGL

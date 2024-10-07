#pragma once
#include "glash/Core/Core.hpp"
#if CINE_PLATFORM_OPENGL

#include "glash/Renderer/GrahpicsContext.hpp"

struct GLFWwindow;

namespace Cine
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

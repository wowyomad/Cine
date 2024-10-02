#include "glash/Renderer/RendererAPI.hpp"
#include "glash/Platform/OpenGL/OpenGLRendererAPI.hpp"
namespace glash
{
	Ref<RendererAPI> IRendererAPI::s_Instance = nullptr;

	//Temporary hard coded.
	Scope<RendererAPI> IRendererAPI::Create()
	{
		return CreateScope<OpenGLRendererAPI>();
	}
}
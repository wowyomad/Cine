#include "glash/Renderer/RendererAPI.hpp"
namespace Cine
{
	Ref<RendererAPI> IRendererAPI::s_Instance = RendererAPI::Create();

	//Temporary hard coded.
	Ref<RendererAPI> IRendererAPI::Create()
	{
		return CreateRef<OpenGLRendererAPI>();

	}
}
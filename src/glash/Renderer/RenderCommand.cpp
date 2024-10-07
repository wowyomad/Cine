#include "glash/glash_pch.hpp"
#include "glash/Renderer/RenderCommand.hpp"

#include "glash/Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace Cine
{
	Ref<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Get();
}
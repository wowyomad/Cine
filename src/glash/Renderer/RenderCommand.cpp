#include "glash/glash_pch.hpp"
#include "glash/Renderer/RenderCommand.hpp"

#include "glash/Platform/OpenGL/OpenGLRendererAPI.hpp"

namespace glash
{
	Scope<RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
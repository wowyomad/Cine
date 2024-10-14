#pragma once

#include "glash/Core/Base.hpp"

#include "glash/Renderer/RendererAPI.hpp"
#include "glash/Renderer/Shader.hpp"
#include "glash/Renderer/OrthoGraphicCamera.hpp"

#include "glash/Platform/OpenGL/OpenGLRendererAPI.hpp"


namespace Cine
{	
	class Renderer 
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const OrthographicCamera& camera); //This is broken btw
		static void EndScene();

		static void Submit(const Ref<IShader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static inline RendererAPI& Get() { return *RendererAPI::Get(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;

	private:
	};
}
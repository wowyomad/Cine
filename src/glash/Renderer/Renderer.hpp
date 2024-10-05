#pragma once

#include "glash/Core/Core.hpp"

#include "glash/Renderer/RendererAPI.hpp"
#include "glash/Renderer/Shader.hpp"
#include "glash/Renderer/Camera.hpp"

#include "glash/Platform/OpenGL/OpenGLRendererAPI.hpp"


namespace glash
{	
	class Renderer 
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene(const Camera& camera);
		static void EndScene();

		static void Submit(const Ref<IShader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static inline RendererAPI& Get() { return RendererAPI::Get(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;

	private:
	};
}
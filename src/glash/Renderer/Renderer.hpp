#pragma once

#include "glash/Core/Core.hpp"

#include "glash/Renderer/RendererAPI.hpp"
#include "glash/Renderer/Shader.hpp"

namespace glash
{
	class Renderer 
	{
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height);

		static void BeginScene();
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));

		static inline GLASH_RENDERER_API_CLASS& GetAPI() { return RendererAPI::GetAPI(); }
	private:
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;
		};

		static Scope<SceneData> s_SceneData;

	private:
	};
}
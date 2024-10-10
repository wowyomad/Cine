#pragma once
#include "glash/Core/Base.hpp"
#include "glash/Renderer/Camera.hpp"

#include "glash/Renderer/Texture.hpp"

namespace Cine
{
	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();

		static void BeginScene(const OrthographicCamera& camera);
		static void EndScene();

		//vec2
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));


		//ve3
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));

		//mat4
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tintColor = glm::vec4(1.0f));



	};
}
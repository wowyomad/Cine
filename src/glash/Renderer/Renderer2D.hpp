#pragma once
#include "glash/Core/Base.hpp"
#include "Camera.hpp"
#include "EditorCamera.hpp"

#include "glash/Scene/Components.hpp"
#include "glash/Renderer/Texture.hpp"

namespace Cine
{

	class Renderer2D
	{
	public:
		static void Init();
		static void Shutdown();
		
		static void StartBatch();
		static void NextBatch();

		//This can also be a system in the feature.
		static void BeginScene(const Camera& camera, const glm::mat4& transform);
		static void BeginScene(const EditorCamera& camera);

		static void EndScene();
		static void Flush();
		static void Clear();
		//vec2
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));

		//veс3
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);
		static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const glm::vec4& color);
		static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));

		//mat4
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color, int entityID = -1);
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tiling = 1.0f, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const std::array<glm::vec2, 4>& texCoords, int entityID, const glm::vec4& tint = glm::vec4(1.0f));

		static void DrawSprite(const glm::mat4& transform, const SpriteSheetComponent& spriteSheet, uint32_t spriteIndex, const glm::vec4& tint = glm::vec4(1.0f));
		static void DrawSprite(const glm::mat4& transform, const SpriteSheetComponent& spriteSheet, uint32_t spriteIndex, int entityID, const glm::vec4& tint = glm::vec4(1.0f));

	public:
		struct Statistics
		{
			uint32_t DrawCalls = 0;
			uint32_t QuadCount = 0;

			uint32_t GetTotalVertexCount() const { return QuadCount * 4; }
			uint32_t GetTotalIndexCount() const { return QuadCount * 6; }
		};
		static void ResetStats();
		static const Statistics& GetStats();

	};
}
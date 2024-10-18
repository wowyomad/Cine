#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "glash/Renderer/Sprite.hpp"
#include "glash/Renderer/SpriteAnimation2D.hpp"

#include "SceneCamera.hpp"

namespace Cine
{
	struct TagComponent
	{
		std::string Tag;

		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const std::string& tag)
			: Tag(tag) { }

		operator std::string& () { return Tag; }
		operator const std::string& () const { return Tag; }
	};

	struct TransformComponent
	{
		glm::vec3 Translation = glm::vec3(0.0f);
		glm::vec3 Rotation = glm::vec3(0.0f);
		glm::vec3 Scale = glm::vec3(1.0f);

		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::vec3& translation)
			: Translation(translation) {}

		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}
	};

	struct SpriteRendererComponent
	{
		glm::vec4 Color = glm::vec4(1.0f);
		int SpriteSheetIndex = 0;
		bool UseSprite = true;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
		SpriteRendererComponent(const glm::vec4& color)
			: Color(color) {}
	};

	struct SpriteSheetComponent
	{
		Ref<Texture2D> Texture;
		std::vector<Sprite::Frame> Frames;

		SpriteSheetComponent() = default;
		SpriteSheetComponent(const SpriteSheetComponent&) = default;
		SpriteSheetComponent(const Ref<Texture2D>& texture)
			: Texture(texture) {
			Frames.push_back(Sprite::Frame(0, 0, texture->GetWidth(), texture->GetHeight()));
		}
	};

	struct SpriteAnimationComponent
	{
		std::unordered_map<std::string, Ref<SpriteAnimation>> Animations;
		std::string CurrentAnimation;
		float ElapsedTime = 0.0f;
		int CurrentFrame = 0;
	};

	//Camera is a component like in Unity
	struct CameraComponent
	{
		SceneCamera Camera;
		bool FixedAspectRatio = false;

		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
	};

	class NativeScript;

	struct NativeScriptComponent
	{
		struct Data
		{
			size_t Index;
			NativeScript* Instance = nullptr;

			std::function<NativeScript*()> InstantiateScript;
		};

		static const size_t MaxScripts = 16;
		std::vector<Data> Scripts;

		template <class T>
		void Bind(std::function<NativeScript* ()> instantiateScript)
		{
			if (Scripts.size() < MaxScripts)
			{
				size_t ScriptID = m_ScriptCounter;
				Data data;
				data.Index = ScriptID;
				data.InstantiateScript = instantiateScript;

				Scripts.push_back(data);

				++m_ScriptCounter;
			}
		}
	private:
		size_t m_ScriptCounter = 0;

	};
}
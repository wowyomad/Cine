#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "SceneCamera.hpp"
#include "glash/Renderer/Texture.hpp"
#include "glash/Utils/StringUtils.hpp"

namespace Cine
{
	class Entity;

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

		glm::mat4 GetLocalTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(Rotation));
			return glm::translate(glm::mat4(1.0f), Translation)
				* rotation
				* glm::scale(glm::mat4(1.0f), Scale);
		}

		static glm::mat4 GetWorldTransform(const Entity& entity);
	};

	struct CachedTransform
	{
		glm::mat4 CachedTransform;
	};

	struct SpriteRendererComponent
	{
		bool UseSprite = false;

		SpriteRendererComponent() = default;
		SpriteRendererComponent(const SpriteRendererComponent&) = default;
	};

	struct SpriteSheetComponent
	{
		struct Frame
		{
			std::array<glm::vec2, 4> Coords;
		};
		Ref<Texture2D> Texture;
		std::vector<Frame> Frames;
	};

	struct SpriteComponent
	{
		glm::vec4 Color = glm::vec4(1.0f);
		int32_t SpriteIndex = 0;
	};

	struct SpriteAnimationComponent
	{
		struct Animation
		{
			std::string Name;
			std::vector<int32_t> SpriteFrames;
			float Duration = 1.0f;
			bool Loop = true;
		};
		struct AnimationState
		{
			int32_t CurrentAnimation = -1;
			int32_t DefaultAnimation = 0;
			int32_t DesiredAnimation = DefaultAnimation;

			float CurrentTime = 0.0f;
			bool Play = true;
		};
		std::map<int32_t, Animation> Animations;
		AnimationState State;
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
			std::string Name;
			NativeScript* Instance = nullptr;

			std::function<NativeScript* ()> InstantiateScript;
			std::function<void()> RemoveScript;
		};

		~NativeScriptComponent()
		{
			for (auto& script : Scripts)
			{
				if (script.RemoveScript)
				{
					script.RemoveScript();
				};
			}
		}

		std::vector<Data> Scripts;

		template <class T>
		void Bind(std::function<NativeScript* ()> instantiateScript, std::function<void()> removeScript)
		{
			size_t ScriptID = m_ScriptCounter;
			Data data;
			data.Index = ScriptID;
			data.Name = Utils::GetClassTypenameWithSpaces<T>();
			data.InstantiateScript = instantiateScript;
			data.RemoveScript = removeScript;

			Scripts.push_back(data);

			++m_ScriptCounter;
		}
	private:
		size_t m_ScriptCounter = 0;

	};
}
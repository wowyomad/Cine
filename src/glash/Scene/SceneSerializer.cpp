#include "glash/glash_pch.hpp"
#include "SceneSerializer.hpp"

#include "glash/Scene/Entity.hpp"
#include "glash/Scene/Components.hpp"

#include <yaml-cpp/yaml.h>

template<>
struct YAML::convert<glm::vec3>
{
	static Node encode(const glm::vec3& rhs)
	{
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		return node;
	}

	static bool decode(const Node& node, glm::vec3& rhs)
	{
		if (!node.IsSequence() || node.size() != 3)
		{
			return false;
		}

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();

		return true;
	}
};

template<>
struct YAML::convert<glm::vec4>
{
	static Node encode(const glm::vec4& rhs)
	{
		Node node;
		node.push_back(rhs.x);
		node.push_back(rhs.y);
		node.push_back(rhs.z);
		node.push_back(rhs.w);

		return node;
	}

	static bool decode(const Node& node, glm::vec4& rhs)
	{
		if (!node.IsSequence() || node.size() != 4)
		{
			return false;
		}

		rhs.x = node[0].as<float>();
		rhs.y = node[1].as<float>();
		rhs.z = node[2].as<float>();
		rhs.w = node[3].as<float>();

		return true;
	}
};


namespace Cine
{

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	
	void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap; //Entity
		out << YAML::Key << "Entity" << YAML::Value << "123123123"; //Entity ID

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; //TagComponent

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; //TagComponent
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap; //TransformComponent

			auto& transformComponent = entity.GetComponent<TransformComponent>();
			
			out << YAML::Key << "Translation" << transformComponent.Translation;
			out << YAML::Key << "Rotation" << transformComponent.Rotation;
			out << YAML::Key << "Scale" << transformComponent.Scale;

			out << YAML::EndMap; //TransformComponent
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap; //CameraComponent

			auto& cameraComponent = entity.GetComponent<CameraComponent>();
			auto& camera = cameraComponent.Camera;

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap; //Camera
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveVerticalFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
			out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
			out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
			out << YAML::EndMap; //Camera

			out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent.FixedAspectRatio;

			//This would work for now. Should use UUIDs and store this 'flag' in scene, not in every camera component.
			//TODO: Remove this
			bool isMainCamera = m_Scene->GetMainCameraEntity() == entity;
			out << YAML::Key << "IsMainCamera" << YAML::Value << isMainCamera; 

			out << YAML::EndMap; //Camera Component
		}

		if (entity.HasComponent<SpriteRendererComponent>())
		{
			out << YAML::Key << "SpriteRendererComponent";
			out << YAML::BeginMap;

			auto& spriteRendererComponent = entity.GetComponent<SpriteRendererComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteRendererComponent.Color;

			out << YAML::EndMap;
		}

		out << YAML::EndMap; //Entity
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene)
		: m_Scene(scene) {}

	void SceneSerializer::Serialize(const std::filesystem::path& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene" << YAML::Value << "Unnamed Scene";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;

		m_Scene->m_Registry.view<entt::entity>().each([&](auto entityID)
			{
				Entity entity = { entityID, m_Scene.get() };
				if (!entity)
				{
					return;
				}

				SerializeEntity(out, entity);
			});
		out << YAML::EndSeq;
		out << YAML::EndMap;
		//TODO: Save which camera is Main.

		if (filepath.has_parent_path())
		{
			std::filesystem::create_directories(filepath.parent_path());
		}

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::filesystem::path& filepath)
	{
		CINE_CORE_ASSERT(false, "{} is not implemented yet", __func__);
	}

	bool SceneSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();
		CINE_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entitiesNode = data["Entities"];

		auto& entities = entitiesNode;

		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>(); //TODO

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
				{
					name = tagComponent["Tag"].as<std::string>();
				}
				CINE_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntity(name); //TODO: with UUID

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = transformComponent["Translation"].as<glm::vec3>();
					tc.Rotation = transformComponent["Rotation"].as<glm::vec3>();
					tc.Scale = transformComponent["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto cameraProps = cameraComponent["Camera"];

					auto& cc = deserializedEntity.AddComponent<CameraComponent>();
					cc.Camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(cameraProps["ProjectionType"].as<int>()));
					cc.Camera.SetPerspectiveVerticalFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.Camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.Camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());
					cc.Camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
					cc.Camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
					cc.Camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());
					
					cc.FixedAspectRatio = cameraComponent["FixedAspectRatio"].as<bool>();
					bool isMainCamera = cameraComponent["IsMainCamera"].as<bool>();
					if (isMainCamera)
					{
						m_Scene->SetMainCamera(deserializedEntity);
					}
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& cc = deserializedEntity.AddComponent<SpriteRendererComponent>();
					cc.Color = spriteRendererComponent["Color"].as<glm::vec4>();
				}

			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::filesystem::path& filepath)
	{
		CINE_CORE_ASSERT(false, "{} is not implemented yet", __func__);
		return false;
	}

	SpriteSheetMetaSerializer::SpriteSheetMetaSerializer(SpriteSheetComponent& spritesheet)
		: m_Spritesheet(spritesheet)
	{

	}

	void SpriteSheetMetaSerializer::Serialize(const std::filesystem::path& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginSeq;

		for (const auto& frame : m_Spritesheet.Frames)
		{
			out << YAML::BeginMap;

			out << YAML::Key << "Coords" << YAML::Value << YAML::BeginSeq;
			for (const auto& coord : frame.Coords)
			{
				out << YAML::Flow << YAML::BeginSeq << coord.x << coord.y << YAML::EndSeq;
			}
			out << YAML::EndSeq;

			out << YAML::EndMap;
		}

		out << YAML::EndSeq;

		std::ofstream fout(filepath);
		fout << out.c_str();
		fout.close();
	}
	bool SpriteSheetMetaSerializer::Deserialize(const std::filesystem::path& filepath)
	{
		std::ifstream fin(filepath);
		if (!fin.is_open())
			return false;

		YAML::Node data = YAML::Load(fin);
		if (!data.IsSequence())
			return false;

		m_Spritesheet.Frames.clear();

		for (const auto& frameNode : data)
		{
			if (!frameNode["Coords"])
				continue;

			SpriteSheetComponent::Frame frame;
			const auto& coordsNode = frameNode["Coords"];

			for (size_t i = 0; i < coordsNode.size() && i < 4; ++i)
			{
				auto coord = coordsNode[i];
				frame.Coords[i].x = coord[0].as<float>();
				frame.Coords[i].y = coord[1].as<float>();
			}

			m_Spritesheet.Frames.push_back(frame);
		}

		return true;
	}
}



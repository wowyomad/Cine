#include "glash/glash_pch.hpp"
#include "SceneSerializer.hpp"
#include "ComponentSerializer.hpp"

#include "glash/Scene/Entity.hpp"
#include "glash/Scene/Components.hpp"
#include "glash/Scene/AssetManager.hpp"
#include "glash/Scene/NativeScript.hpp"


#include <yaml-cpp/yaml.h>

namespace Cine
{
	static std::string RigidBody2DBodyTypeToString(RigidBody2DComponent::BodyType bodyType)
	{
		switch (bodyType)
		{
		case RigidBody2DComponent::BodyType::Static:    return "Static";
		case RigidBody2DComponent::BodyType::Dynamic:   return "Dynamic";
		case RigidBody2DComponent::BodyType::Kinematic: return "Kinematic";
		}

		CINE_CORE_ASSERT(false, "Unknown body type");
		return {};
	}

	static RigidBody2DComponent::BodyType RigidBody2DBodyTypeFromString(const std::string& bodyTypeString)
	{
		if (bodyTypeString == "Static")    return RigidBody2DComponent::BodyType::Static;
		if (bodyTypeString == "Dynamic")   return RigidBody2DComponent::BodyType::Dynamic;
		if (bodyTypeString == "Kinematic") return RigidBody2DComponent::BodyType::Kinematic;

		CINE_CORE_ASSERT(false, "Unknown body type");
		return RigidBody2DComponent::BodyType::Static;
	}


	template<typename T> T ParseValueSafe(const YAML::Node& node, const std::string& key, const T& defaultValue)
	{
		if (node[key])
		{
			try {
				return node[key].as<T>();
			}
			catch (const YAML::Exception&)
			{
				CINE_LOG_WARN("Coudln't parse key {0} in node {1}. Using default value.", key, node.Tag());
			}
		}
		else
		{
			CINE_LOG_WARN("Tried to parse non-existing key {0} in node {1}", key, node.Tag());
		}
		return defaultValue;
	}

	void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		CINE_CORE_ASSERT(entity.HasComponent<IDComponent>(), "Entity doesn't have IDComponent");

		out << YAML::BeginMap; //Entity
		out << YAML::Key << "Entity" << YAML::Value << entity.GetID(); //Entity ID

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap; //TagComponent

			auto& tag = entity.GetComponent<TagComponent>().Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;

			out << YAML::EndMap; //TagComponent
		}

		if (entity.HasComponent<SpriteLayerComponent>())
		{
			out << YAML::Key << "SpriteLayerComponent";
			out << YAML::BeginMap; //SpriteLayerComponent

			auto& layerID = entity.GetComponent<SpriteLayerComponent>().LayerID;
			out << YAML::Key << "LayerID" << YAML::Value << layerID;

			out << YAML::EndMap; //SpriteLayerComponent
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
			out << YAML::Key << "UseSprite" << YAML::Value << spriteRendererComponent.UseSprite;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteComponent>())
		{
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap;

			auto& spriteComponent = entity.GetComponent<SpriteComponent>();
			out << YAML::Key << "Color" << YAML::Value << spriteComponent.Color;
			out << YAML::Key << "SpriteIndex" << YAML::Value << spriteComponent.SpriteIndex;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteSheetComponent>())
		{
			out << YAML::Key << "SpriteSheetComponent";
			out << YAML::BeginMap;

			auto& spriteSheetComponent = entity.GetComponent<SpriteSheetComponent>();
			if (spriteSheetComponent.Texture)
			{
				std::filesystem::path fullTexturePath = spriteSheetComponent.Texture->GetPath();
				std::filesystem::path relativeTexturePath = std::filesystem::relative(fullTexturePath, "Assets");
				std::string name = fullTexturePath.filename().string();

				out << YAML::Key << "Texture" << YAML::Value << relativeTexturePath.string();
				out << YAML::Key << "Name" << YAML::Value << name;
			}

			out << YAML::BeginSeq;

			for (const auto& frame : spriteSheetComponent.Frames)
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

			out << YAML::EndMap;
		}

		if (entity.HasComponent<RigidBody2DComponent>())
		{
			out << YAML::Key << "RigidBody2DComponent";
			out << YAML::BeginMap;

			auto& rb = entity.GetComponent<RigidBody2DComponent>();

			out << YAML::Key << "Type" << YAML::Value << RigidBody2DBodyTypeToString(rb.Type);
			out << YAML::Key << "FixedRotation" << YAML::Value << rb.FixedRotation;
			out << YAML::Key << "GravityScale" << YAML::Value << rb.GravityScale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<BoxCollider2DComponent>())
		{
			out << YAML::Key << "BoxCollider2DComponent";
			out << YAML::BeginMap;

			auto& collider = entity.GetComponent<BoxCollider2DComponent>();

			out << YAML::Key << "IsTrigger" << collider.IsTrigger;
			out << YAML::Key << "Offset" << collider.Offset;
			out << YAML::Key << "Size" << collider.Size;
			out << YAML::Key << "Density" << collider.Density;
			out << YAML::Key << "Friction" << collider.Friction;
			out << YAML::Key << "Restitution" << collider.Restitution;
			out << YAML::Key << "RestitutionThreshold" << collider.RestitutionThreshold;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteAnimationComponent>())
		{
			//Aimations
		}

		if (entity.HasComponent<NativeScriptComponent>())
		{
			out << YAML::Key << "NativeScriptComponent";
			out << YAML::BeginSeq;

			ScriptEngine& se = ScriptEngine::Get();
			auto& nsc = entity.GetComponent<NativeScriptComponent>();
			for (auto& script : nsc.Scripts)
			{
				out << se.SerializeComponent(entity, script.Name);
			}
			out << YAML::EndSeq;
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

		auto entities = m_Scene->m_Registry.view<entt::entity>();
		for (auto e : entities)
		{
			Entity entity = { e, m_Scene.get() };
			if (!entity)
			{
				return;
			}
			SerializeEntity(out, entity);
		}
		out << YAML::EndSeq;

		out << YAML::Key << "Hierarchy" << YAML::Value << YAML::BeginSeq;
		for (auto e : entities)
		{
			Entity entity = { e, m_Scene.get() };
			out << YAML::BeginMap;
			out << YAML::Key << "ID" << YAML::Value << entity.GetID();

			auto parent = entity.GetParent();
			if (parent)
			{
				out << YAML::Key << "ParentID" << YAML::Value << parent.GetID();
			}
			else
			{
				out << YAML::Key << "ParentID" << YAML::Value << YAML::Null;
			}

			out << YAML::EndMap;
		}
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

		std::string sceneName = ParseValueSafe(data, "Scene", std::string("DefaultScene"));
		CINE_CORE_TRACE("Deserializing scene '{0}'", sceneName);

		auto entitiesNode = data["Entities"];
		auto& entities = entitiesNode;

		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = ParseValueSafe(entity, "Entity", uint64_t(0));

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
				{
					name = ParseValueSafe(tagComponent, "Tag", std::string("Unnamed Entity"));
				}
				CINE_CORE_TRACE("Deserialized entity with ID = {0}, name = {1}", uuid, name);

				Entity deserializedEntity = m_Scene->CreateEntityWithUUID(uuid, name);

				auto spriteLayerComponent = entity["SpriteLayerComponent"];
				if (spriteLayerComponent)
				{
					auto& spriteLayer = deserializedEntity.GetComponent<SpriteLayerComponent>();
					spriteLayer.LayerID = ParseValueSafe(spriteLayerComponent, "LayerID", spriteLayer.LayerID);
				}

				auto transformComponent = entity["TransformComponent"];
				if (transformComponent)
				{
					auto& tc = deserializedEntity.GetComponent<TransformComponent>();
					tc.Translation = ParseValueSafe(transformComponent, "Translation", tc.Translation);
					tc.Rotation = ParseValueSafe(transformComponent, "Rotation", tc.Rotation);
					tc.Scale = ParseValueSafe(transformComponent, "Scale", tc.Scale);
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto cameraProps = cameraComponent["Camera"];

					auto& cc = deserializedEntity.AddComponent<CameraComponent>();
					cc.Camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(ParseValueSafe(cameraProps, "ProjectionType", static_cast<int>(cc.Camera.GetProjectionType()))));
					cc.Camera.SetPerspectiveVerticalFOV(ParseValueSafe(cameraProps, "PerspectiveFOV", cc.Camera.GetPerspectiveVerticalFOV()));
					cc.Camera.SetPerspectiveNearClip(ParseValueSafe(cameraProps, "PerspectiveNear", cc.Camera.GetPerspectiveNearClip()));
					cc.Camera.SetPerspectiveFarClip(ParseValueSafe(cameraProps, "PerspectiveFar", cc.Camera.GetPerspectiveFarClip()));
					cc.Camera.SetOrthographicSize(ParseValueSafe(cameraProps, "OrthographicSize", cc.Camera.GetOrthographicSize()));
					cc.Camera.SetOrthographicNearClip(ParseValueSafe(cameraProps, "OrthographicNear", cc.Camera.GetOrthographicNearClip()));
					cc.Camera.SetOrthographicFarClip(ParseValueSafe(cameraProps, "OrthographicFar", cc.Camera.GetOrthographicFarClip()));

					cc.FixedAspectRatio = ParseValueSafe(cameraComponent, "FixedAspectRatio", cc.FixedAspectRatio);
					bool isMainCamera = ParseValueSafe(cameraComponent, "IsMainCamera", false);
					if (isMainCamera)
					{
						m_Scene->SetMainCamera(deserializedEntity);
					}
				}

				auto spriteRendererComponent = entity["SpriteRendererComponent"];
				if (spriteRendererComponent)
				{
					auto& cc = deserializedEntity.AddComponent<SpriteRendererComponent>();
					cc.UseSprite = ParseValueSafe(spriteRendererComponent, "UseSprite", cc.UseSprite);
				}

				auto spriteComponent = entity["SpriteComponent"];
				if (spriteComponent)
				{
					auto&& cc = deserializedEntity.AddComponent<SpriteComponent>();
					cc.Color = ParseValueSafe(spriteComponent, "Color", cc.Color);
					cc.SpriteIndex = ParseValueSafe(spriteComponent, "SpriteIndex", cc.SpriteIndex);
				}

				auto spriteSheetComponent = entity["SpriteSheetComponent"];
				if (spriteSheetComponent)
				{
					auto&& cc = deserializedEntity.AddComponent<SpriteSheetComponent>();

					if (spriteSheetComponent["Texture"])
					{
						std::filesystem::path texturePath = ParseValueSafe(spriteSheetComponent, "Texture", std::string());
						std::string name = ParseValueSafe(spriteSheetComponent, "Name", std::string());

						cc = AssetManager::LoadSpriteSheet(name, texturePath, false);

						if (spriteSheetComponent["Frames"] && spriteSheetComponent["Frames"].IsSequence()) {
							cc.Frames.clear();

							for (const auto& frameNode : spriteSheetComponent["Frames"]) {
								if (!frameNode.IsMap() || !frameNode["Coords"].IsSequence()) continue;

								SpriteSheetComponent::Frame frame;

								int i = 0;
								for (const auto& coordNode : frameNode["Coords"]) {
									if (coordNode.size() != 2) continue;

									glm::vec2 coord;
									coord.x = ParseValueSafe(coordNode, "0", 0.0f);
									coord.y = ParseValueSafe(coordNode, "1", 0.0f);
									frame.Coords[i++] = coord;
								}

								cc.Frames.push_back(frame);
							}
						}
					}
				}

				auto rigidBody2DComponent = entity["RigidBody2DComponent"];
				if (rigidBody2DComponent)
				{
					auto&& rb = deserializedEntity.AddOrReplaceComponent<RigidBody2DComponent>();
					rb.FixedRotation = ParseValueSafe(rigidBody2DComponent, "FixedRotation", rb.FixedRotation);
					rb.Type = RigidBody2DBodyTypeFromString(ParseValueSafe(rigidBody2DComponent, "Type", std::string("Static")));
					rb.GravityScale = ParseValueSafe(rigidBody2DComponent, "GravityScale", rb.GravityScale);
				}

				auto boxCollider2DComponent = entity["BoxCollider2DComponent"];
				if (boxCollider2DComponent)
				{
					auto&& collider = deserializedEntity.AddOrReplaceComponent<BoxCollider2DComponent>();
					collider.IsTrigger = ParseValueSafe(boxCollider2DComponent, "IsTrigger", collider.IsTrigger);
					collider.Offset = ParseValueSafe(boxCollider2DComponent, "Offset", collider.Offset);
					collider.Size = ParseValueSafe(boxCollider2DComponent, "Size", collider.Size);
					collider.Density = ParseValueSafe(boxCollider2DComponent, "Density", collider.Density);
					collider.Friction = ParseValueSafe(boxCollider2DComponent, "Friction", collider.Friction);
					collider.Restitution = ParseValueSafe(boxCollider2DComponent, "Restitution", collider.Restitution);
					collider.RestitutionThreshold = ParseValueSafe(boxCollider2DComponent, "RestitutionThreshold", collider.RestitutionThreshold);
				}

				auto nativeScriptComponent = entity["NativeScriptComponent"];
				if (nativeScriptComponent)
				{
					auto&& nsc = deserializedEntity.AddOrReplaceComponent<NativeScriptComponent>();

					for (const auto& node : nativeScriptComponent)
					{
						if (!node.IsMap() || node.size() != 1)
						{
							CINE_CORE_ERROR("Invalid component format ({0}). Expected a map with one entry.", YAML::Dump(node));
							continue;
						}
						std::string componentName = node.begin()->first.as<std::string>();
						const YAML::Node& componentData = node.begin()->second;

						YAML::Node wrappedComponentData;
						wrappedComponentData[componentName] = componentData;

						ScriptEngine::Get().CreateComponent(deserializedEntity, componentName);
						ScriptEngine::Get().DeserializeComponent(deserializedEntity, const_cast<YAML::Node&>(wrappedComponentData), componentName);
					}
				}
			}
		}

		auto hierarchy = data["Hierarchy"];
		if (hierarchy)
		{
			for (const auto& node : hierarchy)
			{
				UUID entityID = ParseValueSafe(node, "ID", uint64_t(0));
				Entity entity = m_Scene->GetEntityById(entityID);

				if (entity && node["ParentID"].IsScalar())
				{
					UUID parentID = ParseValueSafe(node, "ParentID", uint64_t(0));
					Entity parentEntity = m_Scene->GetEntityById(parentID);
					entity.AddParentWithoutTransform(parentEntity);
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



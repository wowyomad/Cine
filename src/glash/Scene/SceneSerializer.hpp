#pragma once

#include "Scene.hpp"

#include <filesystem>


//Forward declare YAML::Emitter
namespace YAML
{
	class Emitter;
}

namespace Cine
{
	class YAML::Emitter;

	class SceneSerializer
	{
	public:
		SceneSerializer(const Ref<Scene>& scene);

		void Serialize(const std::filesystem::path& filepath);
		void SerializeRuntime(const std::filesystem::path& filepath);

		bool Deserialize(const std::filesystem::path& filepath);
		bool DeserializeRuntime(const std::filesystem::path& filepath);

	private:
		void SerializeEntity(YAML::Emitter& out, Entity entity);

	private:
		Ref<Scene> m_Scene;
	};
}
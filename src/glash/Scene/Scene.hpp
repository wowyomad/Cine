#pragma once
#include "glash/Core/Base.hpp"

#include <entt/entt.hpp>

#include "glash/Core/Timestep.hpp"

namespace Cine
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());
		
		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;

		friend class Entity;
	};
}
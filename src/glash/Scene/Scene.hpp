#pragma once
#include "glash/Core/Base.hpp"
#include "glash/Core/Timestep.hpp"

#include <entt/entt.hpp>

namespace Cine
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene();

		Entity CreateEntity(const std::string& name = std::string());

		void SetMainCamera(Entity cameraEntity);
		Entity GetMainCamera();

		void OnViewportResize(uint32_t width, uint32_t height);
		
		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;
		Entity* m_MainCamera;

		uint32_t m_ViewportWidth = 1;
		uint32_t m_ViewportHeight = 1;

		friend class Entity;
	};
}
#include "glash/glash_pch.hpp"
#include "Entity.hpp"

namespace Cine
{
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{
		CINE_CORE_ASSERT(scene != nullptr && handle != entt::null, "That won't work.");
	}

	Entity Entity::GetParent()
	{
		return GetComponent<HierarchyComponent>().Parent;
	}

	const std::vector<Entity>& Entity::GetChildren()
	{
		return GetComponent<HierarchyComponent>().Children;
	}

	void Entity::AddChild(Entity child)
	{
		auto& hierarchy = GetComponent<HierarchyComponent>();

		if (*this == child)
		{
			CINE_CORE_WARN("Tried to add self as a child");
			return;
		}

		if (hierarchy.Parent == child)
		{
			CINE_CORE_WARN("Tried to add parent as a child");
			return;
		}

		auto& children = hierarchy.Children;
		auto it = std::find_if(children.begin(), children.end(), [child](Entity& entity) -> bool
			{
				return entity == child;
			});
		if (it != children.end())
		{
			CINE_CORE_WARN("Child {0} is already a child of {1}", static_cast<uint32_t>(child), static_cast<uint32_t>(*this));
			return;
		}

		children.push_back(child);
		child.AddParent(*this);

		CINE_CORE_TRACE("Added child {0} to entity {1}", static_cast<uint32_t>(child), static_cast<uint32_t>(*this));
	}

	void Entity::AddParent(Entity parent)
	{
		auto& hierarchy = GetComponent<HierarchyComponent>();

		if (*this == parent)
		{
			CINE_CORE_WARN("Tried to add self as a parent");
			return;
		}

		auto& children = hierarchy.Children;
		auto it = std::find_if(children.begin(), children.end(), [parent](Entity& entity) -> bool
			{
				return entity == parent;
			});
		if (it != children.end())
		{

			CINE_CORE_WARN("Tried to add child {0} as a parent of {1}", static_cast<uint32_t>(parent), static_cast<uint32_t>(*this));
			return;
		}
		if (hierarchy.Parent)
		{
			hierarchy.Parent.RemoveChild(*this);
		}
		hierarchy.Parent = parent;

		CINE_CORE_TRACE("Added parent {0} to entity {1}", static_cast<uint32_t>(parent), static_cast<uint32_t>(*this));
	}

	void Entity::RemoveChild(Entity child)
	{
		auto& hierarchy = GetComponent<HierarchyComponent>();
		auto& childHierarchy = child.GetComponent<HierarchyComponent>();

		//TODO: checks

		auto it = std::find_if(hierarchy.Children.begin(), hierarchy.Children.end(), [child](Entity& entity) -> bool
			{
				return entity == child;
			});
		if (it == hierarchy.Children.end())
		{
			CINE_CORE_WARN("Entity {0} doesn't have child {1}", static_cast<uint32_t>(*this), static_cast<uint32_t>(child));
			return;
		}

		childHierarchy.Parent = {};
		hierarchy.Children.erase(it);

		CINE_CORE_TRACE("Removed child {0} from entity {1}", static_cast<uint32_t>(child), static_cast<uint32_t>(*this));
	}

	void Entity::RemoveParent()
	{
		auto& hierarchy = GetComponent<HierarchyComponent>();
		auto& parentHierarchy = hierarchy.Parent.GetComponent<HierarchyComponent>();

		//TODO: checks

		auto it = std::find_if(parentHierarchy.Children.begin(), parentHierarchy.Children.end(), [*this](Entity& entity) -> bool
			{
				return *this == entity;
			});
		if (it == parentHierarchy.Children.end())
		{
			CINE_CORE_WARN("Entity {0} doesn't have a parent", static_cast<uint32_t>(*this));
			return;
		}

		hierarchy.Parent = {};
		parentHierarchy.Children.erase(it);

		CINE_CORE_TRACE("Removed parent {0} from entity {1}", static_cast<uint32_t>(hierarchy.Parent), static_cast<uint32_t>(*this));
	}


}
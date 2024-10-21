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
		auto& childHierarchy = child.GetComponent<HierarchyComponent>();

		//if child is this or child parent is this
		if (*this == child || childHierarchy.Parent == *this)
		{
			return;
		}

		//?
		if (hierarchy.Parent == child)
		{

		}

		//child has parent different from this -> remove the child from that parent and that parent from the child
		if (childHierarchy.Parent)
		{
			auto& oldParentChildren = childHierarchy.Parent.GetComponent<HierarchyComponent>().Children;
			oldParentChildren.erase(std::remove_if(oldParentChildren.begin(), oldParentChildren.end(), [child](auto& theChild) -> bool
				{
					return child == theChild;
				}));
			childHierarchy.Parent = {};
		}

		childHierarchy.Parent = *this;
		hierarchy.Children.push_back(child);
	}


	void Entity::AddParent(Entity parent)
	{
		auto& hierarchy = GetComponent<HierarchyComponent>();
		auto& parentHierarchy = parent.GetComponent<HierarchyComponent>();

		bool selfParent = *this == parent;
		bool thisHasParent = hierarchy.Parent;
		bool parentHasParent = hierarchy.Parent;
		bool thisIsParent = *this == parentHierarchy.Parent;
		bool thisIsChild = hierarchy.Parent == parent;

		if (selfParent)
		{
			return;
		}

		if (thisHasParent)
		{
			if (thisIsChild)
			{
				return;
			}
			else if (thisIsParent)
			{
				for (auto child : hierarchy.Children)
				{
					child.GetComponent<HierarchyComponent>().Parent = hierarchy.Parent;
				}
				auto& oldParentChildren = hierarchy.Parent.GetComponent<HierarchyComponent>().Children;
				oldParentChildren.erase(std::remove_if(oldParentChildren.begin(), oldParentChildren.end(), [&](auto& theChild)
					{
						return theChild == *this;
					}));
				hierarchy.Children.clear();
				hierarchy.Parent = parent;
				oldParentChildren.push_back(parent);
				parentHierarchy.Children.push_back(*this);
				return;
			}
			else
			{
				auto& oldParentChildren = hierarchy.Parent.GetComponent<HierarchyComponent>().Children;
				for (auto child : hierarchy.Children)
				{
					child.GetComponent<HierarchyComponent>().Parent = hierarchy.Parent;
					oldParentChildren.push_back(child);
				}
				hierarchy.Children.clear();
				oldParentChildren.erase(std::remove_if(oldParentChildren.begin(), oldParentChildren.end(), [&](auto& theChild)
					{
						return theChild == *this;
					}));
				hierarchy.Parent = parent;

				parentHierarchy.Children.push_back(*this);
				return;
			}
		}
		else
		{
			for (auto child : hierarchy.Children)
			{
				child.GetComponent<HierarchyComponent>().Parent = hierarchy.Parent; //Parent is null
			}
			hierarchy.Children.clear();
			hierarchy.Parent = parent;
			parentHierarchy.Children.push_back(*this);
		}


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

	void Entity::RemoveChildren()
	{
		auto& hierarchy = GetComponent<HierarchyComponent>();
		auto& chidlren = hierarchy.Children;

		for (auto& child : chidlren)
		{
			child.RemoveParent();
		}
	}
	void Entity::RemoveParent()
	{
		auto& hierarchy = GetComponent<HierarchyComponent>();

		if (!hierarchy.Parent)
		{
			return;
		}

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
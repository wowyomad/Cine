#include "glash/glash_pch.hpp"
#include "Entity.hpp"
#include "NativeScript.hpp"

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

	glm::vec3 Entity::Translation()
	{
		auto& hierarchy = GetComponent<HierarchyComponent>();
		if (!hierarchy.Parent)
		{
			return LocalTranslation();
		}
		else
		{
			glm::vec3 translation, rotation, scale;
			auto transform = TransformComponent::GetWorldTransform(*this);
			Math::DecomposeTransform(transform, translation, rotation, scale);
			return translation;
		}

	}

	glm::vec3 Entity::Rotation()
	{
		auto& hierarchy = GetComponent<HierarchyComponent>();
		if (!hierarchy.Parent)
		{
			return LocalRotation();
		}
		else
		{
			glm::vec3 translation, rotation, scale;
			auto transform = TransformComponent::GetWorldTransform(*this);
			Math::DecomposeTransform(transform, translation, rotation, scale);
			return rotation;
		}
	}

	glm::vec3 Entity::Scale()
	{
		auto& hierarchy = GetComponent<HierarchyComponent>();
		if (!hierarchy.Parent)
		{
			return LocalScale();
		}
		else
		{
			glm::vec3 translation, rotation, scale;
			auto transform = TransformComponent::GetWorldTransform(*this);
			Math::DecomposeTransform(transform, translation, rotation, scale);
			return scale;
		}
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

	bool Entity::AddParentWithoutTransform(Entity parent)
	{
		if (*this == parent)
		{
			return false;
		}

		auto& hierarchy = GetComponent<HierarchyComponent>();
		auto& parentHierarchy = parent.GetComponent<HierarchyComponent>();

		Entity currentParent = parent;
		while (currentParent)
		{
			if (*this == currentParent)
			{
				return false;
			}
			currentParent = currentParent.GetComponent<HierarchyComponent>().Parent;
		}

		if (hierarchy.Parent)
		{
			auto& oldParentChildren = hierarchy.Parent.GetComponent<HierarchyComponent>().Children;
			oldParentChildren.erase(std::remove(oldParentChildren.begin(), oldParentChildren.end(), *this), oldParentChildren.end());

			for (auto& child : hierarchy.Children)
			{
				child.GetComponent<HierarchyComponent>().Parent = hierarchy.Parent;
				oldParentChildren.push_back(child);
			}

			hierarchy.Children.clear();
		}

		hierarchy.Parent = parent;
		parentHierarchy.Children.push_back(*this);

		for (auto& child : hierarchy.Children)
		{
			child.GetComponent<HierarchyComponent>().Parent = *this;
		}

		return true;
	}


	bool Entity::AddParent(Entity parent)
	{
		if (*this == parent)
		{
			return false;
		}

		auto& hierarchy = GetComponent<HierarchyComponent>();
		auto& parentHierarchy = parent.GetComponent<HierarchyComponent>();

		// Avoid circular hierarchy
		Entity currentParent = parent;
		while (currentParent)
		{
			if (*this == currentParent)
			{
				return false;
			}
			currentParent = currentParent.GetComponent<HierarchyComponent>().Parent;
		}

		glm::vec3 worldTranslation, worldRotation, worldScale;
		auto worldTransform = TransformComponent::GetWorldTransform(*this);
		Math::DecomposeTransform(worldTransform, worldTranslation, worldRotation, worldScale);

		// Detach from old parent
		if (hierarchy.Parent)
		{
			auto& oldParentChildren = hierarchy.Parent.GetComponent<HierarchyComponent>().Children;
			oldParentChildren.erase(std::remove(oldParentChildren.begin(), oldParentChildren.end(), *this), oldParentChildren.end());

			for (auto& child : hierarchy.Children)
			{
				child.GetComponent<HierarchyComponent>().Parent = hierarchy.Parent;
				oldParentChildren.push_back(child);
			}

			hierarchy.Children.clear();
		}

		// Set the new parent
		hierarchy.Parent = parent;
		parentHierarchy.Children.push_back(*this);

		// Update the parent-child relationships
		for (auto& child : hierarchy.Children)
		{
			child.GetComponent<HierarchyComponent>().Parent = *this;
		}

		glm::mat4 parentWorldTransform = TransformComponent::GetWorldTransform(parent);

		// Construct the world transform based on local translation, rotation, and scale
		glm::mat4 worldTransformWithParent = glm::translate(glm::mat4(1.0f), worldTranslation)
			* glm::toMat4(glm::quat(worldRotation))  // Use worldRotation directly as it's in radians
			* glm::scale(glm::mat4(1.0f), worldScale);

		// Calculate the local transform by reversing the parent's world transform
		glm::mat4 localTransform = glm::inverse(parentWorldTransform) * worldTransformWithParent;

		glm::vec3 localTranslation, localRotation, localScale;
		if (Math::DecomposeTransform(localTransform, localTranslation, localRotation, localScale))
		{
			LocalTranslation() = localTranslation;
			LocalRotation() = localRotation;
			LocalScale() = localScale;
		}

		return true;
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

		auto& transform = Transform();

		transform.Translation = Translation();
		transform.Rotation = Rotation();
		transform.Scale = Scale();


		auto& parentHierarchy = hierarchy.Parent.GetComponent<HierarchyComponent>();

		parentHierarchy.Children.erase(
			std::remove(parentHierarchy.Children.begin(), parentHierarchy.Children.end(), *this),
			parentHierarchy.Children.end()
		);


		hierarchy.Parent = {};

	}

	Entity Entity::Clone()
	{
		Entity clone = Entity(m_Scene->m_Registry.create(), m_Scene);
		clone.AddComponent<IDComponent>(UUID());

		auto& nscStorage = m_Scene->m_Registry.storage<NativeScriptComponent>();
		if (nscStorage.contains(m_EntityHandle))
		{
			nscStorage.push(clone);

			for (auto& script : GetComponent<NativeScriptComponent>().Scripts)
			{
				clone.AddComponentByName(script.Name);
			}
		}

		for (auto&& [id, storage] : m_Scene->m_Registry.storage())
		{
			if (storage.type().hash() == entt::type_id<NativeScriptComponent>().hash())
				continue;
			if (storage.type().hash() == entt::type_id<IDComponent>().hash())
				continue;

			//???
			if (storage.contains(m_EntityHandle))
			{
				//Contains 'script' ('script'???)
				if (storage.contains(clone))
				{
					storage.erase(clone);
				}
				storage.push(clone, storage.value(m_EntityHandle));
			}
		}

		//Clear hierarchy of clone
		auto& cloneHierarchy = clone.GetComponent<HierarchyComponent>();
		cloneHierarchy.Children = {};
		cloneHierarchy.Parent = {};

		if (clone.HasComponent<RigidBody2DComponent>())
		{
			auto& rb = clone.GetComponent<RigidBody2DComponent>();
			rb.RuntimeBody = nullptr;
			m_Scene->m_PhysicsSystem.AddRigidBody(clone);
		}

		return clone;
	}

	void Entity::OnTriggerEnter(Entity other)
	{
		if (HasComponent<NativeScriptComponent>())
		{
			auto& nsc = GetComponent<NativeScriptComponent>();
			for (auto& script : nsc.Scripts)
			{
				NativeScript* instance = script.Instance;
				if (instance)
				{
					instance->OnTriggerEnter(other);
				}
			}
		}
	}

	void Entity::OnTriggerExit(Entity other)
	{
		if (HasComponent<NativeScriptComponent>())
		{
			auto& nsc = GetComponent<NativeScriptComponent>();
			for (auto& script : nsc.Scripts)
			{
				NativeScript* instance = script.Instance;
				if (instance)
				{
					instance->OnTriggerExit(other);
				}
			}
		}
	}
}

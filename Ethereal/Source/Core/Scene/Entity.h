#pragma once

#include <Core/Scene/Scene.h>

namespace Ethereal
{
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity id, Scene* scene);
		Entity(const Entity& other) = default;
		~Entity() = default;

		template <class T, class... Args>
		T& AddComponent(Args&... args)
		{
			return scene->registry.emplace<T>(id, std::forward<Args>(args)...);
		}

		template <class T>
		T& GetComponent()
		{
			return scene->registry.get<T>(id);
		}

		template <class T>
		void RemoveComponent()
		{
			scene->registry.remove<T>(id);
		}

		template <class T>
		bool HasComponent()
		{
			return scene->registry.all_of<T>(id);
		}

		operator bool() const { return id != entt::null; }
		operator entt::entity() const { return id; }
		operator uint32_t() const { return (uint32_t)id; }
		bool operator==(const Entity& other) const { return id == other.id && scene == other.scene; }
		bool operator!=(const Entity& other) const { return !(* this == other); }
	private:
		entt::entity id{ entt::null };
		Scene* scene = nullptr;
	};
}


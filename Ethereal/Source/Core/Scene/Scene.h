#pragma once

#include <entt.hpp>

namespace Ethereal
{
	class Entity;

	class Scene
	{
	public:
		Scene();
		~Scene() = default;

		void OnUpdate();
		void OnViewPortResize(uint32_t width, uint32_t height);
		
		Entity CreateEntity(std::string& name = std::string("Untitled Entity"));
		Entity GetPrimaryCameraEntity();
		void DestroyEntity(Entity entity);

	private:
		entt::registry registry;
		uint32_t viewPortWidth, viewPortHeight;

		friend class Entity;
		friend class SceneSerializer;
		friend class SceneHierarchy;
	};
}


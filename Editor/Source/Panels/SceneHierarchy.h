#pragma once
#include <Core/Scene/Scene.h>
#include <Core/Scene/Entity.h>

namespace Ethereal
{
	class SceneHierarchy
	{
	public:
		SceneHierarchy() = default;
		SceneHierarchy(const std::shared_ptr<Scene>& scene);
		void SetContext(const std::shared_ptr<Scene>& scene);

		void OnGUIRender();

		void SetSelectedEntity(Entity entity);
		Entity GetSelectedEntity() const;
	private:
		void DrawEntityNode(Entity entity);
		void DrawComponents(Entity entity);
	private:
		std::shared_ptr<Scene> context;
		Entity selection;
	};
}


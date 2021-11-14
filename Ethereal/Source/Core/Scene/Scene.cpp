#include "Scene.h"
#include "Components.h"

#include <Core/Render/Renderer2D.h>

#include <Core/Scene/Entity.h>

namespace Ethereal
{
	Scene::Scene()
		: viewPortWidth(0), viewPortHeight(0)
	{}

	void Scene::OnUpdate()
	{
		//Update Scripts
		{
			registry.view<NativeScriptComponent>().each([=](auto entity, auto& nsc)
			{
				//TODO: Move to Scene::OnScenePlay
				if (!nsc.instance)
				{
					nsc.instance = nsc.InstantiateScript();
					nsc.instance->entity = Entity{ entity, this };
					nsc.instance->OnCreate();
				}
				nsc.instance->OnUpdate();
			});
		}

		Camera* primaryCamera = nullptr;
		glm::mat4 cameraTransform;

		auto view = registry.view<TransformComponent, CameraComponent>();
		for (auto& entity : view)
		{
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

			if (camera.primary)
			{
				primaryCamera = &camera.camera;
				cameraTransform = transform.GetTransform();
				break;
			}
		}

		if (primaryCamera)
		{
			Renderer2D::BeginScene(primaryCamera->GetProjection(), cameraTransform);

			auto group = registry.group<TransformComponent>(entt::get<SpriteComponent>);
			for (auto& entity : group)
			{
				auto [transform, sprite] = group.get<TransformComponent, SpriteComponent>(entity);
				Renderer2D::DrawQuad(transform.GetTransform(), sprite.color);
			}
			Renderer2D::EndScene();
		}

	}

	void Scene::OnViewPortResize(uint32_t width, uint32_t height)
	{
		viewPortWidth = width;
		viewPortHeight = height;


		//Resize cameras
		auto view = registry.view<CameraComponent>();
		for (auto& entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.fixedAspectRatio)
			{
				cameraComponent.camera.SetViewportSize(width, height);
			}
		}
	}

	Entity Scene::CreateEntity(std::string& name)
	{
		Entity entity = { registry.create(), this };
		entity.AddComponent<TransformComponent>();
		entity.AddComponent<TagComponent>(name);
		return entity;
	}

	Entity Scene::GetPrimaryCameraEntity()
	{
		auto view = registry.view<CameraComponent>();
		for (auto entity : view)
		{
			const auto& camera = view.get<CameraComponent>(entity);
			if (camera.primary)
				return Entity{ entity, this };
		}
		return {};
	}

	void Scene::DestroyEntity(Entity entity)
	{
		registry.destroy(entity);
	}
}

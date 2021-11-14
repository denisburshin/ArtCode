#include "SceneHierarchy.h"
#include <Core/Scene/Components.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_stdlib.h>
#include <glm/gtc/type_ptr.hpp>

namespace Ethereal
{
	static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
	{
		ImGui::PushID(label.c_str());
		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

		ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
		}

		ImGui::SameLine();
		ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
		}

		ImGui::SameLine();
		ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
		}

		ImGui::SameLine();
		ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

	}

	SceneHierarchy::SceneHierarchy(const std::shared_ptr<Scene>& scene)
	{
		SetContext(scene);
	}

	void SceneHierarchy::SetContext(const std::shared_ptr<Scene>& scene)
	{
		context = scene;
	}

	void SceneHierarchy::DrawEntityNode(Entity entity)
	{
		auto& tag = entity.GetComponent<TagComponent>().tag;

		ImGuiTreeNodeFlags flags = ((selection == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
		bool opened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

		if (ImGui::IsItemClicked())
		{
			selection = entity;
		}

		bool entityRemoved = false;
		if (ImGui::BeginPopupContextItem())
		{
			if (ImGui::MenuItem("Remove Entity"))
			{
				entityRemoved = true;
			}
			ImGui::EndPopup();
		}

		if (entityRemoved)
		{
			context->DestroyEntity(entity);
			if (selection == entity)
			{
				selection = {};
			}
		}
	}
	void SceneHierarchy::DrawComponents(Entity entity)
	{
		if (entity.HasComponent<TagComponent>())
		{
			auto& tag = entity.GetComponent<TagComponent>().tag;
			ImGui::InputText("Tag", &tag, ImGuiInputTextFlags_EnterReturnsTrue);
		}

		const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_AllowItemOverlap;

		if (entity.HasComponent<TransformComponent>())
		{
			auto& tc = entity.GetComponent<TransformComponent>();
			DrawVec3Control("Translation", tc.Translation);
			glm::vec3 rotation = glm::degrees(tc.Rotation);
			DrawVec3Control("Rotation", rotation);
			tc.Rotation = glm::radians(rotation);
			DrawVec3Control("Scale", tc.Scale, 1.0f);
			//ImGui::DragFloat3("Position", glm::value_ptr(tc.Translation), 0.1f);
		}

		if (entity.HasComponent<CameraComponent>())
		{
			auto& cameraComponent = entity.GetComponent<CameraComponent>();

			std::string_view projectionTypes[] = { "Perspective", "Orthographic" };
			std::string_view currentProjectionType = projectionTypes[(int)cameraComponent.camera.GetProjectionType()];
			if (ImGui::BeginCombo("Projection", currentProjectionType.data()))
			{
				for (auto i = 0; i < 2; ++i)
				{
					bool isSelected = currentProjectionType == projectionTypes[i];
					if (ImGui::Selectable(projectionTypes[i].data(), isSelected))
					{
						currentProjectionType = projectionTypes[i];
						cameraComponent.camera.SetProjectionType((SceneCamera::ProjectionType)i);
					}

					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}

			if (cameraComponent.camera.GetProjectionType() == SceneCamera::ProjectionType::Orthographic)
			{
				float orthoSize = cameraComponent.camera.GetOrhtoSize();
				if (ImGui::DragFloat("Size", &orthoSize))
				{
					cameraComponent.camera.SetOrthoSize(orthoSize);
				}
				float orthoNear = cameraComponent.camera.GetOrthoNearClip();
				if (ImGui::DragFloat("Near", &orthoNear))
				{
					cameraComponent.camera.SetOrthoNearClip(orthoNear);
				}
				float orthoFar = cameraComponent.camera.GetOrhtoFarClip();
				if (ImGui::DragFloat("Far", &orthoFar))
				{
					cameraComponent.camera.SetOrthoFarClip(orthoFar);
				}
			}
			if (cameraComponent.camera.GetProjectionType() == SceneCamera::ProjectionType::Perspective)
			{
				float perspectiveFOV = glm::degrees(cameraComponent.camera.GetFOV());
				if (ImGui::DragFloat("FOV", &perspectiveFOV))
				{
					cameraComponent.camera.SetFOV(glm::radians(perspectiveFOV));
				}
				float perspectiveNear = cameraComponent.camera.GetPerspectiveNearClip();
				if (ImGui::DragFloat("Near", &perspectiveNear))
				{
					cameraComponent.camera.SetPerspectiveNearClip(perspectiveNear);
				}
				float perspectiveFar = cameraComponent.camera.GetPerspectiveFarClip();
				if (ImGui::DragFloat("Far", &perspectiveFar))
				{
					cameraComponent.camera.SetPerspectiveFarClip(perspectiveFar);
				}
			}
		}

		if (entity.HasComponent<SpriteComponent>())
		{
			auto& sc = entity.GetComponent<SpriteComponent>();
			ImGui::ColorEdit4("Color", glm::value_ptr(sc.color));
		}
	}
	void SceneHierarchy::OnGUIRender()
	{
		ImGui::Begin("Scene Hierarchy");
		context->registry.each([&](auto entityID)
		{
			Entity entity{ entityID, context.get() };
			DrawEntityNode(entity);
		}); 

		if (ImGui::BeginPopupContextWindow(0, 1, false))
		{
			if (ImGui::MenuItem("Create Entity"))
			{
				context->CreateEntity();
			}
			ImGui::EndPopup();
		}

		ImGui::End();

		ImGui::Begin("Properties");
		if (selection)
		{
			DrawComponents(selection);

			if (ImGui::Button("Add Component"))
			{
				ImGui::OpenPopup("Add Component");
			}

			if (ImGui::BeginPopup("Add Component"))
			{
				if (ImGui::MenuItem("Camera"))
				{
					selection.AddComponent<CameraComponent>();
					ImGui::CloseCurrentPopup();
				}

				if (ImGui::MenuItem("Sprite Component"))
				{
					selection.AddComponent<SpriteComponent>();
					ImGui::CloseCurrentPopup();
				}

				ImGui::EndPopup();
			}

		}
		ImGui::End();
	}
	Entity SceneHierarchy::GetSelectedEntity() const
	{
		return selection;
	}
}

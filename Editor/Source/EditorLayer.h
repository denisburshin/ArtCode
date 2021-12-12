#pragma once

#include <Ethereal.h>
#include <Panels/SceneHierarchy.h>
#include <Core/Render/EditorCamera.h>

class EditorLayer final : public Ethereal::Layer
{
public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	void OnUpdate() override;
	void OnGUIRender() override;

	void OnEvent(Ethereal::Event& event) override;


	std::unique_ptr<Layer> GetLayer() const override;

private:
	void InterfacePreparation() const;
	bool OnKeyEvent(Ethereal::KeyEvent& event) override;
	bool OnMouseButtonPressed(Ethereal::MousePressedEvent& event);
private:
	std::unique_ptr<Ethereal::Framebuffer> framebuffer;
	std::shared_ptr<Ethereal::Texture2D> texture;
	Ethereal::CameraController controller;

	Ethereal::Entity camera;
	Ethereal::EditorCamera editorCamera;
	std::shared_ptr<Ethereal::Scene> scene;

	glm::vec4 backgroundColor;
	glm::vec2 viewportSize;
	glm::vec2 viewportBounds[2];

	//Guizmo stuff
	int currentGuizmo = -1;

	Ethereal::Entity hoveredEntity;

	bool viewportHovered, enableGrid;

	//Panels
	Ethereal::SceneHierarchy sceneHierarchy;
};
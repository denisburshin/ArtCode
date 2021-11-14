#pragma once

#include <Ethereal.h>
#include <Panels/SceneHierarchy.h>

class EditorLayer final : public Ethereal::Layer
{
public:
	EditorLayer();
	virtual ~EditorLayer() = default;

	void OnUpdate() override;
	void OnGUIRender() override;

	void OnEvent(Ethereal::Event& event) override;

	bool OnKeyEvent(Ethereal::KeyEvent& event) override;

	std::unique_ptr<Layer> GetLayer() const override;

private:
	void InterfacePreparation() const;
private:
	std::unique_ptr<Ethereal::Framebuffer> framebuffer;
	std::shared_ptr<Ethereal::Texture2D> texture;
	Ethereal::CameraController controller;

	Ethereal::Entity camera;

	std::shared_ptr<Ethereal::Scene> scene;

	glm::vec4 backgroundColor;
	glm::vec2 viewportSize;

	//Guizmo stuff
	int currentGuizmo = -1;

	//Panels
	Ethereal::SceneHierarchy sceneHierarchy;
};
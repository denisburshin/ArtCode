#pragma once

#include <Ethereal.h>

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
	std::unique_ptr<Ethereal::Framebuffer> framebuffer;
};
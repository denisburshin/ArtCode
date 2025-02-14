#pragma once
#include <Ethereal.h>

class Scene final : public Ethereal::Layer
{
public:
	Scene();
	virtual ~Scene() = default;

	void OnUpdate() override;
	void OnEvent(Ethereal::Event& event) override;

	bool OnKeyEvent(Ethereal::KeyEvent& event) override;

	void OnGUIRender() override;

	std::unique_ptr<Ethereal::Layer> GetLayer() const override;

private:
	std::shared_ptr<Ethereal::Shader> shader;
	std::shared_ptr<Ethereal::VertexArray> VAO;
	std::shared_ptr<Ethereal::VertexBuffer> VBO;
	std::shared_ptr<Ethereal::IndexBuffer> IBO;
	std::unique_ptr<Ethereal::Texture> texture;

	Ethereal::OrthographicCamera camera;
	glm::vec3 transform, squareColor, scale;
	glm::vec4 bgColor;

	std::string tex_path;
};
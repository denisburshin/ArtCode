#pragma once
#include <Ethereal.h>

class Layer final : public Ethereal::IApplication
{
public:
	Layer();
	~Layer() = default;

	void OnUpdate(Ethereal::Timestep timestep) override;
	void OnEvent(Ethereal::Event& event) override;

	bool OnKeyEvent(Ethereal::KeyEvent& event) override;

private:
	std::shared_ptr<Ethereal::Shader> shader;
	std::shared_ptr<Ethereal::VertexArray> VAO;
	std::shared_ptr<Ethereal::VertexBuffer> VBO;
	std::shared_ptr<Ethereal::IndexBuffer> IBO;

	Ethereal::OrthographicCamera camera;
	glm::vec3 transform;
};
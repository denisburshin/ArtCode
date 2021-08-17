#pragma once
#include <Ethereal.h>

class Application final : public Ethereal::CoreApplication
{
public:
	Application();
	~Application() = default;

	void OnUpdate() override;
private:
	std::shared_ptr<Ethereal::Program> program;
	std::shared_ptr<Ethereal::VertexArray> VAO;
	std::shared_ptr<Ethereal::VertexBuffer> VBO;
	std::shared_ptr<Ethereal::IndexBuffer> IBO;

	Ethereal::OrthographicCamera camera;
};
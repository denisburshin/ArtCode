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
	void InterfacePreparation() const;
private:
	std::unique_ptr<Ethereal::Framebuffer> framebuffer;
	std::shared_ptr<Ethereal::Shader> shader;
	
	Ethereal::OrthographicCamera camera;
	glm::vec3 transform, scale;

	glm::vec4 backgroundColor;

	struct SceneData
	{
		std::shared_ptr<Ethereal::VertexArray> vao;
		std::shared_ptr<Ethereal::VertexBuffer> vbo;
		std::shared_ptr<Ethereal::IndexBuffer> ibo;
		std::shared_ptr<Ethereal::Texture2D> texture;

		std::vector<float> vertices;
	} scene;

	struct Quad
	{
		Quad()
		{
			vertices = {
				-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
				-0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
				0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
				0.5f, 0.5f, 0.5f, 1.0f, 1.0f
			};

			indices = { 0, 1, 2, 2, 3, 1 };
		}

		std::vector<float> vertices;
		std::vector<unsigned int> indices;
	};
};
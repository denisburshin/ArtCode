#include "SceneSerializer.h"
#include <Core/Scene/Entity.h>
#include <Core/Scene/Components.h>
#include <yaml-cpp/yaml.h>
#include <fstream>

namespace YAML
{
	template<>
	struct convert<glm::vec3>
	{
		static Node encode(const glm::vec3& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<glm::vec4>
	{
		static Node encode(const glm::vec4& rhs)
		{
			Node node;
			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);
			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();
			return true;
		}
	};
}

namespace Ethereal
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}

	SceneSerializer::SceneSerializer(const std::shared_ptr<Scene>& scene)
		: scene(scene)
	{
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "1234567890"; //TODO: ENTITY ID

		if (entity.HasComponent<TagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;

			out << YAML::Key << "Tag" << YAML::Value << entity.GetComponent<TagComponent>().tag;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<TransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;

			auto& tc = entity.GetComponent<TransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << tc.Translation;
			out << YAML::Key << "Rotation" << YAML::Value << tc.Rotation;
			out << YAML::Key << "Scale" << YAML::Value << tc.Scale;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<CameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto& cc = entity.GetComponent<CameraComponent>();

			out << YAML::Key << "Camera" << YAML::Value;
			out << YAML::BeginMap;
			out << YAML::Key << "ProjectionType" << YAML::Value << (int)cc.camera.GetProjectionType();
			out << YAML::Key << "PerspectiveFOV" << YAML::Value << cc.camera.GetFOV();
			out << YAML::Key << "PerspectiveNear" << YAML::Value << cc.camera.GetPerspectiveNearClip();
			out << YAML::Key << "PerspectiveFar" << YAML::Value << cc.camera.GetPerspectiveFarClip();
			out << YAML::Key << "OrthoSize" << YAML::Value << cc.camera.GetOrhtoSize();
			out << YAML::Key << "OrthoNear" << YAML::Value << cc.camera.GetOrthoNearClip();
			out << YAML::Key << "OrthoFar" << YAML::Value << cc.camera.GetOrhtoFarClip();
			out << YAML::EndMap;

			out << YAML::Key << "Primary" << YAML::Value << cc.primary;
			out << YAML::Key << "FixedAR" << YAML::Value << cc.fixedAspectRatio;

			out << YAML::EndMap;
		}

		if (entity.HasComponent<SpriteComponent>())
		{
			out << YAML::Key << "SpriteComponent";
			out << YAML::BeginMap;

			auto& sc = entity.GetComponent<SpriteComponent>();
			out << YAML::Key << "Color" << YAML::Value << sc.color;

			out << YAML::EndMap;
		}

		out << YAML::EndMap;
	}

	void SceneSerializer::Serialize(const std::string& path)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene";
		out << YAML::Value << "Name";
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;

		scene->registry.each([&](auto id)
			{
				Entity entity{ id, scene.get() };
				if (!entity)
					return;
				SerializeEntity(out, entity);
			});

		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fout(path);
		fout << out.c_str();
		fout.close();
	}

	bool SceneSerializer::Deserialize(const std::string& path)
	{
		std::ifstream stream(path);
		std::stringstream ss;
		ss << stream.rdbuf();
		stream.close();

		YAML::Node data = YAML::Load(ss.str());
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		auto entities = data["Entities"];
		if (entities)
		{
			for (auto entity : entities)
			{
				uint64_t uuid = entity["Entity"].as<uint64_t>(); //TODO

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
				{
					name = tagComponent["Tag"].as<std::string>();
				}

				Entity deserializedEntity = scene->CreateEntity(name);

				auto tc = entity["TransformComponent"];
				if (tc)
				{
					auto& transformComponent = deserializedEntity.GetComponent<TransformComponent>();
					transformComponent.Translation = tc["Translation"].as<glm::vec3>();
					transformComponent.Rotation = tc["Rotation"].as<glm::vec3>();
					transformComponent.Scale = tc["Scale"].as<glm::vec3>();
				}

				auto cameraComponent = entity["CameraComponent"];
				if (cameraComponent)
				{
					auto& cc = deserializedEntity.AddComponent<CameraComponent>();

					auto& cameraProps = cameraComponent["Camera"];
					cc.camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());

					cc.camera.SetFOV(cameraProps["PerspectiveFOV"].as<float>());
					cc.camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
					cc.camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

					cc.camera.SetOrthoSize(cameraProps["OrthoSize"].as<float>());
					cc.camera.SetOrthoNearClip(cameraProps["OrthoNear"].as<float>());
					cc.camera.SetOrthoFarClip(cameraProps["OrthoFar"].as<float>());
					cc.primary = cameraComponent["Primary"].as<bool>();
					cc.fixedAspectRatio = cameraComponent["FixedAR"].as<bool>();
				}

				auto spriteComponent = entity["SpriteComponent"];
				if (spriteComponent)
				{
					auto& sc = deserializedEntity.AddComponent<SpriteComponent>();
					sc.color = spriteComponent["Color"].as<glm::vec4>();
				}
			}
		}
		return true;
	}

	void SceneSerializer::SerializeRuntime(const std::string& path)
	{
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& path)
	{
		return false;
	}
}
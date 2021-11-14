#pragma once
#include <Core/Scene/Scene.h>

namespace Ethereal
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const std::shared_ptr<Scene>& scene);
		void Serialize(const std::string& path);
		bool Deserialize(const std::string& path);

		void SerializeRuntime(const std::string& path);
		bool DeserializeRuntime(const std::string& path);
	private:
		std::shared_ptr<Scene> scene;
	};
}


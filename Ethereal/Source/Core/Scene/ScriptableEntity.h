#pragma once

#include <Core/Scene/Entity.h>

namespace Ethereal
{
	class ScriptableEntity
	{
	public:
		virtual ~ScriptableEntity() {}
		template <class T>
		T& GetComponent()
		{
			return entity.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnUpdate() {}
		virtual void OnDestroy() {}
	private:
		Entity entity;
		friend class Scene;
	};
}
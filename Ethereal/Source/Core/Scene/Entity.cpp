#include <Core/Scene/Entity.h>

namespace Ethereal
{
	Entity::Entity(entt::entity id, Scene* scene)
		: id(id), scene(scene)
	{

	}
}
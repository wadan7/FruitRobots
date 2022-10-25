#pragma once
#include <vector>
#include <utility>
#include "Entity.h"


namespace FSR {

	struct CollisionDescriptor
	{

		Entity* GetCollidedEntity() { return CollidedEntity; }
		CollisionDescriptor(Entity* entt)
			:CollidedEntity(entt)
		{
		}
	private:
		Entity* CollidedEntity;
	};

}
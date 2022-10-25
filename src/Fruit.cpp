#include "Fruit.h"
#include "Robot.h"

namespace FSR {
	void Fruit::OnUpdate()
	{
	}

	void Fruit::OnCollision(CollisionDescriptor collisionDescriptor)
	{
		Entity* entt = collisionDescriptor.GetCollidedEntity();
		std::string entityName = entt->GetName();
		if (entityName == "Robot")
		{
			Robot* collidedRobot = static_cast<Robot*>(entt);
			m_Payload -= collidedRobot->GetSize();
		}
	}

}
#include "Charger.h"
#include "Robot.h"

namespace FSR {
	void Charger::OnUpdate()
	{
	}

	void Charger::OnCollision(CollisionDescriptor collisionDescriptor)
	{
		Entity* entt = collisionDescriptor.GetCollidedEntity();
		std::string entityName = entt->GetName();
		if (entityName == "Robot")
		{
			Robot* collidedRobot = static_cast<Robot*>(entt);
			collidedRobot->AddCharge();
		}
	} 
}
#include "Robot.h"
#include "Fruit.h"

namespace FSR {
	Robot::Robot(unsigned int size, unsigned int speed, PathPlanner* pp)
		:Actor("Robot", size, { 0, 0 }), m_Speed((float)speed), m_Charge(100), m_MaxCharge(100),m_Busy(false),m_Payload(size), m_PathPlanner(pp)
	{
	}

	void Robot::OnUpdate()
	{
		if (!(IsBusy()))
			return;

		// RUN A* to the target
		path_t path = m_PathPlanner->Run(*this, m_Target);
		if (!path.empty())
		{
			MoveToLocation(path.top());
		}
	}

	void Robot::OnCollision(CollisionDescriptor collisionDescriptor)
	{
		Entity* entt = collisionDescriptor.GetCollidedEntity();
		std::string entityName = entt->GetName();
		if (entityName == "Fruit")
		{
			// respawn the robot 
			this->ReSpawn();
			Fruit* fruit = static_cast<Fruit*>(entt);
			if (fruit->GetPayload() <= 0)
				m_Busy = false;
			else
				m_Busy = true;
			
		}
	}

	void Robot::AddToLocation(std::pair<unsigned int, unsigned int> offset)
	{
		if (AddToActorLocation(offset))
		{
			ConsumeCharge();
		}
	}

	void Robot::MoveToLocation(std::pair<unsigned int, unsigned int> location)
	{
		
		if (MoveActorToLocation(location))
		{
			ConsumeCharge();
		}
	}

	void Robot::PlaceActor(std::pair<unsigned int, unsigned int> location)
	{
		SetRespawnLocation(location);
		MoveActorToLocation(location);
	}


	void Robot::AddCharge()
	{
		if (m_Charge == m_MaxCharge)
			return;

		else if (m_Charge > m_MaxCharge)
			m_Charge = m_MaxCharge;

		else if (m_Charge < m_MaxCharge)
			m_Charge++;
	}
	
	void Robot::ConsumeCharge()
	{
		if (m_Charge > 0)
			m_Charge -= 1.0f / m_Speed;
	}
	void Robot::ReSpawn()
	{
		MoveActorToLocation(m_ReSpawnLocation);
	}
	void Robot::SetRespawnLocation(std::pair<unsigned int, unsigned int> location)
	{
		m_ReSpawnLocation = location;
	}
}
#pragma once
#include "Actor.h"
#include "PathPlanner.h"

namespace FSR {

	class Robot : public Actor
	{

	public:
		Robot(unsigned int size, unsigned int speed, PathPlanner* pp);
		virtual void OnUpdate(); // RUN A* if a task assigned to the robot
		virtual void OnCollision(CollisionDescriptor collisionDescriptor);

		virtual void AddToLocation(std::pair<unsigned int, unsigned int> offset);
		virtual void MoveToLocation(std::pair<unsigned int, unsigned int> location);
		virtual void PlaceActor(std::pair<unsigned int, unsigned int> location);

		inline float GetCurrentCharge() const noexcept { return m_Charge; }
		inline float GetMaxCharge() const noexcept {return m_MaxCharge; }
		inline float GetSpeed() const noexcept { return m_Speed; }
		
		void AddCharge();
		void ConsumeCharge();
		void ReSpawn();

		inline void SetTargetLocation(std::pair<unsigned int, unsigned int> location) noexcept { m_Target = location; m_Busy = true; }
		inline bool IsBusy() const noexcept { return m_Busy; }
		inline void NeedToCharge() { m_NeedToCharge = true; }
		inline bool IsGoingToCharge() const noexcept { return m_NeedToCharge; }
		inline virtual unsigned char GetSymbol() const noexcept { return 'R'; }

	private:
		void SetRespawnLocation(std::pair<unsigned int, unsigned int> location);

	private:
		float m_Charge, m_MaxCharge;
		unsigned int m_Payload;
		float m_Speed;
		std::pair<unsigned int, unsigned int> m_ReSpawnLocation;
		bool m_Busy, m_NeedToCharge;
		std::pair<unsigned int, unsigned int> m_Target;
		PathPlanner* m_PathPlanner;

	};
}
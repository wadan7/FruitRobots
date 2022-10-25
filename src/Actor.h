#pragma once
#include "Entity.h"
#include "Collision.h"

namespace FSR {

	class Actor : public Entity
	{
	public:
		Actor(const std::string& name, unsigned int size, std::pair<unsigned int, unsigned int> location = {0, 0});
		virtual ~Actor();
		
		virtual void OnUpdate() {};
		virtual void OnCollision(CollisionDescriptor collisionDescriptor) {};

		inline const std::pair<unsigned int, unsigned int>& GetLocation() const { return m_Location; }
		inline const unsigned int& GetSize() const { return m_Size; }
		inline void Lock() { m_LockTransform = true; }
		inline void UnLock() { m_LockTransform = false; }
		inline bool IsLocked() const noexcept { return m_LockTransform; }
		
		
		virtual void AddToLocation(std::pair<unsigned int, unsigned int> offset);
		virtual void MoveToLocation(std::pair<unsigned int, unsigned int> location);
		virtual void PlaceActor(std::pair<unsigned int, unsigned int> location);


	protected:
		
		bool AddToActorLocation(std::pair<unsigned int, unsigned int> offset);
		bool MoveActorToLocation(std::pair<unsigned int, unsigned int> location);


	private:
		std::pair<unsigned int, unsigned int> m_Location;
		unsigned int m_Size;
		bool m_LockTransform;
	};

}
#pragma once
#include "Actor.h"


namespace FSR {

	class Fruit : public Actor {
	
	public:
		Fruit(unsigned int payload, unsigned int size = 1)
			:Actor("Fruit", size, { 0, 0 }), m_Payload(payload)
		{
		}

		virtual void OnUpdate();
		virtual void OnCollision(CollisionDescriptor collisionDescriptor);
		inline int GetPayload() const { return m_Payload; }
		inline virtual unsigned char GetSymbol() const noexcept { return 'F'; }

	private:
		int m_Payload;

	};

}
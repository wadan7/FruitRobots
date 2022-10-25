#pragma once
#include "Actor.h"


namespace FSR {

	class Charger : public Actor
	{
	public:

		Charger(unsigned int size = 1)
			:Actor("Charger", size, { 0, 0 })
		{
		}
		
		virtual void OnUpdate();
		virtual void OnCollision(CollisionDescriptor collisionDescriptor);

		inline virtual unsigned char GetSymbol() const noexcept { return 'C'; }

	};
}


#include "Actor.h"

namespace FSR {


	Actor::Actor(const std::string& name, unsigned int size, std::pair<unsigned int, unsigned int> location)
		:Entity(name), m_Location(location), m_Size(size)
	{
	}

	Actor::~Actor()
	{
	}

	bool Actor::AddToActorLocation(std::pair<unsigned int, unsigned int> offset)
	{


		return true;
	}

	void Actor::AddToLocation(std::pair<unsigned int, unsigned int> offset)
	{
		AddToActorLocation(offset);
	}

	bool Actor::MoveActorToLocation(std::pair<unsigned int, unsigned int> location)
	{
		m_Location.first = location.first;
		m_Location.second = location.second;

		return true;
	}

	void Actor::MoveToLocation(std::pair<unsigned int, unsigned int> location)
	{
		MoveActorToLocation(location);
	}

	void Actor::PlaceActor(std::pair<unsigned int, unsigned int> location)
	{
		MoveActorToLocation(location);
	}

}
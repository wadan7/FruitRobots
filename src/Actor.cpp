#include "Actor.h"

namespace FSR {


	Actor::Actor(const std::string& name, unsigned int size, std::pair<unsigned int, unsigned int> location)
		:Entity(name), m_Location(location), m_Size(size), m_LockTransform(false)
	{
	}

	Actor::~Actor()
	{
	}

	bool Actor::AddToActorLocation(std::pair<unsigned int, unsigned int> offset)
	{
		if (IsLocked())
			return false;

		m_Location.first += offset.first;
		m_Location.second += offset.second;

		return true;
	}

	void Actor::AddToLocation(std::pair<unsigned int, unsigned int> offset)
	{
		AddToActorLocation(offset);
	}

	bool Actor::MoveActorToLocation(std::pair<unsigned int, unsigned int> location)
	{
		if (IsLocked())
			return false;

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
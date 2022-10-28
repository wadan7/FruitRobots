#pragma once
#include "Map.h"
#include "Actor.h"
#include "Robot.h"
#include "PathPlanner.h"
#include <stack>


namespace FSR {

	enum class EntityCategory
	{
		None = 0, Robot, Charger, Fruit
	};

	class World
	{
	public:
		World(const std::string& pathToMap);
		void Spawn(Actor* actor, std::pair<unsigned int, unsigned int> location);
		void SpawnFruit(Actor* actor, std::pair<unsigned int, unsigned int> location);
		void OnTick();
		

	private:
		Actor* GetActorInLocation(std::pair<unsigned int, unsigned int> location);
		void OnCollision(Actor* first, Actor* second);
		void DrawWorld();
		void CheckCharge(std::vector<Robot*> robots);
		std::pair<unsigned int, unsigned int> GetFreeChargeLocation();

		template<typename... Args>
		void SignContract(const std::pair<unsigned int, unsigned int>& location,Args... args);
			

	private:
		Map* m_Map;
		PathPlanner* m_PathPlanner;
		std::vector<Actor*> m_Actors;
		std::stack<std::pair<std::pair<unsigned int, unsigned int>, std::pair<unsigned int, unsigned int>>> m_Fruits;
	};

	template<typename... Args>
	inline void World::SignContract(const std::pair<unsigned int, unsigned int>& location, Args... args)
	{
		std::vector<Robot*> actors = { args... };
		
		for (auto robot : actors)
		{
			robot->SetTargetLocation(location);
		}

	}

}
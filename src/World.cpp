#include "World.h"
#include "Robot.h"
#include "Charger.h"
#include "Fruit.h"
#include "Collision.h"
#include "PathPlanner.h"
#include <iostream>



namespace FSR {
	
	World::World(const std::string& pathToMap)
	{
		m_Map = new Map(pathToMap);
		m_PathPlanner = new PathPlanner(m_Map);

		Robot* R1 = new Robot(1, 4, m_PathPlanner);
		Robot* R3 = new Robot(3, 3, m_PathPlanner);
		Robot* R5 = new Robot(5, 2, m_PathPlanner);
		Robot* R7 = new Robot(7, 1, m_PathPlanner);

		Spawn(R1, { 45, 45 });
		Spawn(R3, { 4, 50 });
		Spawn(R5, { 28, 25 });
		Spawn(R7, { 4, 7 });

		Charger* c1 = new Charger(2);
		Charger* c2 = new Charger(2);
		Spawn(c1, { 1, 5 });
		Spawn(c2, { 1, 20 });

		Fruit* f1 = new Fruit(1);
		SpawnFruit(f1, { 30 , 35 });

		Fruit* f2 = new Fruit(6);
		SpawnFruit(f2, { 6 , 60 });

		Fruit* f3 = new Fruit(7);
		SpawnFruit(f3, { 40 , 3 });


	}


	void World::Spawn(Actor* actor, std::pair<unsigned int, unsigned int> location)
	{
		actor->PlaceActor(location);
		m_Actors.push_back(actor);
	}

	void World::SpawnFruit(Actor* factor, std::pair<unsigned int, unsigned int> location)
	{
		Spawn(factor, location);
		int availableLoad = static_cast<Fruit*>(factor)->GetPayload();

		// RSM here
		std::vector<std::pair<Robot*, float>> proposals;

		for (Actor* actor : m_Actors)
		{
			if (actor->GetName() == "Robot")
			{
				Robot* robot = static_cast<Robot*>(actor);
				if (robot->IsBusy())
					continue;

				path_t path = m_PathPlanner->Run(*actor, location);
				if (!path.empty())
				{
					int numberOfTripsNeeded = std::ceil((float)availableLoad / (float)robot->GetSize());
					float tripCost = (1.0f / robot->GetSpeed()) * path.size();
					float totalCost = numberOfTripsNeeded * tripCost;
					proposals.push_back({ robot, totalCost});
				}
			}
		}

		std::sort(proposals.begin(), proposals.end(), [](const std::pair<Robot*, float>& left, const std::pair<Robot*, float>& right)
			{
				return left.second < right.second;
			});

		SignContract(factor->GetLocation(), proposals[0].first);

	}
	
	void World::OnTick()
	{
		CheckCharge({(Robot*)m_Actors[0], (Robot*)m_Actors[1] , (Robot*)m_Actors[2] , (Robot*)m_Actors[3]});

		for (auto actorItrator = m_Actors.begin(); actorItrator != m_Actors.end(); actorItrator++)
		{
			Actor* actor = *actorItrator;
			actor->OnUpdate();
			if (actor->GetName() == "Fruit")
			{
				Fruit* fruit = static_cast<Fruit*>(actor);
				if (fruit->GetPayload() <= 0)
				{
					//delete it and remove it from the actors
					delete actor;
					*actorItrator = nullptr;
				}
			}
		}
		m_Actors.erase(std::remove(m_Actors.begin(), m_Actors.end(), nullptr), m_Actors.end());

		DrawWorld();

	}

	Actor* World::GetActorInLocation(std::pair<unsigned int, unsigned int> location)
	{
		for (Actor* actor : m_Actors)
		{
			if ((location.first >= actor->GetLocation().first && location.first < (actor->GetLocation().first + actor->GetSize())) &&
				(location.second >= actor->GetLocation().second && location.second < (actor->GetLocation().second + actor->GetSize())))
			{
				return actor;
			}
		}
		return nullptr;
	}

	void World::OnCollision(Actor* first, Actor* second)
	{
		CollisionDescriptor fcd(second);
		first->OnCollision(fcd);
		CollisionDescriptor scd(first);
		second->OnCollision(scd);

	}

	void World::DrawWorld()
	{
		auto map = m_Map->Data;

		for (Actor* actor : m_Actors)
		{
			auto tempMap = map;
			// Draw Actor On The Map
			auto& [y, x] = actor->GetLocation();
			unsigned int RightOffset = x + actor->GetSize();
			unsigned int bottomOffset = y + actor->GetSize();

			for (int i = y; i < bottomOffset; i++)
				for (int j = x; j < RightOffset; j++)
				{
					if (tempMap[i][j] != ' ')
					{
						// create collision with this actor and the actor in [i, j]
						Actor* collidee = GetActorInLocation({ i, j });
						OnCollision(actor, collidee);
						continue;
					}
					tempMap[i][j] = actor->GetSymbol();
				}
			map = tempMap;
		}

		for (int i = 0; i < 64; i++)
		{
			std::cout << '\t';
			for (int j = 0; j < 64; j++)
			{
				std::cout << map[i][j];
			}
			std::cout << '\n';
		}

		std::cout << '\n';
		std::cout << "        [ Robot 1 ] "  << (((Robot*)m_Actors[0])->IsBusy() ? "Busy  " : "Free  ") << "Charge: " << ((Robot*)m_Actors[0])->GetCurrentCharge() << '\n';
		std::cout << "        [ Robot 3 ] " << (((Robot*)m_Actors[1])->IsBusy() ? "Busy  " : "Free  ") << "Charge: " << ((Robot*)m_Actors[1])->GetCurrentCharge() << '\n';
		std::cout << "        [ Robot 5 ] " << (((Robot*)m_Actors[2])->IsBusy() ? "Busy  " : "Free  ") << "Charge: " << ((Robot*)m_Actors[2])->GetCurrentCharge() << '\n';
		std::cout << "        [ Robot 7 ] " << (((Robot*)m_Actors[3])->IsBusy() ? "Busy  " : "Free  ") << "Charge: " << ((Robot*)m_Actors[3])->GetCurrentCharge() << '\n';
		std::cout << '\n';

	}
	void World::CheckCharge(std::vector<Robot*> robots)
	{
		//RCM
		// Set Robot Target To Any Free Charger Location if needs to charge
		std::sort(robots.begin(), robots.end(), [](const Robot* left, const Robot* right)
			{
				return left->GetCurrentCharge() < right->GetCurrentCharge();
			});
		
		for (Robot* robot : robots)
		{
			if (!robot->IsBusy() && !robot->IsGoingToCharge() && (robot->GetCurrentCharge() / robot->GetMaxCharge() < 0.6f))
			{
				std::pair chargerLocation = GetFreeChargeLocation();
				if (chargerLocation.first == 0 && chargerLocation.second == 0)
					return;
				robot->SetTargetLocation(chargerLocation);
				robot->NeedToCharge();
				
			}
		}


	}
	std::pair<unsigned int, unsigned int> World::GetFreeChargeLocation()
	{
		for (std::vector<Actor*>::iterator it = m_Actors.begin() + 4; it != m_Actors.begin() + 6; it++)
		{
			Charger* charger = static_cast<Charger*>(*it);
			if (charger->IsFree())
			{
				return charger->GetLocation();
				charger->SetBusy();
			}
		}
		return { 0, 0 };
	}
}
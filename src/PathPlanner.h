#pragma once
#include "Actor.h"
#include "Map.h"
#include <vector>
#include <queue>
#include <stack>
#include<utility>


namespace FSR {

	using path_t = std::stack<std::pair<unsigned int, unsigned int>>;

	class PathPlanner
	{
	public:
		PathPlanner(Map* map);
		path_t Run(const Actor& robot, std::pair<unsigned int, unsigned int> destination);

	private:
		float FindHeuristic(const std::pair<unsigned int, unsigned int>& firstPoint, const std::pair<unsigned int, unsigned int>& secondPoint);
		std::vector<std::pair<unsigned int, unsigned int>> GetAvailableMoves(const std::pair<unsigned int, unsigned int>& location, unsigned int robotsize);
		bool IsSafeMove(const std::pair<unsigned int, unsigned int>& move, unsigned int robotsize);
		void CleanUp();

		struct Node
		{
			Node* ParnetNode;
			std::pair<unsigned int, unsigned int> Location;
			float CostFromRobot;
			float TotalCost;
			
			Node(Node* parent, std::pair<unsigned int, unsigned int> move)
				:ParnetNode(parent), Location(move)
			{
				if (ParnetNode == nullptr)
				{
					CostFromRobot = 0;
				}
				else
				{
					CostFromRobot = parent->CostFromRobot + 1;

				}
				TotalCost = 0;
			}
			Node(Node* parent, std::pair<unsigned int, unsigned int> move, float heuristisc)
				:ParnetNode(parent), Location(move)
			{
				if (ParnetNode == nullptr)
				{
					CostFromRobot = 0;
				}
				else
				{
					CostFromRobot = parent->CostFromRobot + 1;

				}
				TotalCost = CostFromRobot + heuristisc;
			}

			bool operator==(Node& other)
			{
				return this->Location.first == other.Location.first && this->Location.second == other.Location.second;
			}
		};

		struct Greater
		{
			bool operator()(const Node* left, const Node* right) const
			{
				return left->TotalCost > right->TotalCost;
			}
		};

	private:
		Map* m_Map;
		std::priority_queue<Node*, std::vector<Node*>, Greater> m_OpenList;
		std::vector<Node*> m_ClosedList;
		bool Visited[64][64] = {};
	};

} 
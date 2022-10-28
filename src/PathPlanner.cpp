#include "PathPlanner.h"
#include <cmath>
#include <array>
#include <algorithm>


namespace FSR {

	std::array<std::pair<int,int>, 4> PosibleTransitions =
	{
		{
			{1, 0},
			{0, 1},
			{-1, 0},
			{0, -1},
		}
	};


	PathPlanner::PathPlanner(Map* map)
		:m_Map(map)
	{
	}

	path_t PathPlanner::Run(const Actor& robot, std::pair<unsigned int, unsigned int> destination)
	{
		for (int i = 0; i < 64; i++)
			for (int j = 0; j < 64; j++)
				Visited[i][j] = false;

		Node* root = new Node(nullptr, robot.GetLocation(), FindHeuristic(robot.GetLocation(), destination));
		Node* current = root;
		Visited[root->Location.first][root->Location.second] = true;
		m_OpenList.push(root);

		while (!m_OpenList.empty())
		{
			current = m_OpenList.top();
			m_OpenList.pop();

			if ( (destination.first >= current->Location.first && destination.first < (current->Location.first + robot.GetSize())) &&
				 (destination.second >= current->Location.second && destination.second < (current->Location.second + robot.GetSize())) )
			{
				path_t path;
					while (current->ParnetNode != nullptr)
					{
						path.push(current->Location);
							current = current->ParnetNode;
					}
				CleanUp();
					return path;
			}
			/*
			else if (current->Location == destination)
			{
				path_t path;
				while (current->ParnetNode != nullptr)
				{
					path.push(current->Location);
					current = current->ParnetNode;
				}
				CleanUp();
				return path;
			}
			*/

			std::vector<std::pair<unsigned int, unsigned int>> availableMoves = GetAvailableMoves(current->Location, robot.GetSize());
			
			for (const std::pair<unsigned int, unsigned int>& move : availableMoves)
			{
				Node* node = new Node(current, move, FindHeuristic(move, destination));
				m_OpenList.push(node);
			}
			m_ClosedList.push_back(current);
		}
		CleanUp();
		return path_t();
	}

	float PathPlanner::FindHeuristic(const std::pair<unsigned int, unsigned int>& firstPoint, const std::pair<unsigned int, unsigned int>& secondPoint)
	{
		int a = (firstPoint.first - secondPoint.first);
		int b = (firstPoint.second - secondPoint.second);
		return std::sqrt(a * a + b * b);
	}

	std::vector<std::pair<unsigned int, unsigned int>> PathPlanner::GetAvailableMoves(const std::pair<unsigned int, unsigned int>& location, unsigned int robotsize)
	{
		std::vector<std::pair<unsigned int, unsigned int>> moves;

		for (const std::pair<int, int>& transition : PosibleTransitions)
		{
			std::pair<unsigned int, unsigned int> move = { transition.first + location.first, transition.second + location.second };
			if (Visited[move.first][move.second] == true)
				continue;

			Visited[move.first][move.second] = true;

			if (IsSafeMove(move, robotsize))
			{
				moves.push_back(move);
			}
		}
		return moves;
	}

	bool PathPlanner::IsSafeMove(const std::pair<unsigned int, unsigned int>& move, unsigned int robotSize)
	{

		auto& [y, x] = move;
		unsigned int RightOffset = x + robotSize;
		unsigned int bottomOffset = y + robotSize;

		for (unsigned int i = y; i < bottomOffset; i++)
			for (unsigned int j = x; j < RightOffset; j++)
			{
				if (m_Map->Data[i][j] == 219)
				{
					return false;
				}
			}
		return true;
	}

	void PathPlanner::CleanUp()
	{
		for (Node* node : m_ClosedList)
		{
			delete node;
		}
		m_ClosedList.clear();

		while (!m_OpenList.empty())
		{
			Node* top = m_OpenList.top();
			m_OpenList.pop();
			delete top;
		}
	}

}
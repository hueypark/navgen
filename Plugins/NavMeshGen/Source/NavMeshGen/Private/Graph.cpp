#include "Graph.h"

#include "Node.h"

#include <unordered_set>

Graph::Graph()
{
}

void Graph::AddNode(std::shared_ptr<Node> node)
{
	if (!node)
	{
		// TODO: 로그 추가

		return;
	}

	auto it = m_nodes.emplace(node->GetID(), node);
	if (!it.second)
	{
		// TODO: 로그 추가
	}

	auto itNodePerLocation = m_nodePerLocations.emplace(std::make_pair(node->GetX(), node->GetY()), node);
	if (!itNodePerLocation.second)
	{
		// TODO: 로그 추가
	}
}

void Graph::AddEdge(const int32_t from, const int32_t to)
{
	m_edges[from].emplace(to);
}

void Graph::Clear()
{
	m_nodes.clear();
	m_nodePerLocations.clear();
	m_edges.clear();
}

std::vector<int32_t> Graph::FindPath(const int32_t from, const int32_t to)
{
	if (from == to)
	{
		return std::vector<int32_t>{from, to};
	}

	std::unordered_set<int32_t> targetNodes;
	targetNodes.emplace(from);

	std::unordered_set<int32_t> visitedNodes;
	visitedNodes.emplace(from);

	std::unordered_map<int32_t, std::vector<int32_t>> pathPerNode;

	while (!targetNodes.empty())
	{
		const int32_t targetNode = *targetNodes.begin();
		targetNodes.erase(targetNode);

		pathPerNode[targetNode].emplace_back(targetNode);

		if (targetNode == to)
		{
			return pathPerNode[targetNode];
		}

		for (const int32_t edge : m_edges[targetNode])
		{
			if (visitedNodes.find(edge) == visitedNodes.end())
			{
				visitedNodes.emplace(edge);
				targetNodes.emplace(edge);

				pathPerNode[edge] = pathPerNode[targetNode];
			}
		}
	}

	return std::vector<int32_t>();
}

std::shared_ptr<Node> Graph::GetNode(const int32_t id) const
{
	auto it = m_nodes.find(id);
	if (it == m_nodes.end())
	{
		return nullptr;
	}

	return it->second;
}

std::shared_ptr<Node> Graph::GetNode(const int32_t x, const int32_t y) const
{
	auto key = std::make_pair(x, y);

	auto it = m_nodePerLocations.find(key);
	if (it == m_nodePerLocations.end())
	{
		return nullptr;
	}

	return it->second;
}

const std::unordered_map<int32_t, std::shared_ptr<Node>>& Graph::GetNodes() const
{
	return m_nodes;
}

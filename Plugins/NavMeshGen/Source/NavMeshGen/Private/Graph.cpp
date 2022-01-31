#include "Graph.h"

#include "Node.h"

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

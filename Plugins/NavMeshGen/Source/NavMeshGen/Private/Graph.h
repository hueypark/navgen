#pragma once

class Node;

#include <cstdint>
#include <memory>
#include <unordered_map>

// Graph 는 그래프를 표현합니다.
class Graph
{
public:
	// 생성자
	Graph();

	// 노드를 추가합니다.
	void AddNode(std::shared_ptr<Node> node);

	// 노드를 반환합니다.
	std::shared_ptr<Node> GetNode(const int32_t id) const;

private:
	// 노드들
	std::unordered_map<int32_t, std::shared_ptr<Node>> m_nodes;
};

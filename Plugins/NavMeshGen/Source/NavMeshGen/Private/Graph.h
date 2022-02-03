#pragma once

class Node;

#include <cstdint>
#include <map>
#include <memory>
#include <unordered_map>
#include <unordered_set>

// Graph 는 그래프를 표현합니다.
class Graph
{
public:
	// 생성자
	Graph();

	// 노드를 추가합니다.
	void AddNode(std::shared_ptr<Node> node);

	// 엣지를 추가합니다.
	void AddEdge(const int32_t from, const int32_t to);

	// 데이터를 지웁니다.
	void Clear();

	// 경로를 찾습니다.
	std::vector<int32_t> FindPath(const int32_t from, const int32_t to);

	// 노드를 반환합니다.
	std::shared_ptr<Node> GetNode(const int32_t id) const;

	// 노드를 반환합니다.
	std::shared_ptr<Node> GetNode(const int32_t x, const int32_t y) const;

	// 노드들을 반환합니다.
	const std::unordered_map<int32_t, std::shared_ptr<Node>>& GetNodes() const;

private:
	// 노드들
	std::unordered_map<int32_t, std::shared_ptr<Node>> m_nodes;

	// 위치값별 노드들
	std::map<std::pair<int32, int32>, std::shared_ptr<Node>> m_nodePerLocations;

	// 엣지들
	std::unordered_map<int32_t, std::unordered_set<int32_t>> m_edges;
};

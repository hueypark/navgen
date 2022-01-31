#pragma once

#include <cstdint>

// Node 는 노드를 표현합니다.
class Node
{
public:
	// 생성자
	Node(const int32_t id);

	// ID 를 반환합니다.
	int32_t GetID() const;

private:
	// ID
	int32_t m_id;
};

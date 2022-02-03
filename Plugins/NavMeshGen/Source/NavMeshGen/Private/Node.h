#pragma once

#include <cstdint>

// Node 는 노드를 표현합니다.
class Node
{
public:
	// 생성자
	Node(const int32_t id, const int32 x, const int32 y, const float height);

	// ID 를 반환합니다.
	int32_t GetID() const;

	// X 좌표를 반환합니다.
	int32_t GetX() const;

	// Y 좌표를 반환합니다.
	int32_t GetY() const;

	// 높이를 반환합니다.
	float GetHeight() const;

private:
	// ID
	int32_t m_id;

	// X 좌표
	const int32 m_x;

	// Y 좌표
	const int32 m_y;

	// 높이
	const float m_height;
};

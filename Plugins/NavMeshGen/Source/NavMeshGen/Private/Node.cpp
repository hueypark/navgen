#include "Node.h"

Node::Node(const int32_t id, const int32 x, const int32 y, const float height)
	: m_id(id), m_x(x), m_y(y), m_height(height)
{
}

int32_t Node::GetID() const
{
	return m_id;
}

int32_t Node::GetX() const
{
	return m_x;
}

int32_t Node::GetY() const
{
	return m_y;
}

float Node::GetHeight() const
{
	return m_height;
}

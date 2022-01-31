#include "Node.h"

Node::Node(const int32_t id) : m_id(id)
{
}

int32_t Node::GetID() const
{
	return m_id;
}

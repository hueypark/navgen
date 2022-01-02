#include "HueyNavHeightfield.h"

void HueyNavHeightfield::Add(const int32 x, const int32 y, const float height)
{
	m_heights.emplace(std::make_pair(x, y), height);
}

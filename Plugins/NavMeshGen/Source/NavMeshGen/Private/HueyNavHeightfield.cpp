#include "HueyNavHeightfield.h"

void HueyNavHeightfield::Add(const int32 x, const int32 y, const float height)
{
	m_heights.emplace(std::make_pair(x, y), height);
}

void HueyNavHeightfield::Clear()
{
	m_heights.clear();
}

const std::map<std::pair<int32, int32>, float>& HueyNavHeightfield::GetHeights() const
{
	return m_heights;
}

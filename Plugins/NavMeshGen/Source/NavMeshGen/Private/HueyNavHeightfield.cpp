#include "HueyNavHeightfield.h"

void HueyNavHeightfield::Add(const int32 x, const int32 y, const float height)
{
	m_heights.emplace(std::make_pair(x, y), height);
}

void HueyNavHeightfield::Clear()
{
	m_heights.clear();
}

float HueyNavHeightfield::GetHeight(const int32 x, const int32 y) const
{
	auto key = std::make_pair(x, y);

	auto it = m_heights.find(key);
	if (it == m_heights.end())
	{
		return 0.0f;
	}

	return it->second;
}

const std::map<std::pair<int32, int32>, float>& HueyNavHeightfield::GetHeights() const
{
	return m_heights;
}

bool HueyNavHeightfield::HasValue(const int32 x, const int32 y) const
{
	auto key = std::make_pair(x, y);

	return m_heights.find(key) != m_heights.end();
}

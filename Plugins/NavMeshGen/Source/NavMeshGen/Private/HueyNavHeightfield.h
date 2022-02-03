#pragma once

#include <map>

// HueyNavHeightfield 는 필드의 높이값을 표현합니다.
class HueyNavHeightfield
{
public:
	// 높이값을 추가합니다.
	void Add(const int32 x, const int32 y, const float height);

	// 데이터를 지웁니다.
	void Clear();

	// 높이값을 반환합니다.
	float GetHeight(const int32 x, const int32 y) const;

	// 높이값들을 반환합니다.
	const std::map<std::pair<int32, int32>, float>& GetHeights() const;

	// 값 보유여부를 반환합니다.
	bool HasValue(const int32 x, const int32 y) const;

private:
	// 높이값들
	std::map<std::pair<int32, int32>, float> m_heights;
};

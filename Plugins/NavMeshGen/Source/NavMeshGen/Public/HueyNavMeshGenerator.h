#pragma once

#include "AI/NavDataGenerator.h"
#include "HueyNavHeightfield.h"

class AHueyNavMesh;

class FHueyNavMeshGenerator : public FNavDataGenerator
{
public:
	// 생성자
	FHueyNavMeshGenerator(AHueyNavMesh& ownerNavMesh);

	// 초기화합니다.
	void Init();

	// 모두 리빌드
	virtual bool RebuildAll() override;

	// 틱마다 비동기 빌드 시도
	virtual void TickAsyncBuild(float DeltaSeconds) override;

	// 네비게이션 바운드가 변경되었을 때 처리
	virtual void OnNavigationBoundsChanged() override;

	// 변경된 부분만 리빌드
	virtual void RebuildDirtyAreas(const TArray<FNavigationDirtyArea>& DirtyAreas) override;

private:
	// 네비게이션 바운드를 갱신합니다.
	void _UpdateNavigationBounds();

private:
	// 생성기를 소유한 내비게이션 메쉬
	AHueyNavMesh& m_ownerNavMesh;

	// 모든 네비게이션 구역의 합
	FBox m_totalNavBound;

	// 네비게이션 구역들
	TNavStatArray<FBox> m_navBounds;

	// 하이트필드
	HueyNavHeightfield m_heightfield;

private:
	// 타일 사이즈
	static const float s_tileSize;

	// 정수형 타일 사이즈
	static const int32 s_tileSizeInt;
};

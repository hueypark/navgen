#pragma once

#include "HueyNavHeightfield.h"
#include "NavigationData.h"

#include "HueyNavMesh.generated.h"

UCLASS()
class AHueyNavMesh : public ANavigationData
{
	GENERATED_BODY()

public:
	// 네비게이션 데이터를 추가합니다.
	void AddNavData(const int32 x, const int32 y, const float height);

	// 네비게이션 데이터를 지웁니다.
	void ClearNavData();

	// 네비게이션 생성기 생성
	virtual void ConditionalConstructGenerator() override;

	// 렌더링 컴포넌트 생성
	virtual UPrimitiveComponent* ConstructRenderingComponent() override;

	// 하이트필드를 반환합니다.
	const HueyNavHeightfield& GetHeightField() const;

	// 네비게이션 메시 생성이 종료될 떄 호출됩니다.
	virtual void OnNavMeshGenerationFinished();

private:
	// 하이트필드
	HueyNavHeightfield m_heightfield;
};

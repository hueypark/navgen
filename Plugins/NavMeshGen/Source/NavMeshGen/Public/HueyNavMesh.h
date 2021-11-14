#pragma once

#include "NavigationData.h"

#include "HueyNavMesh.generated.h"

UCLASS()
class AHueyNavMesh : public ANavigationData
{
	GENERATED_BODY()

public:
	// 네비게이션 생성기 생성
	virtual void ConditionalConstructGenerator() override;

	// 렌더링 컴포넌트 생성
	virtual UPrimitiveComponent* ConstructRenderingComponent() override;
};

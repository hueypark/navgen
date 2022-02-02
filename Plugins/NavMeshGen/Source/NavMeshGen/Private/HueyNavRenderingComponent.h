#pragma once

#include "Components/PrimitiveComponent.h"

#include "HueyNavRenderingComponent.generated.h"

UCLASS()
class UHueyNavRenderingComponent : public UPrimitiveComponent
{
	GENERATED_BODY()

public:
	// 씬에 표현될 프록시를 만듭니다.
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;

	// 컴포넌트의 영역을 계산합니다.
	//
	// 렌더링에 포함할지 판정할 때 사용됩니다.
	virtual FBoxSphereBounds CalcBounds(const FTransform& LocalToWorld) const override;
};

#pragma once

#include "DebugRenderSceneProxy.h"
#include "HueyNavSceneProxyData.h"

// FHueyNavSceneProxy 네비게이션 씬 프록시입니다.
class FHueyNavSceneProxy final : public FDebugRenderSceneProxy
{
public:
	// 생성자
	FHueyNavSceneProxy(const UPrimitiveComponent* component, const FHueyNavSceneProxyData& data);

	// ViewRelevance 를 반환합니다.
	//
	// ViewRelevance 는 씬에 보일지에 관한 관련성을 표현하는 값입니다.
	virtual FPrimitiveViewRelevance GetViewRelevance(const FSceneView* View) const override;

	// 데이터를 설정합니다.
	void SetData(const FHueyNavSceneProxyData& data);

private:
	// 씬 프록시 데이터
	//
	// 씬에 렌더링하기 위한 데이터
	FHueyNavSceneProxyData m_data;
};

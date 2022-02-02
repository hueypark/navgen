#pragma once

#include "DebugRenderSceneProxy.h"

// FHueyNavSceneProxyData 네비게이션 씬 프록시를 위한 데이터입니다.
struct FHueyNavSceneProxyData
{
	// 박스들
	TArray<FDebugRenderSceneProxy::FDebugBox> Boxes;
};

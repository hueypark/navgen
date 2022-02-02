#include "HueyNavSceneProxy.h"

FHueyNavSceneProxy::FHueyNavSceneProxy(const UPrimitiveComponent* component, const FHueyNavSceneProxyData& data)
	: FDebugRenderSceneProxy(component) //, VertexFactory(GetScene().GetFeatureLevel(), "FHueyNavSceneProxy")
{
	DrawType = EDrawType::SolidAndWireMeshes;

	SetData(data);
}

FPrimitiveViewRelevance FHueyNavSceneProxy::GetViewRelevance(const FSceneView* View) const
{
	const bool bVisible = View->Family->EngineShowFlags.Navigation;

	FPrimitiveViewRelevance result;
	result.bDrawRelevance = bVisible && IsShown(View);
	result.bDynamicRelevance = true;
	result.bSeparateTranslucency = result.bNormalTranslucency = bVisible && IsShown(View);
	return result;
}

void FHueyNavSceneProxy::SetData(const FHueyNavSceneProxyData& data)
{
	m_data = data;

	Boxes = m_data.Boxes;
}

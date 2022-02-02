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

	Boxes.Empty();

	const FBox samllBox(FVector(-500.0f, -500.0f, -500.0f), FVector(500.0f, 500.0f, 500.0f));
	Boxes.Add(FDebugRenderSceneProxy::FDebugBox(samllBox, FColor::Green, FTransform::Identity));

	const FBox box(FVector(-5000.0f, -5000.0f, -5000.0f), FVector(5000.0f, 5000.0f, 5000.0f));
	Boxes.Add(FDebugRenderSceneProxy::FDebugBox(box, FColor::Red, FTransform::Identity));
}

#include "HueyNavRenderingComponent.h"

#include "HueyNavMesh.h"
#include "HueyNavSceneProxy.h"
#include "NavigationSystem.h"

FPrimitiveSceneProxy* UHueyNavRenderingComponent::CreateSceneProxy()
{
#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
	FHueyNavSceneProxy* hueyNavSceneProxy = nullptr;

	FHueyNavSceneProxyData proxyData;

	hueyNavSceneProxy = new FHueyNavSceneProxy(this, proxyData);

	return hueyNavSceneProxy;
#else
	return nullptr;
#endif
}

FBoxSphereBounds UHueyNavRenderingComponent::CalcBounds(const FTransform& LocalToWorld) const
{
	FBox boundingBox(ForceInit);

	if (AHueyNavMesh* navMesh = Cast<AHueyNavMesh>(GetOwner()))
	{
		if (const UNavigationSystemV1* navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
		{
			TArray<FBox> navBounds;
			navSys->GetNavigationBoundsForNavData(*navMesh, navBounds);

			for (const FBox& bound : navBounds)
			{
				boundingBox += bound;
			}
		}
	}

	return FBoxSphereBounds(boundingBox);
}

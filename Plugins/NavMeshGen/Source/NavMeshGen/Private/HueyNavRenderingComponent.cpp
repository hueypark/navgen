#include "HueyNavRenderingComponent.h"

#include "HueyNavMesh.h"
#include "HueyNavMeshGenerator.h"
#include "HueyNavSceneProxy.h"
#include "NavigationSystem.h"

FPrimitiveSceneProxy* UHueyNavRenderingComponent::CreateSceneProxy()
{
#if !UE_BUILD_SHIPPING && !UE_BUILD_TEST
	FHueyNavSceneProxy* hueyNavSceneProxy = nullptr;

	if (const AHueyNavMesh* navMesh = Cast<AHueyNavMesh>(GetOwner()))
	{
		FHueyNavSceneProxyData proxyData;
		_GatherData(*navMesh, proxyData);

		hueyNavSceneProxy = new FHueyNavSceneProxy(this, proxyData);
	}

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

void UHueyNavRenderingComponent::_GatherData(const AHueyNavMesh& hueyNavMesh, FHueyNavSceneProxyData& proxyData)
{
	for (const auto& iter : hueyNavMesh.GetHeightField().GetHeights())
	{
		const std::pair<int32, int32>& location = iter.first;
		const float height = iter.second;

		const FBox box(
			FVector(-FHueyNavMeshGenerator::GetTileSize() * 0.5f, -FHueyNavMeshGenerator::GetTileSize() * 0.5f, 10.0f),
			FVector(FHueyNavMeshGenerator::GetTileSize() * 0.5f, FHueyNavMeshGenerator::GetTileSize() * 0.5f, 10.0f));

		proxyData.Boxes.Add(FDebugRenderSceneProxy::FDebugBox(
			box, FColor::Green, FTransform(FVector(float(location.first), float(location.second), height))));
	}
}

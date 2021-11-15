#include "HueyNavMesh.h"

#include "AI/NavDataGenerator.h"
#include "HueyNavMeshGenerator.h"
#include "NavMesh/NavMeshRenderingComponent.h"
#include "NavigationSystem.h"

void AHueyNavMesh::ConditionalConstructGenerator()
{
	if (NavDataGenerator.IsValid())
	{
		NavDataGenerator->CancelBuild();
		NavDataGenerator.Reset();
	}

	UWorld* world = GetWorld();
	check(world);
	const bool bRequiresGenerator = SupportsRuntimeGeneration() || !world->IsGameWorld();
	if (bRequiresGenerator)
	{
		TSharedPtr<FHueyNavMeshGenerator, ESPMode::ThreadSafe> generator =
			MakeShared<FHueyNavMeshGenerator, ESPMode::ThreadSafe>(*this);

		generator->Init();

		NavDataGenerator = generator;

		UNavigationSystemV1* navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(world);
		if (navSys)
		{
			RestrictBuildingToActiveTiles(navSys->IsActiveTilesGenerationEnabled());
		}
	}
}

UPrimitiveComponent* AHueyNavMesh::ConstructRenderingComponent()
{
	return NewObject<UNavMeshRenderingComponent>(this, TEXT("NavRenderingComp"), RF_Transient);
}

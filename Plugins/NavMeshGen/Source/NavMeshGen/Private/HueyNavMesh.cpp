#include "HueyNavMesh.h"

#include "AI/NavDataGenerator.h"
#include "HueyNavMeshGenerator.h"
#include "HueyNavRenderingComponent.h"
#include "NavMesh/NavMeshPath.h"
#include "NavMesh/NavMeshRenderingComponent.h"
#include "NavigationSystem.h"

AHueyNavMesh::AHueyNavMesh()
{
	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		FindPathImplementation = FindPath;
	}
}

void AHueyNavMesh::AddNavData(const int32 x, const int32 y, const float height)
{
	m_heightfield.Add(x, y, height);
}

void AHueyNavMesh::ClearNavData()
{
	m_heightfield.Clear();
}

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
	return NewObject<UHueyNavRenderingComponent>(this, TEXT("NavRenderingComp"), RF_Transient);
}

const HueyNavHeightfield& AHueyNavMesh::GetHeightField() const
{
	return m_heightfield;
}

void AHueyNavMesh::OnNavMeshGenerationFinished()
{
	if (!RenderingComp)
	{
		return;
	}

	RenderingComp->MarkRenderStateDirty();
}

FPathFindingResult AHueyNavMesh::FindPath(const FNavAgentProperties& agentProperties, const FPathFindingQuery& query)
{
	FPathFindingResult result(ENavigationQueryResult::Error);

	const ANavigationData* self = query.NavData.Get();
	check(Cast<const AHueyNavMesh>(self));

	FNavigationPath* navPath = query.PathInstanceToFill.Get();
	FNavMeshPath* navMeshPath = navPath ? navPath->CastPath<FNavMeshPath>() : nullptr;

	if (navMeshPath)
	{
		result.Path = query.PathInstanceToFill;
		navMeshPath->ResetForRepath();
	}
	else
	{
		result.Path = self->CreatePathInstance<FNavMeshPath>(query);
		navPath = result.Path.Get();
		navMeshPath = navPath ? navPath->CastPath<FNavMeshPath>() : nullptr;
	}

	if (navMeshPath)
	{
		navMeshPath->ApplyFlags(query.NavDataFlags);

		result.Path->GetPathPoints().Add(FNavPathPoint(FVector(query.StartLocation)));
		result.Path->GetPathPoints().Add(FNavPathPoint(FVector(query.EndLocation)));
	}

	result.Path->MarkReady();
	result.Result = ENavigationQueryResult::Success;

	return result;
}

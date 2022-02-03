#include "HueyNavMesh.h"

#include "AI/NavDataGenerator.h"
#include "HueyNavMeshGenerator.h"
#include "HueyNavRenderingComponent.h"
#include "NavMesh/NavMeshPath.h"
#include "NavMesh/NavMeshRenderingComponent.h"
#include "NavigationSystem.h"
#include "Node.h"

AHueyNavMesh::AHueyNavMesh()
{
	m_graph = MakeShared<Graph>();

	if (HasAnyFlags(RF_ClassDefaultObject) == false)
	{
		FindPathImplementation = FindPath;

		const AHueyNavMesh* defaultObject = GetDefault<AHueyNavMesh>();
		if (defaultObject && defaultObject->m_graph)
		{
			m_graph = defaultObject->m_graph;
		}
	}
}

void AHueyNavMesh::AddNavData(const int32 id, const int32 x, const int32 y, const float height)
{
	m_graph->AddNode(std::make_shared<Node>(id, x, y, height));
}

void AHueyNavMesh::ClearNavData()
{
	m_graph->Clear();
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

const Graph& AHueyNavMesh::GetGraph() const
{
	return *m_graph;
}

void AHueyNavMesh::OnNavMeshGenerationFinished()
{
	if (!RenderingComp)
	{
		return;
	}

	RenderingComp->MarkRenderStateDirty();

	FBox boundingBox(ForceInit);

	if (const UNavigationSystemV1* navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
	{
		TArray<FBox> navBounds;
		navSys->GetNavigationBoundsForNavData(*this, navBounds);

		for (const FBox& bound : navBounds)
		{
			boundingBox += bound;
		}
	}

	int32_t startX = boundingBox.Min.X;
	int32_t startY = boundingBox.Min.Y;
	int32_t endX = boundingBox.Max.X;
	int32_t endY = boundingBox.Max.Y;

	for (int32_t x = startX; x < endX; x += FHueyNavMeshGenerator::GetTileSize())
	{
		for (int32_t y = startY; y < endY; y += FHueyNavMeshGenerator::GetTileSize())
		{
			// 근처 4 타일 검사한 후 높이차가 일정 값 이하면 엣지 추가
			std::vector<std::pair<int32, int32>> neighberOffsets = {
				{0, 1},
				{-1, 0},
				{1, 0},
				{0, -1},
			};

			std::shared_ptr<Node> node = m_graph->GetNode(x, y);
			if (!node)
			{
				continue;
			}

			for (const std::pair<int32, int32>& neighberOffset : neighberOffsets)
			{
				int32_t neighborX = x + (neighberOffset.first * int32_t(FHueyNavMeshGenerator::GetTileSize()));
				int32_t neighborY = y + (neighberOffset.second * int32_t(FHueyNavMeshGenerator::GetTileSize()));

				std::shared_ptr<Node> neighbor = m_graph->GetNode(neighborX, neighborY);
				if (!neighbor)
				{
					continue;
				}

				if (FMath::Abs(node->GetHeight() - neighbor->GetHeight()) <= 500.0f)
				{
					m_graph->AddEdge(node->GetID(), neighbor->GetID());
				}
			}
		}
	}
}

FPathFindingResult AHueyNavMesh::FindPath(const FNavAgentProperties& agentProperties, const FPathFindingQuery& query)
{
	FPathFindingResult result(ENavigationQueryResult::Error);

	const AHueyNavMesh* self = Cast<const AHueyNavMesh>(query.NavData.Get());
	check(self);

	if (!self)
	{
		return result;
	}

	FNavigationPath* navPath = query.PathInstanceToFill.Get();
	FNavMeshPath* navMeshPath = navPath ? navPath->CastPath<FNavMeshPath>() : nullptr;

	int32_t startX = int32_t(query.StartLocation.X) + 500;
	startX -= (startX % 1000);

	int32_t startY = int32_t(query.StartLocation.Y) + 500;
	startY -= (startY % 1000);

	int32_t endX = int32_t(query.EndLocation.X) + 500;
	endX -= (endX % 1000);

	int32_t endY = int32_t(query.EndLocation.Y) + 500;
	endY -= (endY % 1000);

	std::shared_ptr<Node> startNode = self->m_graph->GetNode(startX, startY);
	std::shared_ptr<Node> endNode = self->m_graph->GetNode(endX, endY);

	if (!startNode || !endNode)
	{
		return result;
	}

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

		std::vector<int32_t> path = self->m_graph->FindPath(startNode->GetID(), endNode->GetID());

		for (const int32_t nodeID : path)
		{
			const std::shared_ptr<Node> node = self->m_graph->GetNode(nodeID);
			if (!node)
			{
				// TODO: 로그 추가

				continue;
			}

			result.Path->GetPathPoints().Add(FNavPathPoint(FVector(node->GetX(), node->GetY(), node->GetHeight())));
		}
	}

	result.Path->MarkReady();
	result.Result = ENavigationQueryResult::Success;

	return result;
}

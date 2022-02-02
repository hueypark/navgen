#include "HueyNavMeshGenerator.h"

#include "DrawDebugHelpers.h"
#include "HueyNavMesh.h"
#include "NavigationSystem.h"

FHueyNavMeshGenerator::FHueyNavMeshGenerator(AHueyNavMesh& ownerNavMesh) : m_ownerNavMesh(ownerNavMesh)
{
}

void FHueyNavMeshGenerator::Init()
{
	_UpdateNavigationBounds();
}

bool FHueyNavMeshGenerator::RebuildAll()
{
	m_ownerNavMesh.ClearNavData();

	UWorld* world = m_ownerNavMesh.GetWorld();
	if (!world)
	{
		return false;
	}

	float minX = m_totalNavBound.Min.X;
	float maxX = m_totalNavBound.Max.X;
	float minY = m_totalNavBound.Min.Y;
	float maxY = m_totalNavBound.Max.Y;

	FHitResult hitResult;
	float top = 10000.0f;
	float bottom = 0.0f;
	FVector start(0.0f, 0.0f, top);
	FVector end(0.0f, 0.0f, bottom);
	for (float x = minX; x <= maxX; x += s_tileSize)
	{
		for (float y = minY; y <= maxY; y += s_tileSize)
		{
			start.X = x;
			start.Y = y;
			end.X = x;
			end.Y = y;

			FCollisionShape collisionShape =
				FCollisionShape::MakeBox(FVector(s_tileSize * 0.5f, s_tileSize * 0.5f, .0f));
			if (world->SweepSingleByChannel(
					hitResult, start, end, FQuat::Identity, ECollisionChannel::ECC_WorldStatic, collisionShape))
			{
				m_ownerNavMesh.AddNavData(int32(x), int32(y), hitResult.ImpactPoint.Z);
			}
		}
	}

	m_ownerNavMesh.OnNavMeshGenerationFinished();

	return true;
}

void FHueyNavMeshGenerator::OnNavigationBoundsChanged()
{
	_UpdateNavigationBounds();
}

void FHueyNavMeshGenerator::RebuildDirtyAreas(const TArray<FNavigationDirtyArea>& DirtyAreas)
{
	// TODO: 변경된 부분만 재계산 하게 개선

	RebuildAll();
}

float FHueyNavMeshGenerator::GetTileSize()
{
	return s_tileSize;
}

void FHueyNavMeshGenerator::_UpdateNavigationBounds()
{
	const UNavigationSystemV1* navSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(m_ownerNavMesh.GetWorld());
	if (!navSys)
	{
		m_totalNavBound = FBox(ForceInit);

		return;
	}

	if (navSys->ShouldGenerateNavigationEverywhere() == false)
	{
		FBox boundSum(ForceInit);

		TArray<FBox> supportedBounds;
		navSys->GetNavigationBoundsForNavData(m_ownerNavMesh, supportedBounds);
		m_navBounds.Reset(supportedBounds.Num());

		for (const FBox& bound : supportedBounds)
		{
			m_navBounds.Add(bound);
			boundSum += bound;
		}
		m_totalNavBound = boundSum;
	}
	else
	{
		m_navBounds.Reset(1);
		m_totalNavBound = navSys->GetWorldBounds();
		if (!m_totalNavBound.IsValid)
		{
			m_navBounds.Add(m_totalNavBound);
		}
	}
}

void FHueyNavMeshGenerator::TickAsyncBuild(float DeltaSeconds)
{
}

const float FHueyNavMeshGenerator::s_tileSize = 1000.0f;

const int32 FHueyNavMeshGenerator::s_tileSizeInt = (int32)(s_tileSize);

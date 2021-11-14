#include "HueyNavMeshGenerator.h"

#include "HueyNavMesh.h"
#include "NavigationSystem.h"

FHueyNavMeshGenerator::FHueyNavMeshGenerator(AHueyNavMesh& ownerNavMesh) : m_ownerNavMesh(ownerNavMesh)
{
}

bool FHueyNavMeshGenerator::RebuildAll()
{
	return true;
}

void FHueyNavMeshGenerator::OnNavigationBoundsChanged()
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

void FHueyNavMeshGenerator::RebuildDirtyAreas(const TArray<FNavigationDirtyArea>& DirtyAreas)
{
}

void FHueyNavMeshGenerator::TickAsyncBuild(float DeltaSeconds)
{
}

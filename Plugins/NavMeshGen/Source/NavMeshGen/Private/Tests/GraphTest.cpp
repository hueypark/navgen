#include "Graph.h"
#include "Node.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(
	GraphTest, "NavMeshGen.Graph", EAutomationTestFlags::EditorContext | EAutomationTestFlags::EngineFilter)

bool GraphTest::RunTest(const FString& Parameters)
{
	const int32_t nodeID = 1;

	Graph graph;

	std::shared_ptr<Node> node = std::make_shared<Node>(nodeID);

	graph.AddNode(node);

	std::shared_ptr<Node> getNode = graph.GetNode(nodeID);

	if (node != getNode)
	{
		AddError(FString::Printf(TEXT("Added node and get node is different.")));

		return false;
	}

	return true;
}

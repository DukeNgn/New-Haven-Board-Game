#include "TileNode.h"

// TODO: Consider making all of this header-only? 

TileNode::TileNode()
{
	resources[0] = NULL;
	resources[1] = NULL;
	resources[2] = NULL;
	resources[3] = NULL;
}

TileNode::~TileNode()
{
	delete[] resources;
}

void TileNode::linkResourceNode(Resource* resource, int index)
{
	resources[index] = resource; 
}

Resource** TileNode::getResourceNodes() { return resources; }



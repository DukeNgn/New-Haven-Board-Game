#pragma once
#include <string>
#include "Node.h"

enum class ResourceType {TIMBER, STONE, WHEAT, SHEEP, NONE};

// Resource class (extends Node)
class Resource : public Node
{
	ResourceType* type;
public:
	Resource();
	~Resource();
	void setType(ResourceType type) {
		*this->type = type;
	}
	ResourceType getType() { return *type; }
	std::string getTypeAsString();
};

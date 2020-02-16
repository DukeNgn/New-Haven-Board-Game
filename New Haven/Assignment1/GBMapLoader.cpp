#include "GBMapLoader.h"
#include "Resources.h"
#include <map>

ResourceType strToEnum(std::string str) {
	if (str.compare("SHEEP") == 0)
		return ResourceType::SHEEP;
	else if (str.compare("TIMBER") == 0)
		return ResourceType::TIMBER;
	else if (str.compare("STONE") == 0)
		return ResourceType::STONE;
	else if (str.compare("WHEAT") == 0)
		return ResourceType::WHEAT;
	else if (str.compare("NONE") == 0)
		return ResourceType::NONE;
}

void loadMap(std::string& fileName, GBMap& gb_map)
{
	std::ifstream inFile(fileName, std::ios::in);
	std::string lineRead;
	
	int length = 0;
	int height = 0;
	std::map<int, std::vector<ResourceType>> resourceData;
	std::vector<int> resourceIndices; 
	std::vector<int> disableData;

	while (inFile) {

		getline(inFile, lineRead);
		// String tokenizing code used for parsing the loaded map file 
		// Source: https://stackoverflow.com/a/53921
		std::stringstream strstr(lineRead);
		std::istream_iterator<std::string> it(strstr);
		std::istream_iterator<std::string> end; 
		std::vector<std::string> results(it, end);

		if (results[0].compare("LENGTH") == 0) 
			length = std::stoi(results[1]);
		else if (results[0].compare("HEIGHT") == 0) 
			height = std::stoi(results[1]);		
		else if (results[0].compare("RESOURCE") == 0){
			resourceData[std::stoi(results[1])] = { strToEnum(results[2]), strToEnum(results[3]), strToEnum(results[4]), strToEnum(results[5]) };
			resourceIndices.push_back(std::stoi(results[1]));
		}
		else if (results[0].compare("DISABLE") == 0) {
			disableData.push_back(std::stoi(results[1]));
		}
	}
	inFile.close();

	// Create the graphs using the length and height data from the file
	gb_map.tileGraph->makeGridGraph(length, height, NodeType::TILE);
	gb_map.resourceGraph->makeGridGraph(length * 2, height * 2, NodeType::RESOURCE);
	gb_map.tileGraph->linkResourceNodes(gb_map.resourceGraph); 

	// TO-DO: Use resource data from file to update the above two graphs accordingly. 
	for (int i = 0; i < disableData.size(); i++)
		gb_map.tileGraph->disableNode(disableData[i]);

	for (int i = 0; i < resourceIndices.size(); i++) {
		static_cast<TileNode*>(gb_map.tileGraph->getNode(resourceIndices[i]))->getResourceNodes()[0]->setType(resourceData[resourceIndices[i]][0]);
		static_cast<TileNode*>(gb_map.tileGraph->getNode(resourceIndices[i]))->getResourceNodes()[1]->setType(resourceData[resourceIndices[i]][1]);
		static_cast<TileNode*>(gb_map.tileGraph->getNode(resourceIndices[i]))->getResourceNodes()[2]->setType(resourceData[resourceIndices[i]][2]);
		static_cast<TileNode*>(gb_map.tileGraph->getNode(resourceIndices[i]))->getResourceNodes()[3]->setType(resourceData[resourceIndices[i]][3]);
	}
}


#pragma once
#include <map>
#include <vector>
#include "GBMap.h"
#include "Harvest.h"
#include "Building.h"

// Hand Object 
class Hand 
{
	public:
	Hand();
	~Hand();
	//Inline getters
	int getRemainHarvest() {return *this->numOfHarvest;}
	int getRemainBuilding() {return *this->numOfBuilding;}
	bool hasNoHarvest() {return (*this->numOfHarvest == 0);}
	bool hasNoBuilding() { return (*this->numOfBuilding == 0);}

	HarvestTile* getHarvestTile(int index) {return this->harvestHold[0][index];}

	void showHand();
	void addHarvestTile(HarvestTile* ht);
	void addBuildingTile(BuildingTile* bt);
	int getNodeID(GBMap* gb_map, int row, int col);
	bool requestRotate(HarvestTile* target);

	void exchange(GBMap* gb_map, TileNode*);
	int playHarvest(GBMap* gb_map);

private:
	int* numOfHarvest;
	int* numOfBuilding;
	std::vector<HarvestTile*>* harvestHold;
	std::vector<BuildingTile*>* buildingHold;
};



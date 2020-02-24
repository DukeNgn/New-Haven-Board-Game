#include "Hand.h"

Hand::Hand()
{
	numOfHarvest = new int(0);
	numOfBuilding = new int(0);
	harvestHold = new std::vector<HarvestTile*>(0);
	buildingHold = new std::vector<BuildingTile*>(0);

}

Hand::~Hand()
{
}

void Hand::showHand()
{
	std::cout << "Showing cards on hand:" << std::endl;
	for (int i = 0; i < this->getRemainHarvest(); ++i) {
		std::cout << "Harvest Tile no." << i << std::endl;
		std::cout << *harvestHold[0][i] << std::endl;
	}
	for (int i = 0; i < this->getRemainBuilding(); ++i) {
		std::cout << "Building Tile no." << i << std::endl;
		std::cout << *buildingHold[0][i] << std::endl;
	}

}

void Hand::addHarvestTile(HarvestTile* ht)
{
	harvestHold->push_back(ht);
	*numOfHarvest = *numOfHarvest + 1;
}

void Hand::addBuildingTile(BuildingTile* bt)
{
	buildingHold->push_back(bt);
	*numOfBuilding = *numOfBuilding + 1;
}

bool Hand::requestRotate(HarvestTile* target)
{
	std::cout << "Start rotating session" << std::endl;

	char input;
	char back = 'b';
	char end = 'e';
	char rotate = 'r';

	while (true) {
		std::cout << "\n---------------" << std::endl;
		std::cout << "Your Harvest Tile:" << std::endl;
		std::cout << *target << std::endl;
		std::cout << "---------------" << std::endl;
		
		std::cout << "+ Rotate the Tile: r" << std::endl;
		std::cout << "+ End rotating the Tile: e" << std::endl;
		std::cout << "+ Choose another Tile: b" << std::endl;
		std::vector<ResourceType>* container = target->getContainer();

		try {
			std::cin >> input;

			if (input == back) {
				return false;
			}

			if (input == end) {
				std::cout << "------------------------------------------" << std::endl;
				std::cout << "Placing Tile on the gameboard" << std::endl;
				std::cout << "------------------------------------------" << std::endl;
				return true;

			}

			if (input == rotate) {
				// Mimic a deque for rotating clockwise 
				std::cout << "Rotating ..." << std::endl;
				ResourceType temp = container[0][3];
				container[0].insert(container[0].begin(), temp);
				container[0].erase(container[0].end() - 1);
				continue;
			}

			throw std::exception();
		}
		catch (const std::exception & e) {
			std::cout << "Invalid input. Please try again" << std::endl;
			continue;
		}

	}

}

void Hand::exchange(GBMap* gb_map, HarvestTile* target)
{

}

int Hand::playHarvest(GBMap* gb_map) {
	std::cout << "----PLAYING HARVEST TILE----" << std::endl;
	while(true) {
		if(hasNoHarvest()) {
			std::cout << "There is no Harvest Tile on hand to play" << std::endl;
			return 1;
		}

		showHand();
				
		int choice;
		std::cout << "Currently, you are having " << getRemainHarvest() << " Harvest Tiles on your hand" << std::endl;
		std::cout << "Please enter the index of Harvest Tile you want to play" << std::endl;
		std::cin >> choice;
		if (choice < 0 || choice >= this->getRemainHarvest()) {
			std::cout << "Invalid choice. Please try again." << std::endl;
			continue;
		}

		HarvestTile* target = getHarvestTile(choice);
		std::cout << "Enter the ID of the square you want to place your Tile on:" << std::endl;
		std::cin >> choice;

		TileNode* location = static_cast<TileNode*>(gb_map->getTileGraph()->getNode(choice));
		if(gb_map->isValid(location)) {
			if (requestRotate(target)) {
				// User satisfies with their choice of rotation, process to place HarvestTile
				gb_map->placeHarvestTile(target, location);
				std::cout << "PLACED TILE ON THE GAMEBOARD SUCCESSFULLY" << std::endl;
				exchange(gb_map, target);
				break;
			} 

		} else {
			std::cout << "------------------------------------------" << std::endl;
			std::cout << "The position is either occupied or invalid" << std::endl;
			std::cout << "Please try again." << std::endl;
			std::cout << "------------------------------------------" << std::endl;
			continue;
		}
	}

	return 0;
}



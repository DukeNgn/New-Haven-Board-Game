#include "Hand.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ostream;
using std::exception;

Hand::Hand()
{
	numOfHarvest = new int(0);
	numOfBuilding = new int(0);
	harvestHold = new vector<HarvestTile*>(0);
	buildingHold = new vector<BuildingTile*>(0);
}

Hand::~Hand()
{
	// TODO:!! 
	//for (auto n : *harvestHold)
	//	delete n;
	//harvestHold->clear();

	//for (auto n : *buildingHold)
	//	delete n;
	//buildingHold->clear();

	delete numOfHarvest;
	delete numOfBuilding;
}

void Hand::showHand()
{
	cout << "Showing cards on hand:" << endl;
	for (int i = 0; i < this->getRemainHarvest(); ++i) {
		cout << "Harvest Tile no." << i << endl;
		cout << *harvestHold[0][i] << endl;
	}
	for (int i = 0; i < this->getRemainBuilding(); ++i) {
		cout << "Building Tile no." << i << endl;
		cout << *buildingHold[0][i] << endl;
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

/*
2 functions getNodeID and getNodeID_VG
were used to get the corresponding node ID on the map
of the provided location and the map itself
*/

int Hand::getNodeID(GBMap* gb_map, int row, int col)
{
	int length = gb_map->getTileGraph()->getLength();
	return (row * length + col);
}

int Hand::getNodeID_VG(VGMap* vg_map, int row, int col)
{
	int length = vg_map->getBuildingGraph()->getLength();
	return (row * length + col);
}

/*
A function asking and validating user's prefered orientation of the Harvest Tile they are playing
*/
bool Hand::requestRotate(HarvestTile* target)
{
	cout << "\n---Start rotating session---" << endl;

	char input;
	char back = 'b';
	char end = 'e';
	char rotate = 'r';

	while (true) {
		cout << "\n---------------" << endl;
		cout << "Your Harvest Tile:" << endl;
		cout << *target << endl;
		cout << "---------------" << endl;
		
		cout << "+ Rotate the Tile: r" << endl;
		cout << "+ End rotating the Tile: e" << endl;
		cout << "+ Choose another Tile: b" << endl;
		vector<ResourceType>* container = target->getContainer();

		try {
			cin >> input;

			if (input == back) {
				return false;
			}

			if (input == end) {
				cout << "------------------------------------------" << endl;
				cout << "Placing Tile on the gameboard" << endl;
				cout << "------------------------------------------" << endl;
				return true;

			}

			if (input == rotate) {
				// Mimic a deque for rotating clockwise 
				cout << "Rotating ..." << endl;
				ResourceType temp = container[0][3];
				container[0].insert(container[0].begin(), temp);
				container[0].erase(container[0].end() - 1);
				continue;
			}

			throw exception();
		}
		catch (const exception & e) {
			cout << "Invalid input. Please try again" << endl;
			continue;
		}

	}

}

/*
Function exchange will calculate the generated resources from a player's move
Then, set the Resource Tracker (located on the GBMap).
Finally, display the generated resources.
*/
void Hand::exchange(GBMap* gb_map, TileNode* location)
{
    std::map<ResourceType , int> generatedResources = { {ResourceType::SHEEP,0},{ResourceType::STONE,0},{ResourceType::TIMBER,0},{ResourceType::WHEAT,0} };
	gb_map->calcResourceAdjacencies(location, generatedResources);
	gb_map->setResourceTracker(&generatedResources);
	gb_map->displayResourceTracker();
}

/*
Function to start playing Harvest Tile and placing the tile on the board if it is valid
*/
int Hand::playHarvest(GBMap* gb_map) {
	cout << "\n----PLAYING HARVEST TILE----" << endl;
	while (true) {
		if (hasNoHarvest()) {
			cout << "There is no Harvest Tile on hand to play" << endl;
			return 1;
		}

		showHand();

		//Ask for choice of Harvest Tile on hand
		int choice = -1;
		cout << "\nCurrently, you are having " << getRemainHarvest() << " Harvest Tiles on your hand" << endl;
		cout << "Please enter the index of Harvest Tile you want to play" << endl;
		try {
            cin >> choice;
            if (choice < 0 || choice >= this->getRemainHarvest()) {
                throw exception();
            }

		} catch (const exception& e) {
		    cout << "Invalid choice. Please try again." << endl;
		    continue;
		}

		HarvestTile* target = getHarvestTile(choice);

		//Ask for position on the map to place tile
		cout << "\n---Select position to place Tile---" << endl;
		int row, col;
		try {
			cout << "Enter the index of row:" << endl;
			cin >> row;
			if (row < 0 || row >= gb_map->getTileGraph()->getHeight()) {
				throw exception();
			}

			cout << "Enter the index of column:" << endl;
			cin >> col;
			if (col < 0 || col >= gb_map->getTileGraph()->getLength()) {
				throw exception();
			}
		}
		catch (const exception & e) {
			cout << "Invalid position input. Please try again" << endl;
			continue;
		}

		/* Check if the position is enabled and not occupied
		=> process to rotate the Tile as requested
		=> Place Tile on the map and calculate resources by exchange
		*/
		int nodeID = this->getNodeID(gb_map, row, col);
		TileNode* location = static_cast<TileNode*>(gb_map->getTileGraph()->getNode(nodeID));
		if (gb_map->isValid(location)) {
			if (requestRotate(target)) {
				// User satisfies with their choice of rotation, process to place HarvestTile
				gb_map->placeHarvestTile(target, location);
				cout << "\nPLACED TILE ON THE GAMEBOARD SUCCESSFULLY\n" << endl;
				exchange(gb_map, location);
				harvestHold->erase(harvestHold[0].begin() + choice); //Remove the tile from hand after placement
				*numOfHarvest = *numOfHarvest - 1;
				break;
			}

		}
		else {
			cout << "\n------------------------------------------" << endl;
			cout << "The position is either occupied or invalid" << endl;
			cout << "Please try again." << endl;
			cout << "------------------------------------------\n" << endl;
			continue;
		}
	}

	return 0;
}

/*
A function to ask and validate player's request on flipping the Building Tile or not
*/
bool Hand::requestFlip(BuildingTile* target) {
    cout << "\n---Start flipping---" << endl;

    char input;
    char back = 'b';
    char end = 'e';
    char flip = 'f';
    char question = '?';

    while (true) {
        cout << "\n---------------" << endl;
        cout << "Your Building Tile:" << endl;
        cout << *target << endl;
        cout << "---------------" << endl;

        cout << "+ Flip the Tile: f" << endl;
        cout << "+ End flipping the Tile: e" << endl;
        cout << "+ Choose another Tile: b" << endl;
        cout << "+ Trade-of of flipping request: ?" << endl;

        try {
            cin >> input;

            if (input == back) {
                return false;
            }

            if (input == end) {
                cout << "------------------------------------------" << endl;
                cout << "Validating the request" << endl;
                cout << "------------------------------------------" << endl;
                return true;

            }

            if(input == question) {
                cout << "----OVERVIEW----" << endl;
                cout << "\nBuildings may only be played face up if the number on the\n"
                             "\t space matches the number showing on the Building." << endl;
                cout << "\nBuildings may be played face down onto any number space,\n"
                             "\t regardless of the number on the Building." << endl;
                cout << "\nThe value of each row and column is the number\n"
                             "of Colonists shown at the right or bottom. BUT! The value of a row or column is doubled if every\n"
                             "Building played on that row or column is face up!" << endl;
                continue;
            }

            if(input == flip) {
                cout << "\nFlipping the tile" << endl;
                target->setFaceUp(!target->getFaceUp());
                if(target->getFaceUp()) {
                    cout << "The Building Tile is faced up." << endl;
                    cout << "You CAN earn double points if you complete the whole column or row corresponding to the position of this card" << endl;
                } else {
                    cout << "The Building Tile is faced down." << endl;
                    cout << "You CANNOT earn double points if you complete the whole column or row corresponding to the position of this card" << endl;
                }
                continue;
            }

            throw exception();
        }
        catch (const exception & e) {
            cout << "Invalid input. Please try again" << endl;
            continue;
        }

    }

}

/*
Function to start playing Building Tile and placing the tile on the village board if it is valid
(build village)
*/
void Hand::playBuilding(VGMap* vg_map) {
	cout << "\n----PLAYING BUILDING TILE----" << endl;
	while (true) {
		if (hasNoBuilding()) 
			cout << "There is no Building Tile on hand to play" << endl;
		
		showHand();

		//Ask for choice of Building Tile from hand
		int choice = -1;
		cout << "\nCurrently, you have " << getRemainBuilding() << " Building tiles in your hand" << endl;
		cout << "Please enter the index of Building Tile you want to play" << endl;
		try {
			cin >> choice;
			if (choice < 0 || choice >= this->getRemainBuilding()) {
				throw exception();
			}

		}
		catch (const exception & e) {
			cout << "Invalid choice. Please try again." << endl;
			continue;
		}

		BuildingTile* target = getBuildingTile(choice);

		//Ask for position on the map to place tile
		cout << "\n---Select position to place Tile---" << endl;
		int row, col;
		try {
			cout << "Enter the index of row:" << endl;
			cin >> row;
			if (row < 0 || row >= vg_map->getBuildingGraph()->getHeight()) {
				throw exception();
			}

			cout << "Enter the index of column:" << endl;
			cin >> col;
			if (col < 0 || col >= vg_map->getBuildingGraph()->getLength()) {
				throw exception();
			}
		}
		catch (const exception & e) {
			cout << "Invalid position input. Please try again" << endl;
			continue;
		}

		/* Check if the position is enabled and not occupied
		=> process to flip the tile as requested
		=> Place Tile on the map.
		*/
		int nodeID = this->getNodeID_VG(vg_map, row, col);
		BuildingTile* location = static_cast<BuildingTile*>(vg_map->getBuildingGraph()->getNode(nodeID));
		if (requestFlip(target)) {
		    if(vg_map->isValid(target, location)) {
                vg_map->placeBuildingTile(target, location);
                cout << "\nPLACED TILE ON THE VGBOARD SUCCESSFULLY\n" << endl;
                buildingHold->erase(buildingHold[0].begin() + choice); //Remove the tile from hand after placement
                *numOfBuilding = *numOfBuilding - 1;
                break;
		    } else {
                cout << "\n------------------------------------------" << endl;
                cout << "The position is either occupied or invalid flipping request" << endl;
                cout << "Please try again." << endl;
                cout << "------------------------------------------\n" << endl;
                continue;
            }
		}

	}
}


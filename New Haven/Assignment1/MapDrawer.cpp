#include <string> 
#include "MapDrawer.h"
using std::cout;
using std::endl; 
using std::vector;
using std::to_string;
using std::make_unique;

MapDrawer::MapDrawer()
{
    MAP_TILE = make_unique<CImg<unsigned char>>("./Images/TileNode.BMP");
    DISABLED_TILE = make_unique<CImg<unsigned char>>("./Images/DisabledTile.BMP");

    RESOURCE_TRACKER = make_unique<CImg<unsigned char>>("./Images/ResourceTracker.BMP");
    AVAILABLE_BUILDINGS = make_unique<CImg<unsigned char>>("./Images/AvailableBuildings.BMP");

    NONE = make_unique<CImg<unsigned char>>("./Images/None.BMP");

    SHEEP = make_unique<CImg<unsigned char>>("./Images/Sheep.BMP");
    STONE = make_unique<CImg<unsigned char>>("./Images/Stone.BMP");
    WHEAT = make_unique<CImg<unsigned char>>("./Images/Wheat.BMP");
    TIMBER = make_unique<CImg<unsigned char>>("./Images/Timber.BMP");

    GREEN_TRACKER = make_unique<CImg<unsigned char>>("./Images/GreenTracker.BMP");
    GREY_TRACKER = make_unique<CImg<unsigned char>>("./Images/GreyTracker.BMP");
    YELLOW_TRACKER = make_unique<CImg<unsigned char>>("./Images/YellowTracker.BMP");
    RED_TRACKER = make_unique<CImg<unsigned char>>("./Images/RedTracker.BMP");

    GREEN_BUILDING = make_unique<CImg<unsigned char>>("./Images/GreenBuilding.BMP");
    GREY_BUILDING = make_unique<CImg<unsigned char>>("./Images/GreyBuilding.BMP");
    YELLOW_BUILDING = make_unique<CImg<unsigned char>>("./Images/YellowBuilding.BMP");
    RED_BUILDING = make_unique<CImg<unsigned char>>("./Images/RedBuilding.BMP");

    GREEN_BUILDING_DOWN = make_unique<CImg<unsigned char>>("./Images/GreenBuildingDown.BMP");
    GREY_BUILDING_DOWN = make_unique<CImg<unsigned char>>("./Images/GreyBuildingDown.BMP");
    YELLOW_BUILDING_DOWN = make_unique<CImg<unsigned char>>("./Images/YellowBuildingDown.BMP");
    RED_BUILDING_DOWN = make_unique<CImg<unsigned char>>("./Images/RedBuildingDown.BMP");
}

// Returns the appropriate resource image based on the passed resource type
CImg<unsigned char> MapDrawer::ResourceToBMP(ResourceType type) {
    switch (type) {
    case ResourceType::NONE:
        return *NONE;
    case ResourceType::SHEEP:
        return *SHEEP;
    case ResourceType::STONE:
        return *STONE;
    case ResourceType::WHEAT:
        return *WHEAT;
    case ResourceType::TIMBER:
        return *TIMBER;
    }
}

// Returns the appropriate building image based on the passed resource type
CImg<unsigned char> MapDrawer::BuildingToBMP(ResourceType type) {
    switch (type) {
    case ResourceType::NONE:
        return *NONE;
    case ResourceType::SHEEP:
        return *GREEN_BUILDING;
    case ResourceType::STONE:
        return *GREY_BUILDING;
    case ResourceType::WHEAT:
        return *YELLOW_BUILDING;
    case ResourceType::TIMBER:
        return *RED_BUILDING;
    }
}

// Returns the appropriate face-down building image based on the passed resource type
CImg<unsigned char> MapDrawer::BuildingDownToBMP(ResourceType type) {
    switch (type) {
    case ResourceType::NONE:
        return *NONE;
    case ResourceType::SHEEP:
        return *GREEN_BUILDING_DOWN;
    case ResourceType::STONE:
        return *GREY_BUILDING_DOWN;
    case ResourceType::WHEAT:
        return *YELLOW_BUILDING_DOWN;
    case ResourceType::TIMBER:
        return *RED_BUILDING_DOWN;
    }
}

CImg<unsigned char> MapDrawer::drawColumnIndicators(GBMap& const gb_map) {
    // Make the numbered column indicators based on the length of the tile graph
    CImg<unsigned char> columns(25, 25, 1, 3, 0);    // Empty square corner between column and row indicators

    // Create the column indicator based on length of tile graph
    for (int i = 0; i < gb_map.getTileGraph()->getLength(); i++) {
        CImg<unsigned char> columnTitle("./Images/Column.BMP");
        int columnTitle_x = 50;
        int columnTitle_y = 1;
        columnTitle.draw_text(columnTitle_x, columnTitle_y, to_string(i).c_str(), black, vagueBrown, 1.0f, 22);
        columns.append(columnTitle);
    }
    
    return columns;
}

CImg<unsigned char> MapDrawer::drawGBMap(GBMap& const gb_map)
{
    //// GBMAP
    // GRID is initialized with the numbered columns to start and will gradually be appended with more components
    CImg<unsigned char> GRID = MapDrawer::drawColumnIndicators(gb_map);

    // Row-by-row traversal of harvest tile graph
    for (int i = 0; i < gb_map.getTileGraph()->getHeight(); i++) {

        // Draws the column indicator to the far-left of the row.
        CImg<unsigned char> rowTitle("./Images/Row.BMP");
        int row_x = 8;
        int row_y = 50;
        rowTitle.draw_text(row_x, row_y, to_string(i).c_str(), black, vagueBrown, 1.0f, 22);

        // Traverse the row
        for (int j = 0; j < gb_map.getTileGraph()->getLength(); j++) {

            // Calculate next node id from row and column number and get a pointer to the node.
            int id = i * gb_map.getTileGraph()->getLength() + j;
            Node* n = gb_map.getTileGraph()->getNode(id);

            // If node is enabled, then for each of the 4 resources, draw the resource onto the tile   
            // Else, draw a disabled tile.
            // Append the results to the row as is appropriate. 
            if (n->isEnabled()) {
                // Give the tile a "blank" map tile which will be dressed with resources
                CImg<unsigned char> tile = *MAP_TILE;

                // For each of the 4 resources that are on a harvest tile
                for (int k = 0; k < 4; k++) {
                    // Get the resource as an image
                    CImg<unsigned char> resource = ResourceToBMP(static_cast<Resource*>((static_cast<TileNode*>(n)->getResourceNodes()[k]))->getType());

                    // Determine which position the resource should be drawn in the tile based on the value of k
                    switch (k) {
                    case 0:
                        tile.draw_image(0, 0, 0, resource, 100);
                        break;
                    case 1:
                        tile.draw_image(tile.width() / 2, 0, 0, resource, 100);
                        break;
                    case 2:
                        tile.draw_image(0, tile.height() / 2, 0, resource, 100);
                        break;
                    case 3:
                        tile.draw_image(tile.width() / 2, tile.height() / 2, 0, resource, 100);
                        break;
                    }
                }
                rowTitle.append(tile, 'x');
            }
            else
                rowTitle.append(*DISABLED_TILE, 'x');
        }
        // Once the complete row has been drawn, append it in the vertical direction to GRID
        GRID = GRID.append(rowTitle, 'y');
    }

    return GRID;
}

CImg<unsigned char> MapDrawer::drawVGMap(Player& const player)
{
    CImg<unsigned char> VGMAP("./Images/VGMap.BMP");
    //// VGMAP
    // Add the name of the village to the bottom of the VGMap
    int village_name_x = 235;
    int village_name_y = 655;
    VGMAP.draw_text(village_name_x, village_name_y, player.getVGMap()->getName().c_str(), black, 1, 1.0f, 30);

    vector<Node*> vgNodes = player.getVGMap()->getBuildingGraph()->getNodes()[0];
    int vg_length = player.getVGMap()->getBuildingGraph()->getLength();
    int vg_height = player.getVGMap()->getBuildingGraph()->getHeight();

    // Row-by-row traversal of VGMap
    for (int i = 0; i < vg_length; i++) {

        // Traverse the row
        for (int j = 0; j < vg_height; j++) {

            // Calculate node id from row and column numbers
            int id = j * vg_length + i;
            CImg<unsigned char> buildingTile;

            // Draw building tile appropriately based on if face up or down.
            if (static_cast<BuildingTile*>(vgNodes[id])->isFaceUp()) {
                buildingTile = BuildingToBMP(static_cast<BuildingTile*>(vgNodes[id])->getType());
                int value_x = 3;
                int value_y = 20;
                buildingTile.draw_text(value_x, value_y, to_string(static_cast<BuildingTile*>(vgNodes[id])->getValue()).c_str(), black, 1, 1.0f, 40);
            }
            else
                buildingTile = BuildingDownToBMP(static_cast<BuildingTile*>(vgNodes[id])->getType());

            // Draw the building tile in the appropriate location on VGMap
            int x_offset = 18;
            int y_offset = 23;
            int building_x = x_offset + buildingTile.width() * i;
            int building_y = y_offset + buildingTile.width() * j;
            VGMAP.draw_image(building_x, building_y, 0, buildingTile, 100);
        }
    }
    return VGMAP;
}

void MapDrawer::drawHarvestOnHand(Player& const player, std::shared_ptr<CImg<unsigned char>> HAND)
{
    vector<HarvestTile*> tiles = player.getHand()->getHarvestHold()[0];

    // HAND: HARVEST TILES
    // Iterate through each harvest tile in the player's hand
    for (int j = 0; j < tiles.size(); j++) {
        // Initialize to a "blank" tile
        CImg<unsigned char> tile = *MAP_TILE;
        // For each of the 4 resource slots, draw the resource image onto the tile
        for (int i = 0; i < 4; i++) {

            CImg<unsigned char> resource = ResourceToBMP(tiles[j]->getContainer()[i]);
            switch (i) {
            case 0:
                tile.draw_image(0, 0, 0, resource, 100);
                break;
            case 1:
                tile.draw_image(tile.width() / 2, 0, 0, resource, 100);
                break;
            case 2:
                tile.draw_image(tile.width() / 2, tile.height() / 2, 0, resource, 100);
                break;
            case 3:
                tile.draw_image(0, tile.height() / 2, 0, resource, 100);
                break;
            }
        }
        // Once the tile is "dressed", we draw it in the appropriate place in the player's hand
        int x_offset = tile.width() * 1.25;
        int y_offset = 25;
        HAND->draw_image(x_offset * j, y_offset, 0, tile, 100);
        HAND->draw_text(x_offset * j, y_offset, to_string(j).c_str(), black, vagueBrown, 1.0f, 22);
    }
}

void MapDrawer::drawBuildingsOnHand(Player& const player, std::shared_ptr<CImg<unsigned char>> HAND)
{
    vector<BuildingTile*> buildings = player.getHand()->getBuildingHold()[0];

    // HAND: BUILDINGS
    // Iterate through each building in player's hand
    for (int i = 0; i < buildings.size(); i++) {

        // Assign "building" the correct image based on building type
        CImg<unsigned char> building = BuildingToBMP(buildings[i]->getType());

        // Draw the building value onto the building image
        int value_x = 4;
        int value_y = 20;
        building.draw_text(value_x, value_y, to_string(buildings[i]->getValue()).c_str(), black, 1, 1.0f, 28);

        // The logic below allows for the display of buildings in the hand to adjust depending on how many buildings are in hand
        // Ideally, it can comfortably support 0 to 60 buildings in hand. Over that and it will be difficult to read.
        // The number of buildings in hand will be distributed "as best as possible" over 3 rows of space in the hand section displayed.

        const int number_of_rows = 3;

        int row_size = (buildings.size() + number_of_rows - 1) / number_of_rows;
        int handRow = i / row_size;   // integer division intended

        int x_offset = HAND->width() / row_size;
        int y_offset = 135;
        HAND->draw_image(x_offset * (i % row_size), y_offset + building.width() * handRow, 0, building, 100);
        HAND->draw_text(x_offset * (i % row_size), y_offset + building.width() * handRow, to_string(i).c_str(), black, vagueBrown, 1.0f, 22);
    }
}

CImg<unsigned char> MapDrawer::drawHand(Player& const player)
{
    //// HAND
    std::shared_ptr<CImg<unsigned char>> HAND;
    HAND = std::make_shared<CImg<unsigned char>>("./Images/Hand.BMP");
    drawHarvestOnHand(player, HAND);
    drawBuildingsOnHand(player, HAND);
    return *HAND;
}

void MapDrawer::drawResourceTracker(GBMap& const gb_map)
{
    //// RESOURCE TRACKER
    int stone = gb_map.getResourceTracker()[0][ResourceType::STONE];
    int timber = gb_map.getResourceTracker()[0][ResourceType::TIMBER];
    int wheat = gb_map.getResourceTracker()[0][ResourceType::WHEAT];
    int sheep = gb_map.getResourceTracker()[0][ResourceType::SHEEP];

    // Horizontal positions for each of the 4 trackers on the resource tracker images
    int stone_x = 16;
    int timber_x = 35;
    int wheat_x = 70;
    int sheep_x = 91;

    int resources_y = 670;  // Vertical starting position of resources on the tracker
    int delta_y = 33;    // This is the amount of pixels needed to move up one step in the resource tracker

    RESOURCE_TRACKER->draw_image(stone_x, resources_y - delta_y * stone, *GREY_TRACKER, 100);
    RESOURCE_TRACKER->draw_image(timber_x, resources_y - delta_y * timber, *RED_TRACKER, 100);
    RESOURCE_TRACKER->draw_image(wheat_x, resources_y - delta_y * wheat, *YELLOW_TRACKER, 100);
    RESOURCE_TRACKER->draw_image(sheep_x, resources_y - delta_y * sheep, *GREEN_TRACKER, 100);

}

CImg<unsigned char> MapDrawer::drawGame(GBMap& const gb_map, Player& const player)
{
    CImg<unsigned char> gameBoard = drawGBMap(gb_map);
    
    CImg<unsigned char> VGMAP = drawVGMap(player);

    CImg<unsigned char> HAND = MapDrawer::drawHand(player);

    gameBoard.append(HAND, 'y');
    drawResourceTracker(gb_map);
    
//// FINAL ASSEMBLY OF ALL BOARD AND PLAYER COMPONENTS
    CImg<unsigned char> SCREEN = *RESOURCE_TRACKER;
    SCREEN.append(gameBoard);
    SCREEN.append(*AVAILABLE_BUILDINGS);
    SCREEN.append(VGMAP);
    return SCREEN;
}


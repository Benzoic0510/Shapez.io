#include "map.h"

Map::Map(int r, int c) : rows(r) , cols(c) {
	grid.resize(rows, vector<Tile>(cols));
	for (int i=0; i<r; i++) {
		for (int j=0; j<c; j++) {
			//TODO
			//grid[i][j] = Tile(i,j);
		}
	}
}

void Map::placeDeliveryHub(int x, int y, int w, int h) {
	for (int i=x;i<x+w;i++)
		for (int j=y;j<y+h;j++)
			//TODO
			//grid[i][j].type = TileType::DELIVERY;
	cout << "Delivery hub placed at (" << x << "," << y << ")\n";
}

void Map::addResource(int x, int y, ResourceType type) {
	//grid[x][y].type = TileType::RESOURCE;
	//grid[x][y].resource = make_shared<Resource>(type);
	//cout << "Added resource:[" << grid[x][y].resource->name()
	//	   << "] at (" << x << "," << y << ")\n";
}

void Map::placeMachine(Machine* m) {
	machines.emplace_back(m);
	//cout << "Placed machine: " << m->name() << "\n";
}

void Map::tick() {
	//for (auto& m : machines)
		//m->update();
	//hub.report();
}
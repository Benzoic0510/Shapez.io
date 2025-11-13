#include "map.h"

Map::Map(int r, int c) : rows(r) , cols(c) {
	grid.resize(rows, vector<Tile>(cols));
	for (int i=0; i<r; i++) {
		for (int j=0; j<c; j++) {
			grid[i][j] = Tile(i,j);
		}
	}
}

void Map::placeDeliveryHub(int x, int y, int w, int h) {
	for (int i=x;i<x+w;i++)
		for (int j=y;j<y+h;j++)
			grid[i][j].type = TileType::DELIVERY;
	cout << "Delivery hub placed at (" << x << "," << y << ")\n";
}

void Map::addResource(int x, int y, ResourceType type) {
	grid[x][y].type = TileType::RESOURCE;
	grid[x][y].resource = make_shared<Resource>(type);
	cout << "Added resource:[" << grid[x][y].resource->name()
		 << "] at (" << x << "," << y << ")\n";
}

void Map::placeMachine(Machine* m) {
	machines.emplace_back(m);
	grid[m->x][m->y].machine = m;
	cout << "Placed machine: " << m->name() << "\n";
}

void Map::tick() {
	for (auto& m : machines)
		m->update(*this);

	//[Delivery] part
	for (int i=0;i<rows;i++) {
		for (int j=0;j<cols;j++) {
			if (grid[i][j].type == TileType::DELIVERY && grid[i][j].item) {
				hub.receive(grid[i][j].item->res->value);
				cout << "[Delivery] got " << grid[i][j].item->res->name() << "\n";
				grid[i][j].item = nullptr;
			}
		}
	}
	hub.report();
}

int Map::getCoins() {
	return hub.getCoins();
}

bool Map::inBounds(int x, int y) {
	return x>=0 && x<rows && y>=0 && y<cols;
}


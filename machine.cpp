#include "machine.h"

#include "map.h"
#include "tile.h"

Machine::Machine(int x, int y) : x(x), y(y) {}

//Miner
Miner::Miner(int x, int y, Direction d) : Machine(x,y), dir(d) {}

void Miner::update(Map& map) {
	Tile& t = map.grid[x][y];
	if (!t.resource) return;

	//item create
	static int global_id = 0;
	auto item = make_shared<Item>(global_id++, t.resource);
	cout << "Miner at (" << x << "," << y << ") mining resource\n";

	int nx = x, ny = y;
	switch (dir) {
		case Direction::UP: nx--; break;
		case Direction::DOWN: nx++; break;
		case Direction::LEFT: ny--; break;
		case Direction::RIGHT: ny++; break;
	}

	Tile& out = map.grid[nx][ny];

	if (out.item == nullptr) {
		out.item = item;
		cout << "[Miner] produced " << item->res->name()
			 << " to (" << nx << "," << ny << ")\n";
	}
}

string Miner::name() const { return "Miner"; }


//Conveyor
Conveyor::Conveyor(int x, int y, Direction d) : Machine(x,y), dir(d) {}
void Conveyor::update(Map& map) {
	Tile& t = map.grid[x][y];
	if (!t.item) return;

	int nx=x, ny=y;
	switch (dir) {
		case Direction::UP: nx--; break;
		case Direction::DOWN: nx++; break;
		case Direction::LEFT: ny--; break;
		case Direction::RIGHT: ny++; break;
	}

	Tile& out = map.grid[nx][ny];

	if (out.item == nullptr) {
		out.item = t.item;
		t.item = nullptr;
		cout << "[Conveyor] moved item to (" << nx << "," << ny << ")\n";
	}
}
string Conveyor::name() const { return "Conveyor"; }


//Cutter
Cutter::Cutter(int x, int y) : Machine(x,y) {}
void Cutter::update(Map& map) {
	cout << "Cutter at (" << x << "," << y << ") cutting item\n";
}
string Cutter::name() const { return "Cutter"; }


//TrashCan
TrashCan::TrashCan(int x, int y) : Machine(x,y) {}
void TrashCan::update(Map& map) {
	cout << "TrashCan at (" << x << "," << y << ") destroying item\n";
}
string TrashCan::name() const { return "TrashCan"; }
#include "machine.h"

#include "map.h"
#include "tile.h"

Machine::Machine(int x, int y) : x(x), y(y) {}

//Miner
Miner::Miner(int x, int y, Direction d, int cooldown) : Machine(x,y), dir(d), produceCooldown(cooldown) {}
void Miner::update(Map& map) {
	Tile& t = map.grid[x][y];

	if (buffer) {
		int nx = x, ny = y;
		switch (dir) {
			case Direction::UP: nx--; break;
			case Direction::DOWN: nx++; break;
			case Direction::LEFT: ny--; break;
			case Direction::RIGHT: ny++; break;
		}

		//Out-of-bounds check
		if (!map.inBounds(nx, ny)) return;

		Tile& out = map.grid[nx][ny];

		if (out.machine && out.machine->canAccept(buffer)) {
			out.machine->accept(buffer);
			buffer = nullptr;
			cout << "[Miner] pushed item to conveyor at (" << nx << "," << ny << ")\n";
		}

		//block error
		if (buffer) return;
	}

	if (!t.resource) return;

	produceTimer++;
	if (produceTimer < produceCooldown) return;
	produceTimer = 0;

	//item create
	static int global_id = 0;
	auto item = make_shared<Item>(global_id++, t.resource);
	buffer = item;
	cout << "Miner at (" << x << "," << y << ") mining resource\n";
}
string Miner::name() const { return "Miner"; }


//Conveyor
Conveyor::Conveyor(int x, int y, Direction d, int cooldown) : Machine(x,y), dir(d), moveCooldown(cooldown) {}
void Conveyor::update(Map& map) {
	moveTimer++;
	if (moveTimer < moveCooldown) return;
	moveTimer = 0;

	Tile* from = selfTile;
	if (!from->item) return;

	int nx=x, ny=y;
	switch (dir) {
		case Direction::UP: nx--; break;
		case Direction::DOWN: nx++; break;
		case Direction::LEFT: ny--; break;
		case Direction::RIGHT: ny++; break;
	}

	Tile& out = map.grid[nx][ny];

	if (out.item == nullptr) {
		out.item = from->item;
		from->item = nullptr;
		cout << "[Conveyor] moved item to (" << nx << "," << ny << ")\n";
	}
}
string Conveyor::name() const { return "Conveyor"; }
bool Conveyor::canAccept(shared_ptr<Item> item) {
	return selfTile && selfTile->item == nullptr && item != nullptr;
}
void Conveyor::accept(shared_ptr<Item> item) {
	selfTile->item = item;
}


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
#include "machine.h"

#include "map.h"
#include "tile.h"

Machine::Machine(int x, int y) : x(x), y(y) {}


//Miner
Miner::Miner(int x, int y, Direction d, int cooldown) : Machine(x,y), dir(d), produceCooldown(cooldown) {}
void Miner::update(Map& map) {
	Tile& t = map.grid[x][y];

	if (buffer) {
		int nx, ny;
		Tile* front = nullptr;

		if (!map.getFrontTile(x, y, dir, nx, ny, front))
			return;

		if (front->machine && front->machine->canAccept(buffer, dir)) {
			front->machine->accept(buffer);
			cout << "[Miner] pushed resource to (" << nx << "," << ny << ")\n";
			buffer = nullptr;
		}

		if (buffer) return;
	}

	if (!t.resource) return;

	produceTimer++;
	if (produceTimer < produceCooldown) return;
	produceTimer = 0;

	//item create
	static int global_id = 0;
	buffer = make_shared<Item>(global_id++, t.resource);
	cout << "[Miner] mined resource at (" << x << "," << y << ")\n";
}



//Conveyor
Conveyor::Conveyor(int x, int y, Direction d, Direction fromDir, int cooldown) : Machine(x,y), dir(d), fromDir(fromDir), moveCooldown(cooldown) {}
void Conveyor::update(Map& map) {
	if (!buffer) {
		moving = false;
		moveTimer = 0;
		return;
	}

	int nx, ny;
	Tile* out = nullptr;

	if (!map.getFrontTile(x, y, dir, nx, ny, out)) {
		moving = false;
		moveTimer = 0;
		return;
	}

	bool frontAcceptable = false;
	Machine* fm = out->machine;

	if (fm) {
		frontAcceptable = fm->canAccept(buffer, dir);
	} else if (out->type == TileType::DELIVERY) {
		frontAcceptable = true;
	} else {
		frontAcceptable = false;
	}

	if (!frontAcceptable) {
		moving = false;
		moveTimer = 0;
		return;
	}

	if (!moving) {
		moving = true;
		moveTimer = 0;
	}

	moveTimer++;

	//still moving
	if (moveTimer < moveCooldown)
		return;

	// Push to front machine using unified interface
	if (fm) {
		moveTimer = 0;
		moving = false;
		shared_ptr<Item> item = buffer;
		fm->accept(item);
		buffer = nullptr;
	} else {
		out->item = buffer;
		buffer = nullptr;
	}
}
bool Conveyor::canAccept(shared_ptr<Item> item, Direction dir) {
	if (!item) return false;
    if (buffer && !moving) return false;

	switch (dir) {
		case Direction::UP: return fromDir == Direction::DOWN; break;
		case Direction::DOWN: return fromDir == Direction::UP; break;
		case Direction::LEFT: return fromDir == Direction::RIGHT; break;
		case Direction::RIGHT: return fromDir == Direction::LEFT; break;
		default: return false;
	}
}
void Conveyor::accept(shared_ptr<Item> item) {
	buffer = item;
	moving = false;
	moveTimer = 0;
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
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
Cutter::Cutter(int x, int y, Direction d, int cooldown) : Machine(x,y), dir(d), cooldown(cooldown) {
	switch (dir) {
		case Direction::UP: sx = x; sy = y+1; break;
		case Direction::DOWN: sx = x; sy = y-1; break;
		case Direction::LEFT: sx = x-1; sy = y; break;
		case Direction::RIGHT: sx = x+1; sy = y; break;
	}
}
void Cutter::update(Map& map) {
	auto tryOutput = [&](int ox, int oy, shared_ptr<Item>& slot) {
		if (!slot) return;

		int nx, ny;
		Tile* front = nullptr;
		if (!map.getFrontTile(ox, oy, dir, nx, ny, front) || !front) {
			return;
		}

		Machine* fm = front->machine;
		if (fm && fm->canAccept(slot, dir)) {
			fm->accept(slot);
			slot = nullptr;
		}
	};

	// left output (x, y) outA
	tryOutput(x, y, outA);
	// right output (sx, sy) outB
	tryOutput(sx, sy, outB);

	// still remain
	if (outA || outB) {
		return;
	}

	if (!inputBuffer) {
		timer = 0;
		return;
	}

	timer++;
	if (timer < cooldown) {
		return;
	}
	timer = 0;

	// start cutting
	switch (inputBuffer->res->type) {
		case ResourceType::IRON : {
			// new Resource(ResourceType::IRON_HALF)
			auto halfA = make_shared<Resource>(ResourceType::IRON_HALF);
			auto halfB = make_shared<Resource>(ResourceType::IRON_HALF);
			outA = make_shared<Item>(inputBuffer->id, halfA);
			outB = make_shared<Item>(inputBuffer->id, halfB);
			cout << "[Cutter] at (" << x << "," << y << ") cut IRON into IRON_HALF + IRON_HALF\n";
			break;
		}
		default: return;
	}

	inputBuffer = nullptr;
}
bool Cutter::canAccept(shared_ptr<Item> item, Direction fromdir) {
	if (!item || !item->res->cuttable) return false;

	bool correctInput = false;

	switch (dir) {
		case Direction::UP:    correctInput = (fromdir == Direction::DOWN); break;
		case Direction::DOWN:  correctInput = (fromdir == Direction::UP); break;
		case Direction::LEFT:  correctInput = (fromdir == Direction::RIGHT); break;
		case Direction::RIGHT: correctInput = (fromdir == Direction::LEFT); break;
	}

	if (!correctInput) return false;

	return inputBuffer == nullptr;
}
void Cutter::accept(shared_ptr<Item> item) {
	inputBuffer = item;
	cout << "[Cutter] at (" << x << "," << y << ") accepted item\n";
}



//TrashCan
TrashCan::TrashCan(int x, int y) : Machine(x,y) {}
void TrashCan::update(Map& map) {
	cout << "TrashCan at (" << x << "," << y << ") clear\n";
}

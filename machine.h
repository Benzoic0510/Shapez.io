#ifndef MACHINE_H
#define MACHINE_H

#include <string>
#include <iostream>
#include <memory>
using namespace std;

class Map;    //前置声明
class Item;
class Tile;

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Machine {
	friend class Map;
protected:
	int x, y;
	Tile* selfTile = nullptr;
public:
	Machine(int x, int y);
	virtual ~Machine() = default;
	virtual void update(Map& map) = 0;
	virtual string name() const = 0;
	virtual bool canAccept(shared_ptr<Item> item, Direction dir) = 0;
	virtual void accept(shared_ptr<Item> item) = 0;
	virtual Direction getDirection() const = 0;
};

class Miner : public Machine {
	Direction dir;
	shared_ptr<Item> buffer = nullptr;

	int produceCooldown;    //生产间隔(tick)
	int produceTimer = 0;
public:
	Miner(int x, int y, Direction d, int cooldown = 30);
	void update(Map& map) override;

	string name() const override { return "Miner"; }
	Direction getDirection() const override { return dir; }
	bool canAccept(shared_ptr<Item> item, Direction dir) override { return false; }
	void accept(shared_ptr<Item> item) override { /* do nothing */ }
};

class Conveyor : public Machine {
	Direction dir;
	Direction fromDir;
	shared_ptr<Item> buffer = nullptr;

	int moveCooldown;
	int moveTimer = 0;
	bool moving = false;
public:
	Conveyor(int x, int y, Direction d, Direction fromDir, int cooldown = 15);
	void update(Map& map) override;
	bool canAccept(shared_ptr<Item> item, Direction dir) override;
	void accept(shared_ptr<Item> item) override;

	string name() const override { return "Conveyor"; }
	Direction getDirection() const override { return dir; }
	shared_ptr<Item> getBuffer() const { return buffer; }
	bool isMoving() const { return moving; }
};

class Cutter : public Machine {
	Direction dir;

	int sx, sy;     // second tile

	// Buffer
	shared_ptr<Item> inputBuffer = nullptr;
	shared_ptr<Item> outA = nullptr;
	shared_ptr<Item> outB = nullptr;

	int cooldown = 30;
	int timer = 0;
public:
	Cutter(int x, int y, Direction d, int cooldown = 30);
	void update(Map& map) override;
	bool canAccept(shared_ptr<Item> item, Direction dir) override;
	void accept(shared_ptr<Item> item) override;

	string name() const override { return "Cutter"; }
	Direction getDirection() const override { return Direction::UP; }
};

class TrashCan : public Machine {
	TrashCan(int x, int y);
	void update(Map& map) override;
	bool canAccept(shared_ptr<Item> item, Direction dir) override { return true; }
	void accept(shared_ptr<Item> item) override { /* do nothing */ }

	string name() const override { return "TrashCan"; }
	Direction getDirection() const override { return Direction::UP; }
};
#endif //MACHINE_H
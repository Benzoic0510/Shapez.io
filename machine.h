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
	virtual bool canAccept(shared_ptr<Item> item) = 0;
	virtual void accept(shared_ptr<Item> item) = 0;
};

class Miner : public Machine {
	Direction dir;
	shared_ptr<Item> buffer = nullptr;

	int produceCooldown;    //生产间隔(tick)
	int produceTimer = 0;
public:
	Miner(int x, int y, Direction d, int cooldown = 30);
	void update(Map& map) override;
	string name() const override;
	bool canAccept(shared_ptr<Item> item) override { return false; }
	void accept(shared_ptr<Item> item) override { /* do nothing */ }
};

class Conveyor : public Machine {
	Direction dir;
	shared_ptr<Item> buffer = nullptr;

	int moveCooldown;
	int moveTimer = 0;
public:
	Conveyor(int x, int y, Direction d, int cooldown = 15);
	void update(Map& map) override;
	string name() const override;
	bool canAccept(shared_ptr<Item> item) override;
	void accept(shared_ptr<Item> item) override;
};

class Cutter : public Machine {
public:
	Cutter(int x, int y);
	void update(Map& map) override;
	string name() const override;
	//TODO
	bool canAccept(shared_ptr<Item> item) override { return false; }
	void accept(shared_ptr<Item> item) override { /* do nothing */ }
};

class TrashCan : public Machine {
	TrashCan(int x, int y);
	void update(Map& map) override;
	string name() const override;
	//TODO
	bool canAccept(shared_ptr<Item> item) override { return false; }
	void accept(shared_ptr<Item> item) override { /* do nothing */ }
};
#endif //MACHINE_H
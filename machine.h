#ifndef MACHINE_H
#define MACHINE_H

#include <string>
#include <iostream>
using namespace std;

class Map;    //前置声明

enum class Direction { UP, DOWN, LEFT, RIGHT };

class Machine {
	friend class Map;
protected:
	int x, y;
public:
	Machine(int x, int y);
	virtual ~Machine() = default;
	virtual void update(Map& map) = 0;
	virtual string name() const = 0;
};

class Miner : public Machine {
	Direction dir;
public:
	Miner(int x, int y, Direction d);
	void update(Map& map) override;
	string name() const override;
};

class Conveyor : public Machine {
	Direction dir;
public:
	Conveyor(int x, int y, Direction d);
	void update(Map& map) override;
	string name() const override;
};

class Cutter : public Machine {
public:
	Cutter(int x, int y);
	void update(Map& map) override;
	string name() const override;
};

class TrashCan : public Machine {
	TrashCan(int x, int y);
	void update(Map& map) override;
	string name() const override;
};
#endif //MACHINE_H

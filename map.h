#ifndef MAP_H
#define MAP_H

#include <vector>
#include <memory>
#include "tile.h"
#include "delivery.h"
#include "machine.h"
#include "resource.h"
using namespace std;

class Map {
	friend class Miner;
	friend class Conveyor;
private:
	int rows, cols;
	vector<vector<Tile>> grid;             //地图网络
	Delivery hub;                          //交付中心
	vector<unique_ptr<Machine>> machines;  //机器对象
public:
	Map(int r, int c);
	void placeDeliveryHub(int x, int y, int w, int h);  //放置交付中心
	void addResource(int x, int y, ResourceType type);  //添加矿物
	void placeMachine(Machine* m);                      //放置机器
	void tick();
	int getCoins();
	bool inBounds(int x, int y);
};

#endif //MAP_H

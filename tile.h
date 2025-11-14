#ifndef TILE_H
#define TILE_H

#include <memory>
#include "resource.h"
#include "item.h"
using namespace std;

class Machine;   // 前置声明

enum class TileType { EMPTY, RESOURCE, DELIVERY };

class Tile {
public:
	int x, y;
	TileType type = TileType::EMPTY;

	shared_ptr<Resource> resource;    // 静态地块矿物
	shared_ptr<Item> item = nullptr;  // 当前物品（只有DeliveryHub使用）
	Machine* machine = nullptr;       // 当前设备

	Tile(int x=0, int y=0) : x(x), y(y) {}
};

#endif //TILE_H

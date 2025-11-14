#include <cassert>
#include "delivery.h"
#include "machine.h"
#include "map.h"
#include "resource.h"
#include "tile.h"
#include "item.h"


int main() {
	Map m(10,10);
	m.placeDeliveryHub(4,1,2,2);
	m.addResource(1,1, ResourceType::IRON);
	m.placeMachine(new Miner(1,1,Direction::DOWN));
	m.placeMachine(new Conveyor(2,1,Direction::DOWN,Direction::UP));
	m.placeMachine(new Conveyor(3,1,Direction::DOWN,Direction::UP));
	int before = m.getCoins();
	for (int i=0;i<100;i++) m.tick();
	assert(m.getCoins() > before && "Miner->Conveyor->Hub delivery failed");
	return 0;
}
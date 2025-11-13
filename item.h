#ifndef ITEM_H
#define ITEM_H

#include <memory>
#include "resource.h"

class Item {
public:
	int id;
	shared_ptr<Resource> res;

	Item(int id, shared_ptr<Resource> r) : id(id), res(r) {}
};

#endif //ITEM_H

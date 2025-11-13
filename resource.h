#ifndef RESOURCE_H
#define RESOURCE_H

#include <string>
using namespace std;

enum class ResourceType { IRON, COPPER, IRON_HALF };

class Resource {
public:
	ResourceType type;
	int value;
	bool cuttable;

	Resource(ResourceType t);
	string name() const;
};

#endif //RESOURCE_H

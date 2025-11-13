#include "resource.h"

Resource::Resource(ResourceType t) : type(t) {
	switch (t) {
		case ResourceType::IRON: value = 10; cuttable = true; break;
		case ResourceType::COPPER: value = 15; cuttable = false; break;
		case ResourceType::IRON_HALF: value = 5; cuttable = false; break;
	}
}

string Resource::name() const {
	switch (type) {
		case ResourceType::IRON: return "Iron";
		case ResourceType::COPPER: return "Copper";
		case ResourceType::IRON_HALF: return "Iron_Half";
	}
	return "Unknown";
}

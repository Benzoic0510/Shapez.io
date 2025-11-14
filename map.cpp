#include "map.h"
#include <algorithm>
#include <queue>
#include <unordered_map>
using namespace std;


Map::Map(int r, int c) : rows(r) , cols(c) {
	grid.resize(rows, vector<Tile>(cols));
	for (int i=0; i<r; i++) {
		for (int j=0; j<c; j++) {
			grid[i][j] = Tile(i,j);
		}
	}
}

void Map::placeDeliveryHub(int x, int y, int w, int h) {
	for (int i=x;i<x+w;i++)
		for (int j=y;j<y+h;j++)
			grid[i][j].type = TileType::DELIVERY;
	cout << "Delivery hub placed at (" << x << "," << y << ")\n";
}

void Map::addResource(int x, int y, ResourceType type) {
	grid[x][y].type = TileType::RESOURCE;
	grid[x][y].resource = make_shared<Resource>(type);
	cout << "Added resource:[" << grid[x][y].resource->name()
		 << "] at (" << x << "," << y << ")\n";
}

void Map::placeMachine(Machine* m) {
	machines.emplace_back(m);
	grid[m->x][m->y].machine = m;
	grid[m->x][m->y].machine->selfTile = &grid[m->x][m->y];
	cout << "Placed machine: " << m->name() << "\n";
}

/*
void Map::tick() {
	for (auto& m : machines)
		m->update(*this);

	//[Delivery] part
	for (int i=0;i<rows;i++) {
		for (int j=0;j<cols;j++) {
			if (grid[i][j].type == TileType::DELIVERY && grid[i][j].item) {
				hub.receive(grid[i][j].item->res->value);
				cout << "[Delivery] got " << grid[i][j].item->res->name() << "\n";
				grid[i][j].item = nullptr;
			}
		}
	}
	hub.report();
}
*/
void Map::tick() {
    vector<Machine*> all;
    all.reserve(machines.size());
    for (auto& ptr : machines) all.push_back(ptr.get());

    // 1. Compute the levels of all machines
    unordered_map<Machine*, int> level;
    queue<Machine*> q;

    // 1.1 downstream machine
    for (auto* m : all) {
        int nx, ny;
        Tile* front = nullptr;
        if (!getFrontTile(m->x, m->y, m->getDirection(), nx, ny, front)) {
            level[m] = 0;
            q.push(m);
            continue;
        }

        if (!front->machine) {
            level[m] = 0;
            q.push(m);
        }
    }

    // 1.2 BFS from the downstream & assign numbers to all machines
    while (!q.empty()) {
        Machine* cur = q.front();
        q.pop();
        int L = level[cur];

        for (auto* m : all) {
            // if already have level, skip
            if (level.count(m)) continue;

            int nx, ny;
            Tile* front = nullptr;
            if (!getFrontTile(m->x, m->y, m->getDirection(), nx, ny, front))
                continue;

            if (front && front->machine == cur) {
                level[m] = L + 1;
                q.push(m);
            }
        }
    }

    // Unnumbered (isolated machines)
    const int INF_LEVEL = 999999;
    for (auto* m : all) {
        if (!level.count(m)) level[m] = INF_LEVEL;
    }

    // 1.3 Sort by hierarchy, updating the lower levels (downstream) first
    sort(all.begin(), all.end(), [&](Machine* a, Machine* b) {
        if (level[a] != level[b]) return level[a] < level[b];
        if (a->x != b->x) return a->x < b->x;
        return a->y < b->y;
    });

    // 2. update in order
    for (auto* m : all) {
        m->update(*this);
    }

    // 3. handle Delivery
	for (int i=0;i<rows;i++) {
		for (int j=0;j<cols;j++) {
			if (grid[i][j].type == TileType::DELIVERY && grid[i][j].item) {
				hub.receive(grid[i][j].item->res->value);
				cout << "[Delivery] got " << grid[i][j].item->res->name() << "\n";
				grid[i][j].item = nullptr;
			}
		}
	}

    hub.report();
}

int Map::getCoins() {
	return hub.getCoins();
}

bool Map::inBounds(int x, int y) const {
	return x>=0 && x<rows && y>=0 && y<cols;
}

bool Map::getFrontTile(int x, int y, Direction dir, int& nx, int& ny, Tile*& tile) {
	nx = x;
	ny = y;

	switch (dir) {
		case Direction::UP:    nx--; break;
		case Direction::DOWN:  nx++; break;
		case Direction::LEFT:  ny--; break;
		case Direction::RIGHT: ny++; break;
	}

	if (!this->inBounds(nx, ny)) {
		tile = nullptr;
		return false;
	}

	tile = &grid[nx][ny];
	return true;
}

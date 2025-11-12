#ifndef DELIVERY_H
#define DELIVERY_H

#include <bits/stdc++.h>
using namespace std;

class Delivery {
private:
	int deliveredCount = 0;
	int coins = 0;
public:
	void receive(int value);
	void report() const;
	int getCoins() const;
	int getDeliveredCount() const;
};

#endif //DELIVERY_H

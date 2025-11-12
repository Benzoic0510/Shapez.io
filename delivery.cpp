#include "delivery.h"

void Delivery::receive(int value) {
	deliveredCount++;
	coins += value;
}

void Delivery::report() const {
	cout << "Delivered items: " << deliveredCount
		 << ", Total coins: " << coins << "\n";
}

int Delivery::getCoins() const { return coins; }

int Delivery::getDeliveredCount() const { return deliveredCount; }
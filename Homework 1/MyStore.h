#pragma once

#include "../HW1-Tests/interface.h"
#include "Queue.h"
#include "PriorityQueue.h"
#include <iostream>
#include "MyClient.h"
#include "myWorker.h"

struct MyStore : public Store {
private:
	ActionHandler* actionHandler = nullptr;

	Queue<MyClient> clients;
	int workerCount;

	int availableBananas;
	int availableSchweppes;

	int currMin;

	int requestedBananasFromCustomers = 0;
	int requestedSchweppesFromCustomers = 0;

	int expectedBananasForDelivery = 0;
	int expectedSchweppesForDelivery = 0;

	Queue<MyWorker> departedWorkersForBananas;
	Queue<MyWorker> departedWorkersForSchweppes;

	PriorityQueue<MyClient> waitingCustomers;

	int availableWorkers;

public:
	MyStore();
	void setActionHandler(ActionHandler* handler)override;
	void init(int workerCount, int startBanana, int startSchweppes) override;
	void addClients(const Client* clients, int count) override;
	void advanceTo(int minute) override;
	virtual int getBanana() const;
	virtual int getSchweppes() const;
};


MyStore::MyStore() {
	this->actionHandler = nullptr;
	this->workerCount = 0;
	this->availableBananas = 0;
	this->availableSchweppes = 0;
	this->currMin = 0;
}

void MyStore::setActionHandler(ActionHandler* handler) {
	actionHandler = handler;
}


void MyStore::init(int workerCount, int startBanana, int startSchweppes) {

	this->workerCount = workerCount;
	this->availableBananas = startBanana;
	this->availableSchweppes = startSchweppes;
	this->availableWorkers = workerCount;
}

void MyStore::addClients(const Client* clients, int count) {

	for (int i = 0; i < count; ++i) {

		MyClient currClient(i, clients[i].arriveMinute, clients[i].banana, clients[i].schweppes, clients[i].maxWaitTime);
		this->clients.enqueue(currClient);
	}
}

void MyStore::advanceTo(int minute) {



	while (currMin <= minute)
	{
		if (!departedWorkersForBananas.isEmpty()) {
			while (!departedWorkersForBananas.isEmpty() && departedWorkersForBananas.first().arrivalTime() == currMin) {
				availableBananas += 100;
				expectedBananasForDelivery -= 100;
				departedWorkersForBananas.dequeue();
				availableWorkers++;

				actionHandler->onWorkerBack(currMin, ResourceType::banana);
			}
		}

		if (!departedWorkersForSchweppes.isEmpty()) {
			while (!departedWorkersForSchweppes.isEmpty() && departedWorkersForSchweppes.first().arrivalTime() == currMin) {
				availableSchweppes += 100;
				expectedSchweppesForDelivery -= 100;
				departedWorkersForSchweppes.dequeue();
				availableWorkers++;

				actionHandler->onWorkerBack(currMin, ResourceType::schweppes);
			}
		}

		while (!clients.isEmpty() && currMin == clients.first().arriveMinute) {
			bool hasBananas = clients.first().banana <= availableBananas;
			bool hasSchweppes = clients.first().schweppes <= availableSchweppes;

			if (hasBananas && hasSchweppes) {
				availableBananas -= clients.first().banana;
				availableSchweppes -= clients.first().schweppes;

				actionHandler->onClientDepart(clients.first().index, currMin, clients.first().banana, clients.first().schweppes);
			}
			else {
				waitingCustomers.enqueue(clients.first());

				if (!hasBananas) {
					requestedBananasFromCustomers += clients.first().banana;
				}
				if (!hasSchweppes) {
					requestedSchweppesFromCustomers += clients.first().schweppes;
				}
			}

			int neededBananas = abs(availableBananas - clients.first().banana);
			int neededSchweppes = abs(availableSchweppes - clients.first().schweppes);

			if ((availableWorkers == 1 && (neededBananas >= neededSchweppes))  || (expectedBananasForDelivery + availableBananas < requestedBananasFromCustomers && availableWorkers > 1)) {
				while (expectedBananasForDelivery + availableBananas < requestedBananasFromCustomers && availableWorkers > 0) {
					MyWorker currWorkerB(currMin);
					departedWorkersForBananas.enqueue(currWorkerB);

					actionHandler->onWorkerSend(currMin, ResourceType::banana);
					expectedBananasForDelivery += 100;
					availableWorkers--;
				}
			}
			else {
				while (expectedSchweppesForDelivery + availableSchweppes < requestedSchweppesFromCustomers && availableWorkers > 0) {
					MyWorker currWorkerS(currMin);
					departedWorkersForSchweppes.enqueue(currWorkerS);

					actionHandler->onWorkerSend(currMin, ResourceType::schweppes);
					expectedSchweppesForDelivery += 100;
					availableWorkers--;
				}
			}

			clients.dequeue();
		}

		while (!waitingCustomers.isEmpty()) {
			bool hasBananas = waitingCustomers.first().banana <= availableBananas;
			bool hasSchweppes = waitingCustomers.first().schweppes <= availableSchweppes;

			if (hasBananas && hasSchweppes) { //proverka dali ima dostatuchno B&S
				availableBananas -= waitingCustomers.first().banana;
				availableSchweppes -= waitingCustomers.first().schweppes;

				requestedBananasFromCustomers -= waitingCustomers.first().banana;
				requestedSchweppesFromCustomers -= waitingCustomers.first().schweppes;

				actionHandler->onClientDepart(waitingCustomers.first().index, currMin, waitingCustomers.first().banana, waitingCustomers.first().schweppes);
				waitingCustomers.dequeue();
			}
			else if (waitingCustomers.first().getClientDepart() == currMin) {

				if (hasBananas && !hasSchweppes) {
					actionHandler->onClientDepart(waitingCustomers.first().index, currMin, waitingCustomers.first().banana, availableSchweppes);
				}
				else if (!hasBananas && hasSchweppes) {
					actionHandler->onClientDepart(waitingCustomers.first().index, currMin, availableBananas, waitingCustomers.first().schweppes);
				}
				else {

					actionHandler->onClientDepart(waitingCustomers.first().index, currMin, availableBananas, availableSchweppes);
				}
				if (availableBananas < waitingCustomers.first().banana) {
					availableBananas = 0;
				}
				else {
					availableBananas -= waitingCustomers.first().banana;
				}
				if (availableSchweppes < waitingCustomers.first().schweppes) {
					availableSchweppes = 0;
				}
				else {
					availableSchweppes -= waitingCustomers.first().schweppes;
				}

				requestedBananasFromCustomers -= waitingCustomers.first().banana;
				requestedSchweppesFromCustomers -= waitingCustomers.first().schweppes;

				waitingCustomers.dequeue();
			}
			else {
				break;
			}
		}

		++currMin;
	}

	currMin = minute;
}

int MyStore::getBanana() const {
	return this->availableBananas;
}

int MyStore::getSchweppes() const {
	return this->availableSchweppes;
}

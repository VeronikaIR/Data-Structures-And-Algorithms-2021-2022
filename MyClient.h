#pragma once
#include "../HW1-Tests/interface.h"


struct MyClient : public Client
{
	int index;
public:

	MyClient() {
		this->index = -1;
		this->arriveMinute = -1;
		this->banana = -1;
		this->schweppes = -1;
		this->maxWaitTime = -1;
	}

	MyClient(int idx, int arriveMinute, int banana, int schweppes, int maxWaitTime) {
		this->index = idx;
		this->arriveMinute = arriveMinute;
		this->banana = banana;
		this->schweppes = schweppes;
		this->maxWaitTime = maxWaitTime;
	}

	int getClientDepart()const {
		return this->arriveMinute + this->maxWaitTime;
	}

	bool operator<(MyClient& other) {
		if (this->getClientDepart() < other.getClientDepart()) {
			return true;
		}
		else if (this->getClientDepart() == other.getClientDepart()) {
			if (this->index < other.index) {
				return true;
			}
			else {
				return false;
			}
		}
		return false;
	}
};


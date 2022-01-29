#pragma once

struct MyWorker
{
	int startMin;

	MyWorker() {
		this->startMin = -1;
	}
	MyWorker(int minute) {
		this->startMin = minute;
	}
	int arrivalTime()const {
		return startMin + 60;
	}
};
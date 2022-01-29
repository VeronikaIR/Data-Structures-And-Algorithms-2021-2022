#include <iostream>
#include "MyStore.h"

struct  ConsoleActionHandler : public ActionHandler
{
	 void onWorkerSend(int minute, ResourceType resource) {
		 if (resource == ResourceType::banana) {
			 std::cout << "W" << " " << minute << " " << "banana" << std::endl;
		 }
		 else {
			 std::cout << "W" << " " << minute << " " << "schweppes" << std::endl;
		 }
	 }

	 void onWorkerBack(int minute, ResourceType resource) {
		 if (resource == ResourceType::banana) {
			 std::cout << "D" << " " << minute << " " << "banana" << std::endl;
		 }
		 else {
			 std::cout << "D" << " " << minute << " " << "schweppes" << std::endl;
		 }
	 }

	 void onClientDepart(int index, int minute, int banana, int schweppes) {
		 std::cout << index << " " << minute << " " << banana << " " << schweppes << std::endl;
	 }
};

int main() {
	
	int W;
	std::cout << "Enter number of workers: ";
	std::cin >> W;
	
	int C;
	std::cout << "Enter number of clients: ";
	std::cin >> C;


	Client* clients = new Client[C];

	for (int i = 0; i < C; ++i) {
		
		int arrivalTime;
		int amountOfBananas;
		int amountOfSchweppes;
		int maxWaitingTime;

		std::cout << "Customer [" << i <<"]: ";
		std::cin >> arrivalTime;
		std::cin >> amountOfBananas;
		std::cin >> amountOfSchweppes;
		std::cin >> maxWaitingTime;

		Client currentClient{ arrivalTime,amountOfBananas,amountOfSchweppes,maxWaitingTime };
		clients[i] = currentClient;
	}

	MyStore st;
	st.init(W, 0, 0);
	st.addClients(clients, C);

	ConsoleActionHandler handler;
	st.setActionHandler(&handler);

	std::cout << std::endl;
	std::cout << "Output:" << std::endl;
	st.advanceTo(10);
	st.advanceTo(100);
	st.advanceTo(239);


	delete[] clients;

	return 0;
}
#include <iostream>
#include <queue>

std::queue<int> pushInQueue(const int& numberOfKnights) {

    std::queue<int> queueOfPlaces;

    for (int i = 0; i < numberOfKnights; ++i) {
        queueOfPlaces.push(i+1);
    }
    return queueOfPlaces;
}

int findWinningPlace(int numberOfKnights) {

    std::queue<int> queueOfPlaces = pushInQueue(numberOfKnights);

    std::queue<int> tempQueue;

    while (queueOfPlaces.size() !=1 || tempQueue.size() !=1) {

        if (!queueOfPlaces.empty()) {
            if (numberOfKnights % 2 != 0) {
                queueOfPlaces.pop();
            }
            else {
                tempQueue.push(queueOfPlaces.front());
                queueOfPlaces.pop();
            }
            while (! queueOfPlaces.empty())
            {
              
                queueOfPlaces.pop();
                if (queueOfPlaces.empty()) {
                    break;
                }
                tempQueue.push(queueOfPlaces.front());
                queueOfPlaces.pop();
               
            }
            if (tempQueue.size() == 1) { return tempQueue.front(); }

            numberOfKnights = tempQueue.size();
        }
        if (!tempQueue.empty()) { 

            if (numberOfKnights % 2 != 0) {
                tempQueue.pop(); 
            }
            else {
                queueOfPlaces.push(tempQueue.front());
                tempQueue.pop();
            }

            while (!tempQueue.empty())
            {
                tempQueue.pop();
                if (tempQueue.empty()) {
                    break;
                }
                queueOfPlaces.push(tempQueue.front());
                tempQueue.pop();
            }
            if (queueOfPlaces.size() == 1) { return queueOfPlaces.front(); }
            numberOfKnights = queueOfPlaces.size();
        }
    }
}


int main()
{
    unsigned int numberOfKnights;
    
    std::cout << "Enter number of knights: ";
    std::cin >> numberOfKnights;

    unsigned int winningPlace = findWinningPlace(numberOfKnights);
    std::cout << winningPlace << std::endl;

    return 0;
}



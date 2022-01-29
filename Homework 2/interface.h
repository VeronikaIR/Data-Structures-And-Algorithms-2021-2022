#pragma once

#include <string>
#include <queue>
#include <stack>
#include <vector>
#include "../Homework 2 - implementation/GenericTree.h"
#include "interface.h"

#include <fstream>

using std::string;


struct  WorkerRelation
{
    string manager;
    string subordinate;
};
 
class Hierarchy
{
public:
    Hierarchy() {}
    Hierarchy(Hierarchy&& r) noexcept;
    Hierarchy(const Hierarchy& r);
    Hierarchy(const string& data);
    ~Hierarchy() noexcept;
    void operator=(const Hierarchy&) = delete;

    string print()const;

    int longest_chain() const;
    bool find(const string& name)const;
    int num_employees() const;
    int num_overloaded(int level = 20) const;

    string manager(const string& name) const;
    int num_subordinates(const string& name) const;
    unsigned long getSalary(const string& who) const;

    bool fire(const string& who);
    bool hire(const string& who, const string& boss);

    void incorporate();
    void modernize();

    Hierarchy join(const Hierarchy& right)const;

    //If you need it - add more public methods here
    void setTree(const string& data);
    void setNameOfTheHierarchy(const string& name) { this->nameOfTheHierarchy = name; }
    const string getNameOfTheHierarchy()const { return this->nameOfTheHierarchy; }

private:

    string nameOfTheHierarchy;
    GenericTree<string> tree;

};

Hierarchy::~Hierarchy()noexcept{}

Hierarchy::Hierarchy(Hierarchy&& r) noexcept {
    this->nameOfTheHierarchy = r.nameOfTheHierarchy;
    this->tree.setRootPtr(r.tree.getRootPtr());
}

Hierarchy::Hierarchy(const Hierarchy& r) {
    this->nameOfTheHierarchy = r.nameOfTheHierarchy;

    GenericTree<string>::TreeNode* rootPtr = r.tree.getRootPtr();
    try {
        this->tree.createRoot(rootPtr->data);
    }
    catch (string e) {
        std::cout << e << std::endl;
    }
    std::queue<GenericTree<string>::TreeNode*> nodesToExplore;
    nodesToExplore.push(rootPtr);

    while (!nodesToExplore.empty()) {

        GenericTree<string>::TreeNode* frontNode = nodesToExplore.front();
        if (frontNode->data != this->tree.getRootPtr()->data) {
            GenericTree<string>::TreeNode* parentNode = tree.returnNode(frontNode->parentPtr->data);
            GenericTree<string>::TreeNode* childNode = tree.returnNode(frontNode->data);

            if (parentNode != nullptr) {
                if (childNode == nullptr) {
                    parentNode->addChild(frontNode->data);
                }
                else {
                    this->tree.moveNode(childNode->data, parentNode->data);
                }
            }
        }

        nodesToExplore.pop();

        std::vector<GenericTree<string>::TreeNode*> compressedChildrenPtrs;
        for (auto childPtr : frontNode->childrenPtrs) {
            if (childPtr) {
                compressedChildrenPtrs.push_back(childPtr);
                nodesToExplore.push(childPtr);
            }
        }
        frontNode->childrenPtrs.swap(compressedChildrenPtrs);
    }

}


void Hierarchy::setTree(const string& data) {

    std::queue<WorkerRelation> workers;
    int index = 0;

    string currName;
    string currManagerName;
    string currSubordinateName;

    while (index < data.size()) {

        while (data[index] == ' ') { index++; }

        while ((data[index] != ' ' && data[index] != '-') && (index < data.size() - 1)) {

            currName.push_back(data[index]);
            index++;
        }
        currManagerName = currName;
        currName.clear();

        while (data[index] == '-' || data[index] == ' ')
        {
            index++;
        }

        while (data[index] != '\n' && data[index] != '-' && data[index] != ' ' && (index < data.size() - 1)) {

            currName.push_back(data[index]);
            index++;
        }
        currSubordinateName = currName;
        currName.clear();

        WorkerRelation currWorker{ currManagerName,currSubordinateName };
        workers.push(currWorker);


        if (data[index] == 13) {
            index++;
        }
        while (data[index] == ' '){
            index++;
            if (data[index] == '\n') { break; }
        }
        index++;
    }

    this->tree.createRoot(workers.front().manager);

    while (!workers.empty()) {

        currManagerName = workers.front().manager;

        while (workers.front().manager == currManagerName) {
            tree.returnNode(currManagerName)->addChild(workers.front().subordinate);
            workers.pop();
            if (workers.empty()) { break; }
        }
    }
}

void extractCurName(string& currName, string& currManager, string& currSubordinate) {
    if (currName != "") {
        if (currManager == "") {
            currManager = currName;
        }
        else if (currSubordinate == "") {
            currSubordinate = currName;
        }
        else {
            throw std::invalid_argument("received multiple workers on same line");
        }
        currName = "";
    }
}

void pushWorkerRelation(std::queue<WorkerRelation>& workers, string& currName, string& currManager, string& currSubordinate, int& numberOfDashes) {
    extractCurName(currName, currManager, currSubordinate);

    if (currManager == "") {
        throw std::invalid_argument("invalid data format: missing manager on line");
    }

    if (currSubordinate == "") {
        throw std::invalid_argument("invalid data format: missing subordinate on line");
    }
    workers.push(WorkerRelation{ currManager, currSubordinate });
    currManager = "", currSubordinate = "", numberOfDashes = 0;
}

Hierarchy::Hierarchy(const string& data) {

    std::queue<WorkerRelation> workers;

    string currName = "", currManager = "", currSubordinate = "";

    int numberOfDashes = 0;
    WorkerRelation currWorker;

    for (int idx = 0; idx < data.length(); idx++) {
        switch (data[idx]) {
        case ' ':
            extractCurName(currName, currManager, currSubordinate);
            break;
        case '-':
            numberOfDashes += 1;
            if ((currManager != "" && currSubordinate != "") || numberOfDashes > 1) {
                throw std::invalid_argument("received multiple '-' separaters");
            }
            extractCurName( currName, currManager, currSubordinate);
            break;
        case '\r':
        case '\n':
            pushWorkerRelation( workers, currName, currManager, currSubordinate, numberOfDashes);
            break;
        default:
            if (currManager != "" && numberOfDashes != 1) {
                throw std::invalid_argument("invalid data format: cannot declare subordinate without previous '-' sign");
            }
            currName += data[idx];
            if (idx == data.length() - 1) {
                pushWorkerRelation( workers, currName, currManager, currSubordinate, numberOfDashes);
            }
            break;
        }
    }
   
    if (!workers.empty()) {
        this->tree.createRoot(workers.front().manager);
    }

    while (!workers.empty()) {

        currManager = workers.front().manager;
        while (workers.front().manager == currManager){

            this->tree.addChild(currManager, workers.front().subordinate);

            workers.pop();
            if (workers.empty()) { break; }
        }
    }
}

bool Hierarchy::find(const string& name)const {

    return (tree.returnNode(name) != nullptr);
}

int Hierarchy::num_subordinates(const string& name) const {

    return tree.sizeOfChildren(name);
}

string Hierarchy::manager(const string& name) const {
    GenericTree<string>::TreeNode* node = tree.returnParentPtr(name);
    return node != nullptr ? tree.returnParentPtr(name)->data : "";

}

int Hierarchy::num_employees() const {

    return tree.sizeOfTree();
}


int Hierarchy::num_overloaded(int level) const {

    int counter = 0;
    std::queue<GenericTree<string>::TreeNode*> nodesToExplore;
    GenericTree<string>::TreeNode* rootNodePtr = tree.getRootPtr();

    if (!rootNodePtr) {
        return 0;
    }

    nodesToExplore.push(rootNodePtr);
    while (!nodesToExplore.empty()) {

        GenericTree<string>::TreeNode* frontNode = nodesToExplore.front();
        nodesToExplore.pop();

        if (!frontNode) {
            throw std::runtime_error("Error: Compression exploration queued a null pointer");
        }

        if (tree.sizeOfSubsTree(frontNode) - 1 > level) {
            counter++;
        }

        for (auto childPtr : frontNode->childrenPtrs) {
            if (childPtr) {

                nodesToExplore.push(childPtr);
            }
        }

    }
    return counter;
}

bool Hierarchy::fire(const string& who) {

    if (who == "Uspeshnia") { return false; }

    int sizeBeforeRemoveElement = tree.sizeOfTree();
    tree.removeNode(who);
    int sizeAfterRemoveElement = tree.sizeOfTree();

    return (sizeAfterRemoveElement < sizeBeforeRemoveElement);
}

int Hierarchy::longest_chain() const {

    return tree.heightOfTree(tree.getRootPtr());
}

unsigned long Hierarchy::getSalary(const string& who) const {

     if (tree.returnNode(who) == nullptr) {
         return -1;
     }
    unsigned long directSubordinates = tree.returnNode(who)->childrenPtrs.size();
    unsigned long indirectSubordinates = tree.sizeOfSubsTree(tree.returnNode(who)) - directSubordinates - 1;

    return (500 * directSubordinates) + (50 * indirectSubordinates);
}

bool Hierarchy::hire(const string& who, const string& boss) {
    GenericTree<string>::TreeNode* bossNode = tree.returnNode(boss);
    GenericTree<string>::TreeNode* whoNode = tree.returnNode(who);
   
    if (bossNode != nullptr) {
        if (whoNode == nullptr) {
            bossNode->addChild(who);
            return true;
        }
        return tree.moveNode(who, boss);
    }
    return false;
}

void Hierarchy::incorporate() {

    std::queue<GenericTree<string>::TreeNode*> nodesToExplore;
    std::queue<GenericTree<string>::TreeNode*>sortedNodes;

    nodesToExplore.push(tree.getRootPtr());

    while (!nodesToExplore.empty()) {

        GenericTree<string>::TreeNode* curNode = nodesToExplore.front();
        nodesToExplore.pop();

        //if nullptr..
        if (!curNode) { continue; } //// ?????

        sortedNodes.push(curNode);

        for (auto child : curNode->childrenPtrs) {
            nodesToExplore.push(child);
        }
    }

    std::stack<GenericTree<string>::TreeNode*> reverse;

    while (!sortedNodes.empty()) {

        reverse.push(sortedNodes.front());

        sortedNodes.pop();
    }

    while (!reverse.empty()) {
        GenericTree<string>::TreeNode* curNode = reverse.top();
        if (curNode->parentPtr == nullptr) { break; }

        int sizeOfTeam = curNode->parentPtr->childrenPtrs.size();

        if (sizeOfTeam < 2) {
            reverse.pop();
            continue;
        }
        std::vector<GenericTree<string>::TreeNode*> team;

        team.push_back(curNode);
        reverse.pop();

        int maxSalary = this->getSalary(team[0]->data);
        GenericTree<string>::TreeNode* personWithMaxSalary = team[0];

        int i = 0;
        while (i < sizeOfTeam - 1) { //todo for

            team.push_back(reverse.top());
            reverse.pop();
            i++;
        }

        for (int j = 0; j < sizeOfTeam; ++j) {
            int curSalary = this->getSalary(team[j]->data);
            if (curSalary > maxSalary) {
                maxSalary = curSalary;
                personWithMaxSalary = team[j];
            }
            if (curSalary == maxSalary) {
                if (team[j]->data < personWithMaxSalary->data) {
                    personWithMaxSalary = team[j];
                }
            }
        }

        for (int j = 0; j < sizeOfTeam; j++) {
            if (team[j]->data == personWithMaxSalary->data) { continue; }
            else {
                this->hire(team[j]->data, personWithMaxSalary->data);
            }
        }
    }
}

string Hierarchy::print()const {

    std::queue<GenericTree<string>::TreeNode*> nodesToExplore;
    std::queue<GenericTree<string>::TreeNode*>sortedNodes;

    nodesToExplore.push(tree.getRootPtr());

    while (!nodesToExplore.empty()) {

        GenericTree<string>::TreeNode* curNode = nodesToExplore.front();
        nodesToExplore.pop();

        if (!curNode) { continue; } 

        sortedNodes.push(curNode);

        for (auto child : curNode->childrenPtrs) {
            nodesToExplore.push(child);
        }
    }
    string toReturn = "";

    int index = 0;

    while (!sortedNodes.empty()) {

        if (index == 0) {
            sortedNodes.pop();
            index++;
            continue;
        }
        string currentString;

        GenericTree<string>::TreeNode* frontNode = sortedNodes.front();
        sortedNodes.pop();

        if (sortedNodes.empty()) {
            currentString += frontNode->parentPtr->data;
            currentString += ("-");
            currentString += frontNode->data;
            currentString += ("\n");

            toReturn += currentString;
            break;
        }

        std::vector<GenericTree<string>::TreeNode*> sameLevelNodes;
        sameLevelNodes.push_back(frontNode);

        while (tree.getNodeLevel(frontNode) == tree.getNodeLevel(sortedNodes.front())) {

            sameLevelNodes.push_back(sortedNodes.front());
            sortedNodes.pop();

            if (sortedNodes.empty()) { break; }
        }

        int nodesOnSameLevel = sameLevelNodes.size() - 1;

        for (int i = 0; i < nodesOnSameLevel; i++) {
            for (int j = 0; j < nodesOnSameLevel - i; j++) {
                if (sameLevelNodes[j]->parentPtr->data > sameLevelNodes[j + 1]->parentPtr->data) {
                    GenericTree<string>::TreeNode* temp = sameLevelNodes[j];
                    sameLevelNodes[j] = sameLevelNodes[j + 1];
                    sameLevelNodes[j + 1] = temp;
                }
                if (sameLevelNodes[j]->parentPtr->data == sameLevelNodes[j + 1]->parentPtr->data) {
                    if (sameLevelNodes[j]->data > sameLevelNodes[j + 1]->data) {
                        GenericTree<string>::TreeNode* temp = sameLevelNodes[j];
                        sameLevelNodes[j] = sameLevelNodes[j + 1];
                        sameLevelNodes[j + 1] = temp;
                    }
                }
            }
        }

        for (int i = 0; i < nodesOnSameLevel + 1; i++) {
            currentString += sameLevelNodes[i]->parentPtr->data;
            currentString += ("-");
            currentString += sameLevelNodes[i]->data;
            currentString += ("\n");

        }
        toReturn += currentString;
        index++;
    }
    return toReturn;
}

void Hierarchy::modernize() {

    std::queue<GenericTree<string>::TreeNode*> nodesToExplore;
    std::queue<GenericTree<string>::TreeNode*>sortedNodes;
    nodesToExplore.push(tree.getRootPtr());

    while (!nodesToExplore.empty()) {

        GenericTree<string>::TreeNode* curNode = nodesToExplore.front();
        nodesToExplore.pop();

        if (!curNode) { continue; }

        sortedNodes.push(curNode);

        for (auto child : curNode->childrenPtrs) {
            nodesToExplore.push(child);
        }
    }

    std::stack<GenericTree<string>::TreeNode*> reverse;

    while (!sortedNodes.empty()) {

        reverse.push(sortedNodes.front());
        sortedNodes.pop();
    }

    while (!reverse.empty()) {
        GenericTree<string>::TreeNode* curNode = reverse.top();

        if (reverse.size() == 1) {
            break;
        }

        int curHeight = this->tree.heightOfTree(this->tree.getRootPtr()) - (this->tree.heightOfTree(curNode));

        if ((curHeight % 2 != 0) && (curNode->childrenPtrs.size() >= 1)) {
            this->tree.removeNode(curNode->data);

        }
        reverse.pop();
    }
}

Hierarchy Hierarchy::join(const Hierarchy& right)const {

    std::queue<GenericTree<string>::TreeNode*> nodesToExplore;
    std::queue<GenericTree<string>::TreeNode*>sortedNodes;

    string thisHierarchy = this->print();

    Hierarchy* newHierarchy = new Hierarchy(thisHierarchy);

    nodesToExplore.push(right.tree.getRootPtr());

    while (!nodesToExplore.empty()) {

        GenericTree<string>::TreeNode* curNode = nodesToExplore.front();
        nodesToExplore.pop();

        if (!curNode) { continue; }

        sortedNodes.push(curNode);

        for (auto child : curNode->childrenPtrs) {
            nodesToExplore.push(child);
        }
    }

    while (!sortedNodes.empty()) {

        GenericTree<string>::TreeNode* currNode = sortedNodes.front();
        sortedNodes.pop();

        if (currNode->data == right.tree.getRootPtr()->data) { continue; }

        if (newHierarchy->tree.returnNode(currNode->data) == nullptr) {
            newHierarchy->tree.returnNode(currNode->parentPtr->data)->addChild(currNode->data); 
        }

        else {

            GenericTree<string>::TreeNode* rightParentNode = currNode->parentPtr;
            GenericTree<string>::TreeNode* matchingNode = newHierarchy->tree.returnNode(currNode->data);

            while (rightParentNode){
                if (newHierarchy->tree.find(matchingNode, rightParentNode->data)) {
                    throw std::invalid_argument("invalid parent-child relation");
                }
                rightParentNode = rightParentNode->parentPtr;
            }

            GenericTree<string>::TreeNode* leftParentNode = matchingNode->parentPtr;
            while (leftParentNode){
                if (newHierarchy->tree.find(currNode, leftParentNode->data)) {
                    throw std::invalid_argument("invalid parent-child relation");
                }
                leftParentNode = leftParentNode->parentPtr;
            }
          
            GenericTree<string>::TreeNode* parentThisTree = newHierarchy->tree.returnNode(currNode->parentPtr->data);
            GenericTree<string>::TreeNode* parentRightTree = newHierarchy->tree.returnNode(currNode->parentPtr->data);

            int levelParentThis = newHierarchy->tree.getNodeLevel(this->tree.returnNode(currNode->data)) + 1;
            int levelParentRight = right.tree.getNodeLevel(currNode) + 1;

            if (levelParentThis < levelParentRight) {
                continue;
            }
            else if (levelParentThis > levelParentRight) {
                newHierarchy->tree.moveNode(currNode->data, parentRightTree->data);
            }
            else {
                if (parentThisTree->data < parentRightTree->data) {
                    continue;
                }
                else {
                    newHierarchy->tree.moveNode(currNode->data, parentRightTree->data);
                }
            }
        }
    }
    return *newHierarchy;
}
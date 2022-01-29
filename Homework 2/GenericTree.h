#pragma once

#include<iostream>
#include<stdexcept>
#include<stack>
#include<queue>
#include<vector>
#include <ostream>

template <typename T>
class GenericTree {

public:
	class TreeNode {
	public:
		TreeNode* parentPtr;
		T data;
		std::vector< TreeNode* > childrenPtrs;


		TreeNode() : parentPtr(nullptr) {}
		TreeNode(const T& dataArg) : parentPtr(nullptr), data(dataArg) {}
		TreeNode(const TreeNode& other) = delete;
		TreeNode& operator=(const TreeNode& other) = delete;
		~TreeNode();
		TreeNode* addChild(const T& childData);
	};

private:
	TreeNode* rootNodePtr;

public:
	GenericTree() : rootNodePtr(nullptr) {}
	GenericTree(const T& rootData) : GenericTree() { createRoot(rootData); }
	GenericTree(const GenericTree& other) = delete;
	GenericTree& operator=(const GenericTree& other) = delete;
	~GenericTree() {
		delete this->rootNodePtr;
	}

	TreeNode* createRoot(const T& rootData);
	TreeNode* getRootPtr()const { return rootNodePtr; }

	int sizeOfTree()const;
	int sizeOfSubsTree(TreeNode* subsTreeRoot)const;
	int sizeOfChildren(const T& element)const;
	int heightOfTree(const TreeNode* root)const;
	bool find(TreeNode* node, const T& element)const;
	TreeNode* returnNode(const T& element)const;
	TreeNode* returnParentPtr(const T& element)const;
	void removeNode(const T& element)const;
	bool moveNode(const T& element, const T& parent)const;
	int getNodeLevel(TreeNode* node)const;
	void setRootPtr(TreeNode* rootNodePtr);
	void addChild(const T& parent, const T& element);
};

template <typename T>
typename GenericTree<T>::TreeNode* GenericTree<T>::createRoot(const T& rootData) {
	if (rootNodePtr != nullptr) {
		throw std::runtime_error("Attempted to create a new root when an existing root is already present");
	}
	this->rootNodePtr = new TreeNode(rootData);

	return rootNodePtr;
}

template <typename T>
GenericTree<T>::TreeNode::~TreeNode() {
	for (int i = 0; i < childrenPtrs.size(); i++) {
		delete childrenPtrs[i];
	}
}

template <typename T>
typename GenericTree<T>::TreeNode* GenericTree<T>::TreeNode::addChild(const T& childData) {

	TreeNode* newChildPtr = new TreeNode(childData);
	newChildPtr->parentPtr = this;

	childrenPtrs.push_back(newChildPtr);

	return newChildPtr;
}

template <typename T>
int GenericTree<T>::sizeOfTree()const {

	if (rootNodePtr == nullptr) { return 0; }

	int countNodes = 0;

	std::stack<TreeNode*> nodesToExplore;

	nodesToExplore.push(rootNodePtr);

	while (!nodesToExplore.empty()) {
		TreeNode* frontNode = nodesToExplore.top();
		nodesToExplore.pop();

		for (auto childPtr : frontNode->childrenPtrs) {
			nodesToExplore.push(childPtr);
			countNodes++;
		}
	}
	return countNodes + 1;
}

template<typename T>
int GenericTree<T>::sizeOfSubsTree(TreeNode* subsTreeRoot)const {

	if (rootNodePtr == nullptr) { return 0; }

	int countNodes = 0;

	std::stack<TreeNode*> nodesToExplore;

	nodesToExplore.push(subsTreeRoot);

	while (!nodesToExplore.empty()) {
		TreeNode* frontNode = nodesToExplore.top();
		nodesToExplore.pop();

		for (auto childPtr : frontNode->childrenPtrs) {
			nodesToExplore.push(childPtr);
			countNodes++;
		}
	}
	return countNodes + 1;
}

template<typename T>
int GenericTree<T>::heightOfTree(const TreeNode* root)const {

	int max = 0;

	if (root == nullptr) { return 0; }
	for (auto childPtr : root->childrenPtrs) {
		int height = heightOfTree(childPtr);
		if (height > max) {

			max = height;
		}
	}
	return max + 1;
}

template <typename T>
bool GenericTree<T>::find(TreeNode* node, const T& element)const {

	std::queue<TreeNode*> nodesToExplore;
	nodesToExplore.push(node);

	while (!nodesToExplore.empty()) {

		TreeNode* frontNode = nodesToExplore.front();
		nodesToExplore.pop();

		if (frontNode->data == element) {
			return true;
		}

		for (auto childPtr : frontNode->childrenPtrs) {
			if (childPtr) {

				nodesToExplore.push(childPtr);
			}
		}
	}
	return false;
}

template<typename T>
typename GenericTree<T>::TreeNode* GenericTree<T>::returnNode(const T& element)const {

	if (!rootNodePtr) return nullptr;
	
	if (rootNodePtr->data == element) {
		return rootNodePtr;
	}

	std::queue<TreeNode*> nodesToExplore;

	nodesToExplore.push(rootNodePtr);

	while (!nodesToExplore.empty()) {

		TreeNode* frontNode = nodesToExplore.front();
		nodesToExplore.pop();

		if (frontNode->data == element) {
			return frontNode;
		}

		for (auto childPtr : frontNode->childrenPtrs) {
			if (childPtr) {

				nodesToExplore.push(childPtr);
			}
		}
	}
	return nullptr;
}

template <typename T>
void GenericTree<T>::addChild(const T& parent, const T& element) {

	TreeNode* currParent = returnNode(parent);

	if (currParent == nullptr) {
		throw std::invalid_argument("invalid data format: missing parent");
	}

	if (this->returnNode(element) != nullptr) {
		throw std::invalid_argument("invalid data format: element already exists");
	}

	currParent->addChild(element);
}

template <typename T>
int GenericTree<T>::sizeOfChildren(const T& element)const {

	TreeNode* currNode = returnNode(element);

	return currNode != nullptr ? currNode->childrenPtrs.size(): -1;
}

template <typename T>
typename GenericTree<T>::TreeNode* GenericTree<T>::returnParentPtr(const T& element)const {
	TreeNode* parent = returnNode(element)->parentPtr;

	return parent;
}

template <typename T>
void GenericTree<T>::removeNode(const T& element)const {

	TreeNode* nodeToDelete = returnNode(element);
	if (nodeToDelete == nullptr) { return; }

	std::vector<TreeNode*> newChildren;

	for (auto brother : nodeToDelete->parentPtr->childrenPtrs) {
		if (brother == nodeToDelete) {
			for (auto child : nodeToDelete->childrenPtrs) {
				child->parentPtr = nodeToDelete->parentPtr;
				newChildren.push_back(child);
			}
		}
		else {
			newChildren.push_back(brother);
		}
	}
	nodeToDelete->parentPtr->childrenPtrs = newChildren;
	nodeToDelete->childrenPtrs.clear();
	delete nodeToDelete;
}


template <typename T>
bool GenericTree<T>::moveNode(const T& element, const T& parent)const {
	TreeNode* nodeToMove = returnNode(element);
	if (nodeToMove->parentPtr->data == parent) { return true; }

	TreeNode* newParent = returnNode(parent);

	newParent->childrenPtrs.push_back(nodeToMove);

	std::vector<TreeNode*> newChildPtr;
	for (auto child : nodeToMove->parentPtr->childrenPtrs) {
		if (child == nodeToMove) { continue; }
		newChildPtr.push_back(child);
	}
	nodeToMove->parentPtr->childrenPtrs = newChildPtr;
	nodeToMove->parentPtr = newParent;


	return true;
}

template <typename T>
int GenericTree<T>::getNodeLevel(TreeNode* node)const {

	TreeNode* walkBack = node;
	int level = 0;

	while (walkBack->parentPtr) {
		walkBack = walkBack->parentPtr;
		level++;
	}
	return level;
}

template <typename T>
void GenericTree<T>::setRootPtr(TreeNode* rootNodePtr) {
	this->rootNodePtr = rootNodePtr;
}


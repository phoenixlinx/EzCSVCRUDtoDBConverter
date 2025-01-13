
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef BINARY_SEARCH_TREE_HPP
#define BINARY_SEARCH_TREE_HPP

#include <iostream>
#include <string>
#include <deque>
#include <cmath>
#include "../include/CSVparser.hpp"
#include "../include/StringConverter.hpp"
#include<CSVAnalyzer.hpp>
#include<CSVrow.hpp>

using namespace std;



template <typename T, typename KeyExtractor>
class BinarySearchTree {

	using Key = decltype(std::declval<KeyExtractor>()(std::declval<T&>()));
private:
	size_t maxParentLookUp;
	size_t currentParentLookUp;

protected:
	struct Node {

	
		unsigned int longestChildPath;
		Node* parentNodePtr;
		Node* leftNodePtr;
		Node* rightNodePtr;
		shared_ptr<T> containedValueObject; // The object/value held within this tree node.
		Node();
		Node(shared_ptr<T> containedValueObject);
	
		
	};

	Node* root;
	size_t bstNodeCount; // Total nodes in BST
	size_t treeHeight;// Height of BST
	KeyExtractor keyExtractor;  // Function to extract BST key value from containedValueObject
	//Incline function to reduce overhead since one call to insert can result to many calls to this function
	inline void updateTreeMetrics(size_t nodeInsertionHeight);
	void debugKeyType() const;
	inline bool fixLeftImbalance(Node* subTreeRootNode);
	inline bool fixRightImbalance(Node* subTreeRootNode);
	void printNodeDetails(Node* node);
	void reSetDeepestLevel();
	//Previous Nodes in height
	inline unsigned int calculateMaxNodesInHight(unsigned int previousNodesInHeight);
public:
	static constexpr  unsigned int initialNodeLevel = 1;
	BinarySearchTree(KeyExtractor extractor);
	virtual ~BinarySearchTree();
	template <typename InputKey>
	void remove(const InputKey& searchKey,const string& csvPathDeletedNodes);
	void insert(shared_ptr<T> containedValueObject);

	template<typename InputKey>
	vector<shared_ptr<T>> search(const InputKey& searchKey);
	size_t getDeepestLevel() const;
	size_t getBSTSize() const;
	inline bool isTreeEmpty() const;
	void printInOrder();
	void printLevelOrder();
};
#include "../src/BinarySearchTree.tpp"

#endif 


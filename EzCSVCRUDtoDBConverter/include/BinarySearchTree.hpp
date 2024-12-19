
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef BINARY_SEARCH_TREE_HPP
#define BINARY_SEARCH_TREE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include "../include/CSVparser.hpp"
#include "../include/StringConverter.hpp"
#include<CSVAnalyzer.hpp>
#include<CSVrow.hpp>
using namespace std;



template <typename T, typename KeyExtractor>
class BinarySearchTree {

	using Key = decltype(std::declval<KeyExtractor>()(std::declval<T&>()));

private:
	struct Node {

	
		unsigned int longestChildPath;
	
		Node* leftNodePtr;
		Node* rightNodePtr;
		shared_ptr<T> containedValueObject; // The object/value held within this tree node.
		Node();
		Node(shared_ptr<T> containedValueObject);
	
		
	};

	Node* root;
	unsigned int bstNodeCount; // Total nodes in BST
	unsigned int treeHeight;// Height of BST
	KeyExtractor keyExtractor;  // Function to extract BST key value from containedValueObject
	vector<Node**> insertionPath;//InsertionPath is a reusable member variable to avoid executive memory management during the insertion of many rows.
	//Incline function to reduce overhead since one call to insert can result to many calls to this function
	inline void updateTreeMetrics(size_t nodeInsertionHeight);
	void debugKeyType() const;
	inline void fixLeftImbalance(Node*& subTreeRootNode);
	inline void fixRightImbalance(Node*& subTreeRootNode);
public:
	static constexpr  unsigned int NodeHeightIncrement = 1;
	BinarySearchTree();
	BinarySearchTree(KeyExtractor extractor);
	virtual ~BinarySearchTree();
	template <typename InputKey>
	void remove(const InputKey& searchKey,const string& csvPathDeletedNodes);
	void insert(shared_ptr<T> containedValueObject);

	template<typename InputKey>
	vector<shared_ptr<T>> search(const InputKey& searchKey);

	unsigned int getDeepestLevel() const;
	void reSetDeepestLevel();
	unsigned int getBSTSize() const;
	bool isEmpty() const;
	void printInOrder();
	
};
#include "../src/BinarySearchTree.tpp"

#endif 


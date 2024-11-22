
// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include <iostream>
# include <string>
# include <vector>
#include "../include/CSVparser.hpp"
#include "../include/StringConverter.hpp"
using namespace std;


struct Bid {
	// BidId is the unique identifier for bid,
	// and is used as the key in the Binary Search Tree.
	string bidId; // Unique identifier for the bid
	string title;
	string fund;
	// RowPos is the Index of the bid's row in the CSV file.
	int rowPos; // Row index of the bid in the CSV file
	double amount;
	
	Bid();
};

struct Node {

	Bid bid;

	Node* leftNodePtr;

	Node* rightNodePtr;
	Node();
	Node(Bid bid);
};




class BinarySearchTree {



private:

	Node* root;
	unsigned int bstNodeCount; // Total nodes in BST
	unsigned int treeHeight;// Height of BST
	//Incline function to reduce overhead since one call to insert can result to many calls to this function
	inline void updateTreeMetrics(unsigned int nodeInsertionHeight);
public:
	static constexpr  unsigned int NodeHeightIncrement = 1;
	BinarySearchTree();

	void remove(string bidId, string csvPath, string csvPathDeletedBids);
	Node* getRoot();
	virtual ~BinarySearchTree();;
	void insert(Bid bid);
	unsigned int getDeepestLevel() const;
	void backUpDeletedNode(Node* soonToBeDeletedNode, const string& csvPathDeletedBids);
	void reSetDeepestLevel();
	unsigned int getBSTSize() const;
	Bid search(Node* node, string bidId);
	bool isEmpty() const;
	void printInOrder();
	Bid getBid(string csvPath);
	bool loadBids(string csvPath);
};

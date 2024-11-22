
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
public:
	static constexpr  unsigned int NodeHeightIncrement = 1;
	BinarySearchTree();

	void remove(string bidId, string csvPath, string csvPathDeletedBids);
	Node* getRoot();
	void createTreeNode(Bid bid);
	virtual ~BinarySearchTree();
	void  printInOrderFromRoot();
	void insert(Node* node, Bid bid, unsigned int nodeInsertionHeight);
	unsigned int getDeepestLevel() const;
	void reSetDeepestLevel();
	unsigned int getBSTSize() const;
	//	void recoverRemovedBid();
	vector<Node> recoverDeletedBids(Node* node, string csvPathDeletedBids);
	Bid search(Node* node, string bidId);
	bool isEmpty() const;
	void printInOrder(Node* node);
	Bid getBid(string csvPath);
	bool loadBids(string csvPath, BinarySearchTree* binarySearchTree);
};

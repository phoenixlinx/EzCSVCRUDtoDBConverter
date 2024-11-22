// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "../include/BinarySearchTree.hpp"



 // Initializing only non-default members
Bid::Bid() : rowPos(0), amount(0.0) {
   
}


Node::Node() : leftNodePtr(nullptr), rightNodePtr(nullptr) {

}

Node::Node(Bid bid) : bid(bid),leftNodePtr(nullptr), rightNodePtr(nullptr) {


}

Node* BinarySearchTree::getRoot() {
	return root;
}
BinarySearchTree::BinarySearchTree() {

	

	root = 0;
	treeHeight = 0;
	bstNodeCount = 0;
}

bool BinarySearchTree::isEmpty() const { 
	return root == 0; 
}



BinarySearchTree::~BinarySearchTree() {



}


unsigned int BinarySearchTree::getDeepestLevel() const {
	return treeHeight;
}

unsigned int BinarySearchTree::getBSTSize() const {
	return bstNodeCount;
}
void BinarySearchTree::reSetDeepestLevel() {
	treeHeight = 0;
}

vector<Node> BinarySearchTree::recoverDeletedBids(Node* node, string csvPathDeletedBids)

{

	Bid bid;
	cout << endl << endl;// The the bid will be sent to addBid and added to a
	csv::Parser fileBackUp = csv::Parser(csvPathDeletedBids); 
	unsigned int fileRowCount;
	fileRowCount = fileBackUp.rowCount();
	cout << fileRowCount;

	vector<Node> deletedBids; 

	std::vector<std::string> row;

	string bidds;


	bid.bidId = node->bid.bidId;
	bid.title = node->bid.title;
	bid.amount = node->bid.amount;
	bid.fund = node->bid.fund;

	string a;
	string b;
	double c;
	string d;
	a = node->bid.bidId;
	b = node->bid.title;
	c = node->bid.amount;

	string s = to_string(c); // this will convert the c of type double into a string

	d = node->bid.fund;

	row.push_back(b);
	row.push_back(a);
	row.push_back("0");
	row.push_back("0");
	row.push_back(s);
	row.push_back("0");
	row.push_back("0");
	row.push_back("0");
	row.push_back(d);
	fileBackUp.deleteRow(fileRowCount);
	fileBackUp.sync();
	fileBackUp.addRow(fileRowCount, row);
	fileBackUp.sync();
	cout << endl << endl;






	cout << endl << endl;

	//deletedBids.push_back(node); // This will add a deleted Bid to the end of the deltedBids vector.
	for (unsigned i = 0; i < row.size(); ++i) {




		bidds = row.at(i);
		cout << bidds << endl;

	}

	return deletedBids;



}

Bid BinarySearchTree::search(Node* node, string bidId) {
	Bid bid;
	Node* currNodePosition = root;
	while (currNodePosition != 0) {

		if (bidId == currNodePosition->bid.bidId) {
			cout << currNodePosition->bid.bidId << " | ";
			cout << " Amount: $" << currNodePosition->bid.amount << " | ";
			cout << currNodePosition->bid.title << " | ";
			cout << " Row: " << currNodePosition->bid.rowPos << " | ";

			cout << currNodePosition->bid.fund << endl;

			//
			if ((currNodePosition->rightNodePtr != 0 && currNodePosition->rightNodePtr->bid.bidId == bidId)) { // This will check if the right child has also a value of bidId
				currNodePosition = currNodePosition->rightNodePtr;   // This will change currNodePosition to its right child
			}
			else if (currNodePosition->rightNodePtr != 0 && (currNodePosition->rightNodePtr)->leftNodePtr != 0) { // this will first check if the curr node has a right child if it does then it will check if that right node has a child
				currNodePosition = (currNodePosition->rightNodePtr)->leftNodePtr; // This will set curr node as the left child of its curent right child. This will serch for duplicate values
			}
			else {
				return currNodePosition->bid; // This will end the search function if currNodePosition does not have a right child or if the right child does not have a value of bidId

			}

		}
		if (bidId > currNodePosition->bid.bidId) {                           // this will search the right branch of a node if string bidId is more than the curr position bidId

			currNodePosition = currNodePosition->rightNodePtr;


		}

		else if (bidId < currNodePosition->bid.bidId)
			currNodePosition = currNodePosition->leftNodePtr;

	}

	return bid;





}




void BinarySearchTree::remove(string bidId, string csvPath, string csvPathDeletedBids) {                  // credit:http://www.cplusplus.com/forum/general/1551/
	BinarySearchTree* mangoCat;
	mangoCat = new BinarySearchTree();
	csv::Parser file = csv::Parser(csvPath);
	int deleteRowPosition = 0;

	Node* tmp3;
	Node* tmp2;
	Node* tmp1;
	Node* currNodePosition;
	Node* par = 0;
	currNodePosition = root;
	bool nodeBidIdExists = false;
	while (currNodePosition != 0)
	{
		if (currNodePosition->bid.bidId == bidId)
		{
			nodeBidIdExists = true;
			break;
		}
		else
		{
			par = currNodePosition;
			if (bidId > currNodePosition->bid.bidId)
				currNodePosition = currNodePosition->rightNodePtr;
			else currNodePosition = currNodePosition->leftNodePtr;
		}
	}
	if (!nodeBidIdExists) 
	{

		return;
	}


	if ((currNodePosition->leftNodePtr == 0 && currNodePosition->rightNodePtr != 0) || (currNodePosition->leftNodePtr != 0
		&& currNodePosition->rightNodePtr == 0))
	{
		if (currNodePosition->leftNodePtr == 0 && currNodePosition->rightNodePtr != 0)
		{
			if (par->leftNodePtr == currNodePosition)
			{
				par->leftNodePtr = currNodePosition->rightNodePtr;

				deleteRowPosition = currNodePosition->bid.rowPos; // This will assign the rowPos
				cout << "Row Position: " << deleteRowPosition << endl;
				//of current node position to variable deleteRowPosition
				mangoCat->recoverDeletedBids(currNodePosition, csvPathDeletedBids);
				file.deleteRow(deleteRowPosition);
				file.sync();

				delete currNodePosition; // This will delete a parent node with one right child and
				bstNodeCount--;
				// put that child in the parents place
			}
			else
			{

				par->rightNodePtr = currNodePosition->rightNodePtr;

				deleteRowPosition = currNodePosition->bid.rowPos; // This will assign the rowPos
				cout << "Row Position: " << deleteRowPosition << endl;
				//of current node position to variable deleteRowPosition
				mangoCat->recoverDeletedBids(currNodePosition, csvPathDeletedBids);
				file.deleteRow(deleteRowPosition);
				file.sync();
				delete currNodePosition;
				bstNodeCount--;
			}
		}
		else
		{
			if (par->leftNodePtr == currNodePosition)
			{
				par->leftNodePtr = currNodePosition->leftNodePtr;

				deleteRowPosition = currNodePosition->bid.rowPos; // This will assign the rowPos
				cout << "Row Position: " << deleteRowPosition << endl;
				//of current node position to variable deleteRowPosition
				mangoCat->recoverDeletedBids(currNodePosition, csvPathDeletedBids);
				file.deleteRow(deleteRowPosition);
				file.sync();
				delete currNodePosition;
				bstNodeCount--;
			}
			else
			{
				par->rightNodePtr = currNodePosition->leftNodePtr;

				deleteRowPosition = par->bid.rowPos; // This will assign the rowPos
				cout << "Row Position: " << deleteRowPosition << endl;
				//of current node position to variable deleteRowPosition
				mangoCat->recoverDeletedBids(currNodePosition, csvPathDeletedBids);
				file.deleteRow(deleteRowPosition);
				file.sync();
				delete currNodePosition;
				bstNodeCount--;
			}
		}

		return;
	}


	if (currNodePosition->leftNodePtr == 0 && currNodePosition->rightNodePtr == 0)
	{
		if (par->leftNodePtr == currNodePosition)
			par->leftNodePtr = 0;               // this will delete par's copy of currNodePositions pointers.
		else par->rightNodePtr = 0;

		deleteRowPosition = currNodePosition->bid.rowPos; // This will assign the rowPos
		cout << "Row Position: " << deleteRowPosition << endl;
		//of current node position to variable deleteRowPosition
		mangoCat->recoverDeletedBids(currNodePosition, csvPathDeletedBids);
		file.deleteRow(deleteRowPosition);
		file.sync();
		delete currNodePosition;   // This will delete a leaf node
		bstNodeCount--;
		return;
	}



	if (currNodePosition->leftNodePtr != 0 && currNodePosition->rightNodePtr != 0)
	{                                                                                                // this will delete a parent with two children

		tmp1 = currNodePosition->rightNodePtr;
		if ((tmp1->leftNodePtr == 0) && (tmp1->rightNodePtr == 0))
		{
			currNodePosition = tmp1;

			deleteRowPosition = tmp1->bid.rowPos; // This will assign the rowPos
			//of current node position to variable deleteRowPosition
			cout << "Row Position: " << deleteRowPosition << endl;
			mangoCat->recoverDeletedBids(tmp1, csvPathDeletedBids);
			file.deleteRow(deleteRowPosition);
			file.sync();
			delete tmp1;
			bstNodeCount--;
			currNodePosition->rightNodePtr = 0;
		}
		else
		{



			if ((currNodePosition->rightNodePtr)->leftNodePtr != 0)
			{

				tmp2 = currNodePosition->rightNodePtr;
				tmp3 = (currNodePosition->rightNodePtr)->leftNodePtr;
				while (tmp3->leftNodePtr != 0)
				{
					tmp2 = tmp3;
					tmp3 = tmp3->leftNodePtr;
				}
				currNodePosition->bid.bidId = tmp3->bid.bidId;

				deleteRowPosition = tmp3->bid.rowPos; // This will assign the rowPos
				cout << "Row Position: " << deleteRowPosition << endl;
				//of current node position to variable deleteRowPosition
				mangoCat->recoverDeletedBids(tmp3, csvPathDeletedBids);
				file.deleteRow(deleteRowPosition);
				file.sync();
				delete tmp3;
				bstNodeCount--;
				tmp2->leftNodePtr = 0;
			}
			else                // this will delete a parent an take a child that as its replacement
			{
				Node* tmp;
				tmp = currNodePosition->rightNodePtr;
				currNodePosition->bid.bidId = tmp->bid.bidId;
				currNodePosition->rightNodePtr = tmp->rightNodePtr;

				deleteRowPosition = tmp->bid.rowPos; // This will assign the rowPos
				cout << "Row Position: " << deleteRowPosition << endl;
				mangoCat->recoverDeletedBids(tmp, csvPathDeletedBids);
				//of current node position to variable deleteRowPosition
				file.deleteRow(deleteRowPosition);
				file.sync();
				delete tmp;
				bstNodeCount--;
			}

		}
		return;
	}

}




void BinarySearchTree::createTreeNode(Bid bid) {
	if (root == nullptr) {
		root = new Node(bid);
		cout << "Inserted at level 0 (root)" << endl;
		
	}
	else {
		insert(root, bid, 0); // Start at level 0 for the root
	}
}

void BinarySearchTree::insert(Node* node, Bid bid, unsigned int nodeInsertionHeight) {
	if (bid.bidId < node->bid.bidId) {
		if (node->leftNodePtr == nullptr) {
			node->leftNodePtr = new Node(bid);
			bstNodeCount++;
			cout << "Inserted at level " << nodeInsertionHeight + NodeHeightIncrement << endl;
			if (nodeInsertionHeight + NodeHeightIncrement > treeHeight) {
				treeHeight = nodeInsertionHeight + NodeHeightIncrement;
			}
		}
		else {
			insert(node->leftNodePtr, bid, nodeInsertionHeight + NodeHeightIncrement);
		}
	}
	else {
		if (node->rightNodePtr == nullptr) {
			node->rightNodePtr = new Node(bid);
			bstNodeCount++;
			cout << "Inserted at level " << nodeInsertionHeight + NodeHeightIncrement << endl;
			if (nodeInsertionHeight + NodeHeightIncrement > treeHeight) {
				treeHeight = nodeInsertionHeight + NodeHeightIncrement;
			}
		}
		else {
			insert(node->rightNodePtr, bid, nodeInsertionHeight + NodeHeightIncrement);
		}
	}
}





void BinarySearchTree::printInOrder(Node* node) {
	// This method first prints the left branch of the tree.
	// It reaches the bottom of the left branch, then starts printing all values of the left branches as it ascends toward the root.
	// For the right branches of the root's left branch, this method immediately prints the values of nodes that belong to the right branches as it ascends toward the root after reaching the bottom of a left branch.
	// Once finished with the left branch of the root, it repeats the process for the root's right branch.


	if (node != 0)
	{
		if (node->leftNodePtr)

			printInOrder(node->leftNodePtr);

		cout << node->bid.bidId << " | ";
		cout << " Amount: $" << node->bid.amount << " | ";
		cout << " Row: " << node->bid.rowPos << " | ";
		cout << node->bid.title << " | ";
		cout << node->bid.fund << endl;

		if (node->rightNodePtr)

			printInOrder(node->rightNodePtr);
	}
	else return;

}

void BinarySearchTree::printInOrderFromRoot()
{
	printInOrder(root);
}

Bid BinarySearchTree::getBid(string csvPath) {// This method will obtain user input and return it as a bid



	Bid bid;

	cout << endl << endl;// The the bid will be sent to addBid and added to a
	BinarySearchTree* cat;
	cat = new BinarySearchTree();
	csv::Parser file = csv::Parser(csvPath);
	unsigned int fileRowCount;
	fileRowCount = file.rowCount();
	cout << fileRowCount;
	std::vector<std::string> myrow;
	optional <double> amount;
	


		cout << endl << endl;
		string a;
		string b;
		string d;

		cout << "Enter bidId: ";
		getline(cin, a);                               // This method creates a new bid and sends it to the createTreeNode method for insertion into the binary search tree.


		cout << "Enter Title: ";
		getline(cin, b);

		string strAmount;
		cout << "Enter amount: ";
		getline(cin, strAmount);
		if (amount = StringConverter::toDouble(strAmount); amount.has_value()) {
			bid.amount = amount.value();
		}
		cout << "Enter Fund: ";
		getline(cin, d);
		bid.bidId = a;
		bid.title = b;
		bid.fund = d;
		bid.rowPos = fileRowCount;
		cat->createTreeNode(bid);


		myrow.push_back(b);
		myrow.push_back(a);
		myrow.push_back("3");
		myrow.push_back("4");
		myrow.push_back(strAmount);
		myrow.push_back("6");
		myrow.push_back("7");
		myrow.push_back("8");
		myrow.push_back(strAmount);

		file.addRow(fileRowCount, myrow);
		file.sync();
		cout << endl << endl;






		cout << endl << endl;



	
	return bid;


}


bool BinarySearchTree::loadBids(string csvPath, BinarySearchTree* binarySearchTree) {
	cout << "Loading CSV file " << csvPath << endl;
	binarySearchTree->reSetDeepestLevel();
	
	try {
	
	// initialize the CSV Parser using the given path
	csv::Parser file = csv::Parser(csvPath);
	// read and display header row - optional
	optional <double> amount;
	vector<string> header = file.getHeader();
	for (auto const& c : header) {
		cout << c << " | ";
	}
	cout << "" << endl;

	
		// loop to read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {
			// Create a data structure and add to the collection of bids
			Bid bid;
			bid.bidId = file[i][1];
			bid.title = file[i][0];
			bid.fund = file[i][8];
			bid.rowPos = i;
			if (bid.amount = StringConverter::toDouble(file[i][4]).value(); amount.has_value()) {

			}



			binarySearchTree->createTreeNode(bid); // This will initiate a binary tree by adding a bid to the root if one does not already exist

		}
	}
	catch (const csv::Error& e) {
		string message = e.what();

		if (message.find("No data") != string::npos) {
			cerr << "The file is empty. Please provide a file with valid data." << endl;

		}
		else if (message.find("Failed to open") != string::npos) {
			cerr << "Unable to open the file. Check the file path or permissions." << endl;

		}
		else {
			cerr << "An unknown error occurred: " << message << endl;
		}
		return false;
	}
	cout << "Bids read: " << binarySearchTree->getBSTSize() << endl;
	cout << "Deepest level: " << binarySearchTree->getDeepestLevel() << endl;
	return true;
}

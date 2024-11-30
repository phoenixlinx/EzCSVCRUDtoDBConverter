// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "../include/BinarySearchTree.hpp"



 // Initializing only non-default members
Bid::Bid() : rowPos(0), amount(0.0) {
   
}


Node::Node() : leftNodePtr(nullptr), rightNodePtr(nullptr), longestChildPath(1)  {

}

Node::Node(Bid bid) : bid(bid),leftNodePtr(nullptr), rightNodePtr(nullptr),longestChildPath(1) {


}

Node* BinarySearchTree::getRoot() {
	return root;
}
BinarySearchTree::BinarySearchTree() 

	: root(nullptr), treeHeight(0), bstNodeCount(0) {
	insertionPath.reserve(32); // Reserve memory to avoid costly vector resizing during insertion


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

void BinarySearchTree::backUpDeletedNode(Node* soonToBeDeletedNode, const string& csvPathDeletedBids) {
	if (soonToBeDeletedNode == nullptr) {
		cerr << "Error: Null node passed to recoverDeletedBids." << endl;
		return; // Exit if the node is null
	}

	try {
		// Parse the CSV file for writing
		csv::Parser fileBackUp(csvPathDeletedBids);

		// Create a row for the given node's bid
		vector<string> row = {
			soonToBeDeletedNode->bid.title,
			soonToBeDeletedNode->bid.bidId,
			"0", "0", // Placeholder values
			to_string(soonToBeDeletedNode->bid.amount),
			"0", "0", "0", // Placeholder values
			soonToBeDeletedNode->bid.fund
		};

		// Add the row to the CSV file
		fileBackUp.addRow(fileBackUp.rowCount(), row);

		// Synchronize the file
		fileBackUp.sync();

		cout << "Bid with ID " << soonToBeDeletedNode->bid.bidId << " successfully backed up to file." << endl;

	}
	catch (const std::exception& e) {
		cerr << "Error processing CSV: " << e.what() << endl;
	}
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
	
	csv::Parser file = csv::Parser(csvPath);
	unsigned int deleteRowPosition = 0;

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
				this->backUpDeletedNode(currNodePosition, csvPathDeletedBids);
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
				this->backUpDeletedNode(currNodePosition, csvPathDeletedBids);
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
				this->backUpDeletedNode(currNodePosition, csvPathDeletedBids);
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
				this->backUpDeletedNode(currNodePosition, csvPathDeletedBids);
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
		this->backUpDeletedNode(currNodePosition, csvPathDeletedBids);
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
			this->backUpDeletedNode(tmp1, csvPathDeletedBids);
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
				this->backUpDeletedNode(tmp3, csvPathDeletedBids);
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
				this->backUpDeletedNode(tmp, csvPathDeletedBids);
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

inline void BinarySearchTree::updateTreeMetrics(unsigned int nodeInsertionHeight) {
	nodeInsertionHeight = nodeInsertionHeight + NodeHeightIncrement;
	cout << "Inserted at level " << nodeInsertionHeight << endl;
	if (nodeInsertionHeight  > treeHeight) {
		treeHeight = nodeInsertionHeight;
	}

}


void BinarySearchTree::insert(Bid bid) {
	Node** currentNode = &root;
	unsigned int nodeInsertionHeight = 0;
	unsigned int elementsRemaining = 0; // Tracks remaining nodes for re balancing
	insertionPath.clear(); // Clears path for rebalancing
	while (*currentNode != nullptr) {
		insertionPath.push_back(currentNode);
		if (bid.bidId > (*currentNode)->bid.bidId) {
			    //Set current node to its left child because a deeper traversal is needed to find a null node for insertion.
				currentNode = &((*currentNode)->rightNodePtr);
			
			
			
		}else if((bid.bidId < (*currentNode)->bid.bidId)) {


			
				//Set current node to its left child because a deeper traversal is needed to find a null node for insertion.
				currentNode = &((*currentNode)->leftNodePtr);
			

		}else {
			//Duplicate key so do not insert.
			return;
		}




	}
	*currentNode = new Node(bid);
	bstNodeCount++;
	nodeInsertionHeight = insertionPath.size();
	elementsRemaining = nodeInsertionHeight;
	updateTreeMetrics(nodeInsertionHeight);
	while (elementsRemaining > 0) {
	

		currentNode = insertionPath.back();
		fixLeftImbalance(*currentNode);
		fixRightImbalance(*currentNode);
		insertionPath.pop_back();
		elementsRemaining--;    // Decrement remaining nodes to check for balancing
	}



}
 
void BinarySearchTree::printInOrder() {
	Node* currentNode = root;
	Node* previousNode = nullptr;//Used to find the rightmost node in the left subtree
	while (currentNode != nullptr) {
		// Check if not null because it is more likely
		if (currentNode->leftNodePtr != nullptr) {



			
			previousNode = currentNode->leftNodePtr;
			// Find the rightmost node in the left subtree
			while (previousNode->rightNodePtr != nullptr && previousNode->rightNodePtr != currentNode) {
				previousNode = previousNode->rightNodePtr;
			}

			//This condition is prioritized because after second while loop previousNode->rightNodePtr is likely null.
			if (previousNode->rightNodePtr == nullptr) {



				previousNode->rightNodePtr = currentNode;//If no temporary link exists, create one to revisit this node after processing the left subtree
				currentNode = currentNode->leftNodePtr;
				
			}
			else {

			

				// Left subtree has  been processed so remove link
				previousNode->rightNodePtr = nullptr;
				cout << currentNode->bid.bidId << " | " << currentNode->bid.amount << " | " << currentNode->bid.rowPos << " | " << currentNode->bid.title << " | " << currentNode->bid.fund << '\n';
				currentNode = currentNode->rightNodePtr;

				
			}
		
		}else {

	        //Print the left node of the right most child
			cout << currentNode->bid.bidId << " | " << currentNode->bid.amount << " | " << currentNode->bid.rowPos << " | " << currentNode->bid.title << " | " << currentNode->bid.fund << '\n';
			currentNode = currentNode->rightNodePtr;
			




		}
	}
// This implementation is based on the Morris Traversal algorithm.
// Reference: Morris, J. H. (1979). Traversing binary trees simply and cheaply.
// Communications of the ACM, 22(8), 235–239. See README.md for details.

}




Bid BinarySearchTree::getBid(string csvPath) {// This method will obtain user input and return it as a bid



	Bid bid;

	cout << endl << endl;
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
		this->insert(bid);


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


bool BinarySearchTree::loadBids(string csvPath) {
	cout << "Loading CSV file " << csvPath << endl;
	
	this->reSetDeepestLevel();
	
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



			this->insert(bid); 

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
	cout << "Bids read: " << this->getBSTSize() << endl;
	cout << "Deepest level: " << this->getDeepestLevel() << endl;
	return true;
}
void BinarySearchTree::fixLeftImbalance(Node*& subTreeRootNode) {

	Node* subTreeRootNodeLeftChild = nullptr;
	if (subTreeRootNode == nullptr || subTreeRootNode->leftNodePtr == nullptr) {
		return;
	}
	if (subTreeRootNode != nullptr && subTreeRootNode->leftNodePtr->longestChildPath == subTreeRootNode->longestChildPath) {

		
		subTreeRootNodeLeftChild = subTreeRootNode->leftNodePtr;
		subTreeRootNode->leftNodePtr = subTreeRootNodeLeftChild->rightNodePtr;
		subTreeRootNodeLeftChild->rightNodePtr = subTreeRootNode;
		subTreeRootNode = subTreeRootNodeLeftChild;
	}
	return;

	// This implementation is based on AA Trees by Anderson Arne 1993.See README.md for details.

}
void BinarySearchTree::fixRightImbalance(Node*& subTreeRootNode) {
	
	Node* subTreeRootNodeRightChild = nullptr; 

	//Exit early if there is no imbalance to fix. 
	if (subTreeRootNode == nullptr || subTreeRootNode->rightNodePtr == nullptr || subTreeRootNode->rightNodePtr->rightNodePtr == nullptr) {
		return;  
	}

	// Check if the current node's longest child path equals the level of its right-right grandchild.
	if (subTreeRootNode->longestChildPath == subTreeRootNode->rightNodePtr->rightNodePtr->longestChildPath) {
		

	
		 subTreeRootNodeRightChild = subTreeRootNode->rightNodePtr;

		// Update the current node's right pointer to point to the left child of the saved node.
		 subTreeRootNode->rightNodePtr = subTreeRootNodeRightChild->leftNodePtr;

		// Update the saved node's left pointer to point to the current node.
		 subTreeRootNodeRightChild->leftNodePtr = subTreeRootNode;

		
		subTreeRootNodeRightChild->longestChildPath++; 
		subTreeRootNode = subTreeRootNodeRightChild;
		
	
	}

	// Return the node unchanged if no balancing needed.
	return;
}

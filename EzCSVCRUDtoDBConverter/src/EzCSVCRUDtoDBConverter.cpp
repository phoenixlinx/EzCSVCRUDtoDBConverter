
//============================================================================
// Name        : EzCSVCRUDtoDBConverter.cpp
// Author      : Alfredo A. Martinez
// Version     : 1.6
//============================================================================
// EzCSVCRUDtoDBConverter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <time.h>
# include <string>
# include <vector>
# include <list>
#include "../include/CSVparser.hpp"

using namespace std;
// forward declarations

double strToDouble(string str, char ch);
string replaceSlash(string newPath);
int stringToInt;
int sizeBinarySearchTree = 0;
double strToDouble(string str, char ch);
int X;  // this var will keep track if a user has loadead a cvs file
bool HasLoadedFile = false; //
vector<string> attendeesList(0); // List of attendees
string attendeeName;             // Name of attendee to match
int matchPos = 0;                // Matched position in attendee list
string greetingText;
std::vector<std::string> recoverRemovedBids; // This vector will temporaly hold removed Bids

struct Bid {

	string bidId; // unique identifier

	string title;

	string fund;

	int rowPos; // This varible will hold a bids' row position  within a CVS file

	double amount;

	Bid() {

		amount = 0.0;
		rowPos = 0;

	}

};



struct Node {

	Bid bid;

	Node* leftNodePtr;

	Node* rightNodePtr;



	//default constructor, initializes node pointers to 0

	Node() {

		leftNodePtr = nullptr;

		rightNodePtr = nullptr;

	}



	//initializes with an input bid

	Node(Bid myBid) : Node() {

		this->bid = myBid;

	}

};



//============================================================================

// Binary Search Tree class definition

//============================================================================



class BinarySearchTree {



private:

	Node* root;
public:

	BinarySearchTree();

	void remove(string bidId, string csvPath, string csvPathDeletedBids);

	void createTreeNode(Bid bid);
	virtual ~BinarySearchTree();
	void  printInOrderFromRoot();
	void insert(Node* node, Bid bid);
	void loadBids(string csvPath, BinarySearchTree* binaryTreeObj);
	//	void recoverRemovedBid();
	vector<Node> recoverDeletedBids(Node* node, string csvPathDeletedBids);
	Bid search(Node* node, string bidId);
	bool isEmpty() const { return root == 0; }
	void displayBid(Bid bid);
	void printInOrder(Node* node);
	string replaceSlash(string newPath);
};



/**

 * Default constructor

 */

BinarySearchTree::BinarySearchTree() {

	// initialize housekeeping variables

	root = 0;

}



/**

 * Destructor

 */

BinarySearchTree::~BinarySearchTree() {



}




void BinarySearchTree::displayBid(Bid bid) {
	cout << bid.bidId << " | ";
	cout << " Amount: $" << bid.amount << " | ";
	cout << bid.title << " | ";
	cout << bid.fund << endl;

	return;

}



vector<Node> BinarySearchTree::recoverDeletedBids(Node* node, string csvPathDeletedBids)

{

	Bid bid;
	cout << endl << endl;// The the bid will be sent to addBid and added to a
	BinarySearchTree* AligatorCat;
	AligatorCat = new BinarySearchTree();
	csv::Parser fileBackUp = csv::Parser(csvPathDeletedBids); //Thus creates an instance of the class Parser
	string addBidKey2;
	int fileRowCount2;
	fileRowCount2 = fileBackUp.rowCount();
	//fileRowCount++;
	cout << fileRowCount2;

	vector<Node> deletedBids; // This is a vector of vids. The vector is called deletedBids

	std::vector<std::string> myrow2;

	string bidds;


	bid.bidId = node->bid.bidId;
	bid.title = node->bid.title;
	bid.amount = node->bid.amount;
	bid.fund = node->bid.fund;

	string a;
	string b;
	double c;
	string d;
	string dob;
	int row;



	a = node->bid.bidId;
	b = node->bid.title;
	c = node->bid.amount;

	string s = to_string(c); // this will convert the c of type double into a string

	d = node->bid.fund;

	myrow2.push_back(b);
	myrow2.push_back(a);
	myrow2.push_back("0");
	myrow2.push_back("0");
	myrow2.push_back(s);
	myrow2.push_back("0");
	myrow2.push_back("0");
	myrow2.push_back("0");
	myrow2.push_back(d);
	fileBackUp.deleteRow(fileRowCount2);
	fileBackUp.sync();
	fileBackUp.addRow(fileRowCount2, myrow2);
	fileBackUp.sync();
	cout << endl << endl;






	cout << endl << endl;

	//deletedBids.push_back(node); // This will add a deleted Bid to the end of the deltedBids vector.
	for (unsigned i = 0; i < myrow2.size(); ++i) {




		bidds = myrow2.at(i);
		cout << bidds << endl;

	}

	return deletedBids;



}

Bid BinarySearchTree::search(Node* node, string bidId) {
	//                            
	Bid bid;
	Node* currNodePosition = root;


	while (currNodePosition != 0) {

		if (bidId == currNodePosition->bid.bidId) {       // this will compare if the bib.bidid of the root node of the three matches the users input
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






/**

 * remove a bid

 */




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
	Node* tmp;
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
	if (!nodeBidIdExists) //This will stop the execution of the remove method if no node->bid.bidId mathche
		// user input string bidId
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
			}

		}
		return;
	}

}






void BinarySearchTree::createTreeNode(Bid bid) {

	if (root == 0) {

		root = new Node(bid);

		root->leftNodePtr == 0;
		root->rightNodePtr = 0;                                   // this will create a tree root and send any other requst to the method insert

	}
	else {

		insert(root, bid);

	}

}


void BinarySearchTree::insert(Node* node, Bid bid) {


	if (bid.bidId < node->bid.bidId) {                                   // this will insert a new node on an existing tree. The insertion location depends on the value of the bidId parameter

		if (node->leftNodePtr == nullptr) {
			node->leftNodePtr = new Node(bid);
		}
		else {
			insert(node->leftNodePtr, bid);
		}
	}

	else {
		if (node->rightNodePtr == nullptr) {
			node->rightNodePtr = new Node(bid);
		}
		else {
			insert(node->rightNodePtr, bid);

		}
	}

}



void BinarySearchTree::printInOrder(Node* node) {

	//This method will first print the left branch of the tree.
	//This method will reach the bothom of the left branch and then start printing all value of leeft branche of the left tree breanch as it ascends towards the root.
   // As for the right branches of the root left branch, this method will imedently print the value of nodes that belong to right branches if it is asending towrds the the root of a tree after recging the buttom of a left branch
	// After it is done with the left branch of the root it will do the same to the right branch of the root.
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
//============================================================================

// Static methods used for testing

//============================================================================


void loadBids(string csvPath, BinarySearchTree* binarySearchTree) {
	cout << "Loading CSV file " << csvPath << endl;

	// initialize the CSV Parser using the given path
	csv::Parser file = csv::Parser(csvPath);

	// read and display header row - optional
	vector<string> header = file.getHeader();
	for (auto const& c : header) {
		cout << c << " | ";
	}
	cout << "" << endl;

	try {
		// loop to read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			// Create a data structure and add to the collection of bids
			Bid bid;
			bid.bidId = file[i][1];
			bid.title = file[i][0];
			bid.fund = file[i][8];
			bid.rowPos = i;
			bid.amount = strToDouble(file[i][4], '$');
			binarySearchTree->createTreeNode(bid); // This will initiate a binary tree by addig a bid to the root if one does not alery exist
			//If one alredy exist then the bid will be added to a subtree.
			sizeBinarySearchTree++;
		}
	}
	catch (csv::Error& e) {
		std::cerr << e.what() << std::endl;
	}
}
void loadBidsNewCVS(string csvPath, BinarySearchTree* binarySearchTree) {
	cout << "Loading CSV file " << csvPath << endl;

	// initialize the CSV Parser using the given path
	csv::Parser file = csv::Parser(csvPath);

	// read and display header row - optional
	vector<string> header = file.getHeader();
	for (auto const& c : header) {
		cout << c << " | ";
	}
	cout << "" << endl;

	try {
		// loop to read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			// Create a data structure and add to the collection of bids
			Bid bid;
			bid.bidId = file[i][1];
			bid.title = file[i][0];
			bid.fund = file[i][8];
			bid.rowPos = i;
			bid.amount = strToDouble(file[i][4], '$');
			binarySearchTree->createTreeNode(bid); // This will initiate a binary tree by addig a bid to the root if one does not alery exist
			//If one alredy exist then the bid will be added to a subtree.
			sizeBinarySearchTree++;
		}
	}
	catch (csv::Error& e) {
		std::cerr << e.what() << std::endl;
	}
}

Bid getBid(string csvPath) {// This method will obtain user input and return it as a bid






	Bid bid;

	cout << endl << endl;// The the bid will be sent to addBid and added to a
	BinarySearchTree* cat;
	cat = new BinarySearchTree();
	csv::Parser file = csv::Parser(csvPath);
	string addBidKey;
	int fileRowCount;
	fileRowCount = file.rowCount();
	//fileRowCount++;
	cout << fileRowCount;
	std::vector<std::string> myrow;
	if (X > 0) {




		cout << endl << endl;



		string addBidKey;
		string a;
		string b;
		string c;
		string d;
		int row;

		cin.ignore();
		cout << " ";
		getline(cin, bid.title);

		cout << "Enter bidId: ";
		getline(cin, a);                                  // this will crerate a newee bid and send it to creaTreeNode method so it can be inserted on the bst

		cout << "Enter Title: ";
		getline(cin, b);

		string strAmount;
		cout << "Enter amount: ";
		cin.ignore();
		getline(cin, strAmount);

		cout << "Enter Fund: ";
		getline(cin, d);
		row = fileRowCount;
		bid.bidId = a;
		bid.title = b;
		bid.amount = strToDouble(strAmount, '$');
		bid.fund = d;
		bid.rowPos = row;
		cat->createTreeNode(bid);
		myrow.push_back(b);
		myrow.push_back(a);
		myrow.push_back("3");
		myrow.push_back("4");
		myrow.push_back(d);
		myrow.push_back("6");
		myrow.push_back("7");
		myrow.push_back("8");
		myrow.push_back(strAmount);

		file.deleteRow(fileRowCount);
		file.sync();
		file.addRow(fileRowCount, myrow);
		file.sync();
		cout << endl << endl;






		cout << endl << endl;



	}
	return bid;


}




/**

 * Simple C function to convert a string to a double

 * after stripping out unwanted char

 *

 * credit: http://stackoverflow.com/a/24875936

 *

 * @param ch The character to strip out

 */

double strToDouble(string str, char ch) {
	str.erase(remove(str.begin(), str.end(), ch), str.end());
	return atof(str.c_str());

}

int string_to_int(const std::string& s) // this method will convert string user input into ints.
{
	int intN = std::stof(s);
	return int(intN);
}
bool is_integer(const std::string& s) // this method will tes if user input is an integer
{
	int intN;
	int r;
	intN = std::stof(s);
	r = intN - int(intN);
	return r == 0;
}



int FindMatch(vector<string> stringsList, string itemMatch,
	int lowVal, int highVal, string indentAmt) { // indentAmt used for print debug

	int midVal = 0;    // Midpoint of low and high values
	int itemPos = 0;   // Position where item found, -1 if not found
	int rangeSize = 0; // Remaining range of values to search for match

	cout << indentAmt << "Find() range " << lowVal << " " << highVal << endl;

	rangeSize = (highVal - lowVal) + 1;
	midVal = (highVal + lowVal) / 2;

	if (itemMatch == stringsList.at(midVal)) {   // Base case 1: item found at midVal position
		cout << indentAmt << "Found person." << endl;
		itemPos = midVal;
	}
	else if (rangeSize == 1) {                   // Base case 2: match not found
		cout << indentAmt << "Person not found." << endl;
		itemPos = -1;
	}
	else {                                       // Recursive case: Search lower or upper half
		if (itemMatch < stringsList.at(midVal)) { // Search lower half, recursive call
			cout << indentAmt << "Searching lower half." << endl;
			itemPos = FindMatch(stringsList, itemMatch, lowVal, midVal, indentAmt + "   ");
		}
		else {                                    // Search upper half, recursive call
			cout << indentAmt << "Searching upper half." << endl;
			itemPos = FindMatch(stringsList, itemMatch, midVal + 1, highVal, indentAmt + "   ");
		}
	}

	cout << indentAmt << "Returning pos = " << itemPos << "." << endl << endl << endl;
	return itemPos;
}


string replaceSlash(string newPath) {
	int    itemIndex = 0; // Local variable itemIndex
	string newPath2;
	char h = '\\';
	int mouse = newPath.size();


	char b = ' ';
	greetingText = newPath;

	itemIndex = greetingText.find(h);
	if (itemIndex > 0) {
		b = greetingText.at(itemIndex);

		if (b != h) { // Found
			return greetingText;
		}
		else if (b == h) { // Found
			b = 'b';

			greetingText.replace(itemIndex, 1, "/");
			//	greetingText = greetingText;

			newPath2 = greetingText;

			cout << greetingText << endl;
			itemIndex = greetingText.find(h);
			if (itemIndex > 0) {
				replaceSlash(greetingText);
			}
			//cout << greetingText << endl;
		}

		newPath2 = greetingText;
		// Replace occurrence of "Darn" by "@$#"
		itemIndex = newPath2.find(h);


		//b = newPath2.at(itemIndex);

		if (itemIndex > 0) { // Found

			b = 'a';

			//replaceSlash(newPath2);
		}
		else
			return greetingText;
	}

	else
		return greetingText;





}


void findMissingNumber() {

	int numTestCases;
	int caseSize;
	int aNumber;


	// 4 + 4 + 1 = 9
	// 1 , 2 , 4
	cout << "Enter NumBer Of Test Cases:";
	cin >> numTestCases;


	while (numTestCases != 0) {

		cout << "Enter Case Size:";
		cin >> caseSize;


		int sum = ((caseSize * (caseSize + 1)) / 2);
		for (int i = 0; i < caseSize - 1; i++) {
			cout << "Enter A Case Number:";
			cin >> aNumber;
			sum -= aNumber;
		}

		cout << sum << endl;

		numTestCases--;


	}


}





void loadBidsNewCVSm(string csvPath, BinarySearchTree* binarySearchTree) {
	cout << "Loading CSV file " << csvPath << endl;

	// initialize the CSV Parser using the given path
	csv::Parser file = csv::Parser(csvPath);


	int filecolumnCount2;
	filecolumnCount2 = file.columnCount();

	vector<string> rif(0);


	int x = 0;
	x = filecolumnCount2;
	int y = 0;
	y = filecolumnCount2;
	rif.resize(filecolumnCount2);
	string bb;
	string cc;
	string aa;
	// read and display header row - optional
	vector<string> header = file.getHeader();
	for (auto const& c : header) {
		cout << c << " | ";
		bb = c;
		cc = bb;
		if (aa != c && bb != aa && c != aa) {
			aa = cc;
			cc = " ";
			cout << aa << " | ";
		}

		//	rif.at(y) = c;
			//cout << rif.at(y) << " | ";
			//y = y - 1;





	}



	//for (int il = 0; il < filecolumnCount2; ++il) {
	//	cout << rif.at(il) << " ";
	//}


	cout << "" << endl;

	try {
		// loop to read rows of a CSV file



// loop to read rows of a CSV file
		for (unsigned int i = 0; i < file.rowCount(); i++) {

			// Create a data structure and add to the collection of bids
			Bid bid;
			bid.bidId = file[i][1];
			bid.title = file[i][0];
			bid.fund = file[i][8];
			bid.rowPos = i;
			bid.amount = strToDouble(file[i][4], '$');
			binarySearchTree->createTreeNode(bid); // This will initiate a binary tree by addig a bid to the root if one does not alery exist
			//If one alredy exist then the bid will be added to a subtree.
			sizeBinarySearchTree++;
		}
	}

	catch (csv::Error& e) {
		std::cerr << e.what() << std::endl;
	}
}

/**

 * The one and only main() method

 */



int main(int argc, char* argv[], int argc2, const char** argv2) {







	// Omitting part of program that adds attendees
// Instead, we insert some sample attendees in sorted order
	attendeesList.push_back("Adams, Mary");
	attendeesList.push_back("Carver, Michael");
	attendeesList.push_back("Domer, Hugo");
	attendeesList.push_back("Donlee, Jake");
	attendeesList.push_back("Fredericks, Carlos");
	attendeesList.push_back("Li, Jie");
	attendeesList.push_back("Martinez, Alfredo");
	attendeesList.push_back("Pagaros, Sophie");

	Bid bid; //This creates an instance of the strut Bid
	Bid vectorOfBids;
	//  LinkedList bidList; //This creates an instance of class LinnkedList

	vector<Bid> bids;
	BinarySearchTree* binaryTreeObj;
	binaryTreeObj = new BinarySearchTree;
	//	 binaryTreeObj2 = new BinarySearchTree();
	clock_t ticks; //  This creates an instance of clock_t
	int choice = 0;
	string bidKey;
	string csvPath;
	string csvPathDeletedBids;
	string bidss;
	string filePath;

	int enterPathChoice;
	Node* treeRoot = new Node;


	string ss;


	switch (argc) {
	case 2:
		csvPath = argv[1];
		bidKey = "98109";
		break;
	case 3:
		csvPath = argv[1];
		bidKey = argv[2];
		break;
	default:
		csvPath = "data\\eOfferMonthlySalesNov.csv";
		

	}
	csv::Parser file = csv::Parser(csvPath);
	std::vector<std::string> myrow;


	//loadBidsNewCVSm(csvPath, binaryTreeObj);


	while (choice != 9) {
		cout << "Menu:" << endl;
		cout << " 1. Load Bids" << endl;
		cout << " 2. Display All Bids" << endl;
		cout << " 3. Find Bid" << endl;
		cout << " 4. Remove Bid" << endl;
		cout << " 5. Insert Bid" << endl;
		cout << " 6. Read Line From txt File" << endl;
		cout << " 9. Exit" << endl;
		cout << "10. Recursion Debug" << endl;
		std::string userInput;
		std::string userInput2;
		cout << "Enter choice:";
		try
		{
			// this will prevent the program from crashing by execution the swich below   if  and only if the users input is a digit
			std::cin >> userInput;
			if (is_integer(userInput)) {
				choice = string_to_int(userInput);
				cout << endl << endl;

				switch (choice) {   // This switch is the main switch that will provide users with the programs six main functions.
				case 1:
					if (HasLoadedFile == false) {
						ticks = clock();
						loadBidsNewCVSm(csvPath, binaryTreeObj); // This will load data from a csv file into a vector called nodes.
						//This will also create an array of of poiters that will contain quick  quick refrences to the vector nodes.

						ss = csvPath;
						// Calculate elapsed time and display result
						ticks = clock() - ticks; // current clock ticks minus starting clock ticks
						filePath = csvPath;
						cout << sizeBinarySearchTree << " bids read" << endl;
						cout << "time: " << ticks << " clock ticks" << endl;
						cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl << endl << endl;

						HasLoadedFile = true;
						X++;
					}
					else {
						cout << " 1.Load Previous File  " << filePath << endl;
						cout << " 2.Load Bids From Ebid_Monthly_Sales_Dec-2016.CSV" << endl;
						cout << " 3.Load Bids From Ebid_Monthly_Sales.csv  " << endl;
						cout << " 4.Enter New CSV File Path  " << endl;
						cout << " 5.Back to Main Menu" << endl;
						cout << "Enter choice:";
						std::cin >> userInput2;
						if (is_integer(userInput2)) {
							enterPathChoice = string_to_int(userInput2);

							switch (enterPathChoice) { // This switch will prompt user if the user want to append or prepend the linked list.
							case 1:
								// Initialize a timer variable before loading bids
								ticks = clock();
								// Complete the method call to load the bids
								loadBids(csvPath, binaryTreeObj);
								// Calculate elapsed time and display result
								ticks = clock() - ticks; // current clock ticks minus starting clock ticks
								cout << sizeBinarySearchTree << " bids read" << endl;
								cout << "time: " << ticks << " clock ticks" << endl;
								cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl << endl;

								break;
							case 2:
								filePath = "eBidsMonthlySalesDec.csv";
								sizeBinarySearchTree = 0;
								ticks = clock();
								// Complete the method call to load the bids
								loadBids(filePath, binaryTreeObj);
								ss = filePath;

								// Calculate elapsed time and display result
								ticks = clock() - ticks; // current clock ticks minus starting clock ticks
								cout << sizeBinarySearchTree << " bids read" << endl;
								cout << "time: " << ticks << " clock ticks" << endl;
								cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl << endl;
								break;
							case 3:
								filePath = "data\\eOfferSales.csv";
								sizeBinarySearchTree = 0;
								ticks = clock();
								// Complete the method call to load the bids
								loadBids(filePath, binaryTreeObj);
								ss = filePath;

								// Calculate elapsed time and display result
								ticks = clock() - ticks; // current clock ticks minus starting clock ticks
								cout << sizeBinarySearchTree << " bids read" << endl;
								cout << "time: " << ticks << " clock ticks" << endl;
								cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl << endl;
								break;

							case 4:
								cout << "Enter the absolute CSV File Path:";
								cin >> bidss;
								filePath = replaceSlash(bidss);
								ss = filePath;
								sizeBinarySearchTree = 0;
								sizeBinarySearchTree = 0;
								ticks = clock();
								// Complete the method call to load the bids
								loadBids(filePath, binaryTreeObj);
								// Calculate elapsed time and display result
								ticks = clock() - ticks; // current clock ticks minus starting clock ticks
								cout << sizeBinarySearchTree << " bids read" << endl;
								cout << "time: " << ticks << " clock ticks" << endl;
								cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl << endl;
								break;
							default:
								break;

							}


						}

					}

					break;
				case 2:
					ticks = clock();
					binaryTreeObj->printInOrderFromRoot();

					ticks = clock() - ticks;
					cout << "time: " << ticks << " clock ticks" << endl;
					cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl << endl;
					if (HasLoadedFile == false) {  // this will prompt the user to  select menu choice 2
						cout << " Load Bid file first. " << endl << "Select menu choice 2" << endl << endl;

					}
					cout << endl;
					cout << endl;

					break;
				case 3:
					cout << "Enter BidId:";
					cin >> bidss;
					bidKey = bidss;
					stringToInt = atoi(bidKey.c_str()); // This will cover a string into an int
					ticks = clock(); //  Object ticks begins to account for ticks
					binaryTreeObj->search(treeRoot, bidKey);
					ticks = clock() - ticks; // current clock ticks minus starting clock ticks
					//loadBids(csvPath, binaryTreeObj); // This sinc the current row position of bids by reloading the csvfile
					cout << "time: " << ticks << " clock ticks" << endl;
					cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl << endl;
					break;
				case 4:
					X = 5; // This will remove bids from the bid CSV file, and store those deletions on a CVS file that consists of deleted bids.
					cout << "Enter BidId:";
					cin >> bidss;
					bidKey = bidss;

					// This will prevent the program from crashing if user input does not match and existing BidKey
					ticks = clock(); //  Object ticks begins to account for ticks
					binaryTreeObj->remove(bidKey, csvPath, csvPathDeletedBids); // This method will remove a node if user input  matches an existing BidKey

					ticks = clock() - ticks; // current clock ticks minus starting clock tick

					cout << "time: " << ticks << " clock ticks" << endl;
					cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl << endl;
					cout << endl << endl << endl;
					break;
				case 5:
					if (HasLoadedFile == false) {
						loadBids(csvPath, binaryTreeObj);
						cout << endl << endl;
						bid = getBid(csvPath); // This will get the users input and store it in a struct.
						ticks = clock();
						binaryTreeObj->createTreeNode(bid);
						ticks = clock() - ticks; // current clock ticks minus starting clock ticks
						filePath = csvPath;
						cout << 1 << " bids read" << endl;
						cout << "time: " << ticks << " clock ticks" << endl;
						cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl << endl;

					}
					else {
						cout << endl << endl;
						bid = getBid(csvPath); // This will get the users input and store it in a struct.It will als
						ticks = clock();
						binaryTreeObj->createTreeNode(bid);
						ticks = clock() - ticks; // current clock ticks minus starting clock ticks
						filePath = csvPath;
						cout << 1 << " bids read" << endl;
						cout << "time: " << ticks << " clock ticks" << endl;
						cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl << endl;

					}
					break;


				case 9://This will terminate the program.
					cout << "Good bye." << endl;
					break;
				case 10:
					cin.ignore();
					cout << " ";
					getline(cin, bid.title);
					cout << "Enter person's name: Last, First: ";
					getline(cin, attendeeName); // Use getline to allow space in name

					// Call function to match name, output results
					matchPos = FindMatch(attendeesList, attendeeName, 0,
						attendeesList.size() - 1, "   ");
					if (matchPos >= 0) {
						cout << "Found at position " << matchPos << "." << endl;
					}
					else {
						cout << "Not found. " << endl;
					}





					break;

				default:
					cout << "The input that you entered is not one of the 6 choices. Please try Again" << endl; // This prompt will appear if user input is an integer but the inger is not one of the user input choices.
					choice = 0;
					break;
				}

			}

			else
			{
				std::cout << userInput << "is not valid";
			}

		}    // This will create an exception when a user enters characters that are not integers.
		catch (const std::invalid_argument& ia)
		{
			std::cerr << "Invalid argument: " << ia.what() << '\n';
			std::cout << "User input " << userInput << " is a non integer character . Please enter an integer character." << endl;
		}

	}

	return 0;
}


#include "../include/BinarySearchTree.hpp"
#include "BinarySearchTree.hpp"

// Node Constructor Definitions
template <typename T, typename KeyExtractor>
BinarySearchTree<T, KeyExtractor>::Node::Node() : longestChildPath(1), leftNodePtr(nullptr), rightNodePtr(nullptr) {}

template <typename T, typename KeyExtractor>
BinarySearchTree<T, KeyExtractor>::Node::Node(shared_ptr<T> containedObject)
    : longestChildPath(1), leftNodePtr(nullptr), rightNodePtr(nullptr), containedValueObject(containedObject) {
}

template<typename T, typename KeyExtractor>
BinarySearchTree<T, KeyExtractor>::BinarySearchTree()
{
}

// BinarySearchTree Constructor and Destructor
template <typename T, typename KeyExtractor>
BinarySearchTree<T, KeyExtractor>::BinarySearchTree(KeyExtractor extractor)
    : root(nullptr), bstNodeCount(0), treeHeight(0), keyExtractor(extractor) {
    insertionPath.reserve(32);
}

template <typename T, typename KeyExtractor>
BinarySearchTree<T, KeyExtractor>::~BinarySearchTree() {
    // Add cleanup logic if necessary
}

// Utility Functions
template <typename T, typename KeyExtractor>
bool BinarySearchTree<T, KeyExtractor>::isEmpty() const {
    return root == nullptr;
}

template <typename T, typename KeyExtractor>
unsigned int BinarySearchTree<T, KeyExtractor>::getDeepestLevel() const {
    return treeHeight;
}

template <typename T, typename KeyExtractor>
unsigned int BinarySearchTree<T, KeyExtractor>::getBSTSize() const {
    return bstNodeCount;
}

template <typename T, typename KeyExtractor>
void BinarySearchTree<T, KeyExtractor>::reSetDeepestLevel() {
    treeHeight = 0;
}

template <typename T, typename KeyExtractor>
inline void BinarySearchTree<T, KeyExtractor>::updateTreeMetrics(size_t nodeInsertionHeight) {
    if (nodeInsertionHeight > treeHeight) {
        treeHeight = nodeInsertionHeight;
    }
}

template <typename T, typename KeyExtractor>
void BinarySearchTree<T, KeyExtractor>::debugKeyType() const {
    using UnderlyingKeyType = typename std::decay<Key>::type;
    std::cout << "Key is of type: " << typeid(UnderlyingKeyType).name() << std::endl;

    if constexpr (std::is_integral<UnderlyingKeyType>::value) {
        std::cout << "Key is an integral type!" << std::endl;
    }
    else if constexpr (std::is_floating_point<UnderlyingKeyType>::value) {
        std::cout << "Key is a floating-point type!" << std::endl;
    }
    else if constexpr (std::is_same<UnderlyingKeyType, std::string>::value) {
        std::cout << "Key is a std::string!" << std::endl;
    }
    else {
        std::cout << "Key is some other type!" << std::endl;
    }
}


template <typename T, typename KeyExtractor>
void BinarySearchTree<T, KeyExtractor>::insert(std::shared_ptr<T> containedValueObject) {
    
  

    Node** currentNode = &root; // Start traversal at the root of the tree
    size_t nodeInsertionHeight = 0; // Tracks height of the inserted node
    size_t elementsRemaining = 0; // Tracks remaining nodes for re balancing
    insertionPath.clear(); // Clears path for re-balancing

    Key key = keyExtractor(*containedValueObject); // Extract key for the object being inserted (only once)

    // Traverse the tree to find the correct insertion point
    while (*currentNode != nullptr) {
        insertionPath.push_back(currentNode); // Add the current node to the insertion path for rebalancing

        Key currentKey = keyExtractor(*(*currentNode)->containedValueObject); // Extract the key for the current node (inside the loop)

        if (key >= currentKey) {
            // Set current node to its right child because a deeper traversal is needed to find a null node for insertion.
            currentNode = &((*currentNode)->rightNodePtr);
        }
        else {
            // Set current node to its left child because a deeper traversal is needed to find a null node for insertion.
            currentNode = &((*currentNode)->leftNodePtr);
        }
        
       
    }

    *currentNode = new Node(containedValueObject); // Create a new node and insert it at the current position
    bstNodeCount++; // Increment the node count
    nodeInsertionHeight = insertionPath.size(); // Determine the height of the inserted node
    elementsRemaining = nodeInsertionHeight;
    updateTreeMetrics(nodeInsertionHeight); // Update tree metrics for height

    // Re balance the tree after insertion
    while (elementsRemaining > 0) {
        currentNode = insertionPath.back(); // Retrieve the last node from the insertion path
        fixLeftImbalance(*currentNode); 
        fixRightImbalance(*currentNode); 
        insertionPath.pop_back(); 
        elementsRemaining--;    // Decrement remaining nodes to check for balancing
    }
}

template <typename T, typename KeyExtractor>
template <typename InputKey>
 vector<shared_ptr<T>> BinarySearchTree<T, KeyExtractor>::search(const InputKey& searchKey) {
   
    vector <shared_ptr<T>> sameKeyObjects; // Result object to return
    Node* currNodePosition = root;
    //TODO implement validation before casting. 
    Key convertedKey = static_cast<Key>(searchKey);
    while (currNodePosition != nullptr) { // Traverse the tree until a match is found or the end is reached

        // Compare the search key with the key of the current node
        if (convertedKey == keyExtractor(*currNodePosition->containedValueObject)) {

            sameKeyObjects.push_back(currNodePosition->containedValueObject);
            //
            if ((currNodePosition->rightNodePtr != nullptr && keyExtractor(*currNodePosition->rightNodePtr->containedValueObject) == convertedKey)) {
                // This will check if the right child also has a value matching the search key
             //   sameKeyObjects.push_back(currNodePosition->rightNodePtr->containedValueObject);
                currNodePosition = currNodePosition->rightNodePtr;
               
            }
            // This will check if the right child also has a value matching the search key
            else if ((currNodePosition->leftNodePtr != nullptr && keyExtractor(*currNodePosition->leftNodePtr->containedValueObject) == convertedKey)) {
             //   sameKeyObjects.push_back(currNodePosition->leftNodePtr->containedValueObject);
                currNodePosition = currNodePosition->leftNodePtr;
             
            }else if (currNodePosition->rightNodePtr != nullptr &&    (currNodePosition->rightNodePtr)->leftNodePtr != nullptr) {
                // This will first check if the current node has a right child, 
                // and if it does, then it will check if that right node has a left child
                currNodePosition = (currNodePosition->rightNodePtr)->leftNodePtr;
                // This will set currNodePosition as the left child of its current right child.
                // This will search for duplicate values
            }
            else {
                  return sameKeyObjects;
                // This will end the search function if currNodePosition does not have a right child 
                // or if the right child does not have a value matching the search key
            }
        }

        // Traverse the right branch of a node if the search key is greater than the current key
        if (convertedKey > keyExtractor(*currNodePosition->containedValueObject)) {
            currNodePosition = currNodePosition->rightNodePtr;
        }
        // Traverse the left branch of a node if the search key is less than the current key
        else if (convertedKey < keyExtractor(*currNodePosition->containedValueObject)) {
            currNodePosition = currNodePosition->leftNodePtr;
        }
    }

    return sameKeyObjects; // Return empty vector if no match is found.









}
 //TODO: Allow deletion of all objects with the same key.
template <typename T, typename KeyExtractor>
template <typename InputKey>
void BinarySearchTree<T, KeyExtractor>::remove(const InputKey& searchKey, const string& csvPathDeletedNodes) {
    // credit: http://www.cplusplus.com/forum/general/1551/
    Key convertedKey = static_cast<Key>(searchKey);
    unsigned int deleteRowPosition = 0;

    Node* tmp3;
    Node* tmp2;
    Node* tmp1;
    Node* currNodePosition;
    Node* par = nullptr;
    currNodePosition = root;
    bool nodeExists = false;

    while (currNodePosition != nullptr) {
        if (keyExtractor(*currNodePosition->containedValueObject) == convertedKey) {
            nodeExists = true;
            break;
        }
        else {
            par = currNodePosition;
            if (convertedKey > keyExtractor(*currNodePosition->containedValueObject)) {
                currNodePosition = currNodePosition->rightNodePtr;
            }
            else {
                currNodePosition = currNodePosition->leftNodePtr;
            }
        }
    }

    if (!nodeExists) {
        return; // Exit if the node with the specified key does not exist
    }

    // Node with one child
    if ((currNodePosition->leftNodePtr == nullptr && currNodePosition->rightNodePtr != nullptr) ||
        (currNodePosition->leftNodePtr != nullptr && currNodePosition->rightNodePtr == nullptr)) {
        if (currNodePosition->leftNodePtr == nullptr && currNodePosition->rightNodePtr != nullptr) {
            if (par->leftNodePtr == currNodePosition) {
                par->leftNodePtr = currNodePosition->rightNodePtr;

                // Backup deleted node's data (if applicable)
          //      this->backUpDeletedNode(currNodePosition, csvPathDeletedNodes);
                delete currNodePosition;
                bstNodeCount--;
            }
            else {
                par->rightNodePtr = currNodePosition->rightNodePtr;

             //   this->backUpDeletedNode(currNodePosition, csvPathDeletedNodes);
                delete currNodePosition;
                bstNodeCount--;
            }
        }
        else {
            if (par->leftNodePtr == currNodePosition) {
                par->leftNodePtr = currNodePosition->leftNodePtr;

             //   this->backUpDeletedNode(currNodePosition, csvPathDeletedNodes);
                delete currNodePosition;
                bstNodeCount--;
            }
            else {
                par->rightNodePtr = currNodePosition->leftNodePtr;

            //    this->backUpDeletedNode(currNodePosition, csvPathDeletedNodes);
                delete currNodePosition;
                bstNodeCount--;
            }
        }

        return;
    }

    // Leaf Node
    if (currNodePosition->leftNodePtr == nullptr && currNodePosition->rightNodePtr == nullptr) {
        if (par->leftNodePtr == currNodePosition) {
            par->leftNodePtr = nullptr; // Remove reference to leaf node
        }
        else {
            par->rightNodePtr = nullptr;
        }

       // this->backUpDeletedNode(currNodePosition, csvPathDeletedNodes);
        delete currNodePosition;
        bstNodeCount--;
        return;
    }

    // Node with two children
    if (currNodePosition->leftNodePtr != nullptr && currNodePosition->rightNodePtr != nullptr) {
        tmp1 = currNodePosition->rightNodePtr;

        if (tmp1->leftNodePtr == nullptr && tmp1->rightNodePtr == nullptr) {
            // Case 1: Right child is a leaf
            *currNodePosition->containedValueObject = *tmp1->containedValueObject;
        //    this->backUpDeletedNode(tmp1, csvPathDeletedNodes);
            delete tmp1;
            bstNodeCount--;
            currNodePosition->rightNodePtr = nullptr;
        }
        else {
            if (currNodePosition->rightNodePtr->leftNodePtr != nullptr) {
                // Case 2: Find the leftmost child of the right subtree
                tmp2 = currNodePosition->rightNodePtr;
                tmp3 = currNodePosition->rightNodePtr->leftNodePtr;

                while (tmp3->leftNodePtr != nullptr) {
                    tmp2 = tmp3;
                    tmp3 = tmp3->leftNodePtr;
                }
                *currNodePosition->containedValueObject = *tmp3->containedValueObject;
            //    this->backUpDeletedNode(tmp3, csvPathDeletedNodes);
                delete tmp3;
                bstNodeCount--;
                tmp2->leftNodePtr = nullptr;
            }
            else {
                // Case 3: Replace current node with its right child
                Node* tmp = currNodePosition->rightNodePtr;
                *currNodePosition->containedValueObject = *tmp->containedValueObject;
                currNodePosition->rightNodePtr = tmp->rightNodePtr;

               // this->backUpDeletedNode(tmp, csvPathDeletedNodes);
                delete tmp;
                bstNodeCount--;
            }
        }

        return;
    }
}


template <typename T, typename KeyExtractor>
void BinarySearchTree<T, KeyExtractor>::printInOrder() {
    Node* currentNode = root;
    Node* previousNode = nullptr; // Used to find the rightmost node in the left subtree

    while (currentNode != nullptr) {
        // Check if the left child exists
        if (currentNode->leftNodePtr != nullptr) {

            previousNode = currentNode->leftNodePtr;
            // Find the rightmost node in the left subtree
            while (previousNode->rightNodePtr != nullptr && previousNode->rightNodePtr != currentNode) {
                previousNode = previousNode->rightNodePtr;
            }

            // If no temporary link exists, create one to revisit this node after processing the left subtree
            if (previousNode->rightNodePtr == nullptr) {
                previousNode->rightNodePtr = currentNode;
                currentNode = currentNode->leftNodePtr;

            }
            else {
                // Left subtree has been processed; remove temporary link
                previousNode->rightNodePtr = nullptr;
                // Print the current node's key
                std::cout << keyExtractor(*currentNode->containedValueObject) << std::endl;
                currentNode = currentNode->rightNodePtr;
            }

        }
        else {
            // Print the current node's key if no left subtree exists
            std::cout << keyExtractor(*currentNode->containedValueObject) << std::endl;
            currentNode = currentNode->rightNodePtr;
        }
    }
    // This implementation is based on the Morris Traversal algorithm.
    // Reference: Morris, J. H. (1979). Traversing binary trees simply and cheaply.
    // Communications of the ACM, 22(8), 235–239. See README.md for details.
}

//TODO: Improve balancing performance
template <typename T, typename KeyExtractor>
void BinarySearchTree<T, KeyExtractor>::fixLeftImbalance(Node*& subTreeRootNode) {

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

//TODO: Improve balancing performance
template <typename T, typename KeyExtractor>
void BinarySearchTree<T, KeyExtractor>::fixRightImbalance(Node*& subTreeRootNode) {

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
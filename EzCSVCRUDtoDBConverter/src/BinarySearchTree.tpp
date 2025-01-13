

#include "../include/BinarySearchTree.hpp"
#include "BinarySearchTree.hpp"


// Node Constructor Definitions
template <typename T, typename KeyExtractor>
BinarySearchTree<T, KeyExtractor>::Node::Node() : parentNodePtr(nullptr), longestChildPath(1), leftNodePtr(nullptr), rightNodePtr(nullptr) {}

template <typename T, typename KeyExtractor>
BinarySearchTree<T, KeyExtractor>::Node::Node(shared_ptr<T> containedObject)
    : longestChildPath(initialNodeLevel), parentNodePtr(nullptr), leftNodePtr(nullptr), rightNodePtr(nullptr), containedValueObject(containedObject) {
}


// BinarySearchTree Constructor and Destructor
template <typename T, typename KeyExtractor>
BinarySearchTree<T, KeyExtractor>::BinarySearchTree(KeyExtractor extractor)
    : root(nullptr), bstNodeCount(0), treeHeight(0), keyExtractor(extractor), maxParentLookUp(2), currentParentLookUp(0) {
}

template <typename T, typename KeyExtractor>
BinarySearchTree<T, KeyExtractor>::~BinarySearchTree() {
    // Add cleanup logic if necessary
}

// Utility Functions
template <typename T, typename KeyExtractor>
bool BinarySearchTree<T, KeyExtractor>::isTreeEmpty() const {
    return root == nullptr;
}

template <typename T, typename KeyExtractor>
size_t BinarySearchTree<T, KeyExtractor>::getDeepestLevel() const {
    return treeHeight;
}

template <typename T, typename KeyExtractor>
size_t BinarySearchTree<T, KeyExtractor>::getBSTSize() const {
    return bstNodeCount;
}

template <typename T, typename KeyExtractor>
void BinarySearchTree<T, KeyExtractor>::reSetDeepestLevel() {
    treeHeight = 0;
}

template<typename T, typename KeyExtractor>
inline unsigned BinarySearchTree<T, KeyExtractor>::calculateMaxNodesInHight(unsigned nodeHeight)
{
    return std::pow(2, nodeHeight);
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
    Node* parent = nullptr; // Tracks the parent node during traversal
    Node* nextParent = nullptr; // Tracks the parent node during traversal
    size_t nodeInsertionHeight = 0; // Tracks height of the inserted node



    Key key = keyExtractor(*containedValueObject); // Extract key for the object being inserted (only once)

    // Traverse the tree to find the correct insertion point
    while (*currentNode != nullptr) {
        parent = *currentNode; //Update parent node to the last visited node
        Key currentKey = keyExtractor(*(*currentNode)->containedValueObject); // Extract the key for the current node (inside the loop)

        if (key >= currentKey) {
            // Set current node to its right child because a deeper traversal is needed to find a null node for insertion.
            currentNode = &((*currentNode)->rightNodePtr);
        }
        else {
            // Set current node to its left child because a deeper traversal is needed to find a null node for insertion.
            currentNode = &((*currentNode)->leftNodePtr);
        }

        nodeInsertionHeight++;
        updateTreeMetrics(nodeInsertionHeight); // Update tree metrics for height

    }

    *currentNode = new Node(containedValueObject); // Create a new node and insert it at the current position



    bstNodeCount++;



    if (parent != nullptr) {
        (*currentNode)->parentNodePtr = parent; // Set parent pointer

    }
    else {
        (*currentNode)->parentNodePtr = nullptr; // Root case
    }
    //Get next parent before re-balance because re-balance may shuffle the sub root with one of its children.
//    if (parent) {
  //      nextParent = parent->parentNodePtr;
 //  }
    std::cout << "AFTER INSERT!" << std::endl;
    printNodeDetails(*currentNode);
  //  printLevelOrder();
    std::cout << "END AFTER INSERT!\n\n";
    // Re balance the tree starting from the inserted node's parent
    bool fixLeft = false;
    bool fixRight = false;
    bool last = false;
    while (currentNode != nullptr) {
        // Fix left and right imbalance
        fixLeft = fixLeftImbalance(*currentNode);
        if (!fixLeft) {
            *currentNode = (*currentNode)->parentNodePtr;
            printLevelOrder();
        }
        fixRight = fixRightImbalance(*currentNode);


        if (last) {

            currentParentLookUp = 0;
            // Stop upward traversal if no rebalancing occurred
            printLevelOrder();
            break;

        }
        if (parent) {
            currentNode = &parent;// Move to the parent node pointer up the tree
            //Get next parent before re-balance because re-balance may shuffle the sub root with one of its children.
            parent = parent->parentNodePtr;
        }
        else {
            printLevelOrder();
       //     currentNode = &parent;// Move to the parent node pointer up the tree
            last = true;
            break;
        }


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
void BinarySearchTree<T, KeyExtractor>::printNodeDetails(Node* node) {
    if (node == nullptr) {
        return;
    }
    std::cout << "-----------------" << endl;
    // Node ID
    auto nodeID = keyExtractor(*node->containedValueObject);

    // Print current node details
    std::cout << "Node ID: " << nodeID
        << ", Level: " << node->longestChildPath
        << "\n";


    // Parent ID
    if (node->parentNodePtr != nullptr) {
        auto parentID = keyExtractor(*node->parentNodePtr->containedValueObject);
        std::cout << "Parent ID: " << parentID << ", " << "Level: " << node->parentNodePtr->longestChildPath << "\n";
    }
    else {
        std::cout << "Parent ID: null \n";
    }

    // Left Child ID
    if (node->leftNodePtr != nullptr) {
        auto leftChildID = keyExtractor(*node->leftNodePtr->containedValueObject);
        std::cout << "Left Child ID: " << leftChildID << ", " << "Level: " << node->leftNodePtr->longestChildPath << "\n";
    }
    else {
        std::cout << "Left Child ID: null \n";
    }

    // Right Child ID
    if (node->rightNodePtr != nullptr) {
        auto rightChildID = keyExtractor(*node->rightNodePtr->containedValueObject);
        std::cout << "Right Child ID: " << rightChildID << ", " << "Level: " << node->rightNodePtr->longestChildPath << "\n";

        if (node->rightNodePtr->rightNodePtr != nullptr) {
            auto rightRightChildID = keyExtractor(*node->rightNodePtr->rightNodePtr->containedValueObject);
            std::cout << "Right Right Child ID: " << rightRightChildID << ", " << "Level: " << node->rightNodePtr->rightNodePtr->longestChildPath << "\n";
        }
        else {
            std::cout << "Right Right Child ID: null \n";
        }
    }
    else {
        std::cout << "Right Child ID: null \n";
    }

    


    std::cout << "-----------------" << endl;
}




template <typename T, typename KeyExtractor>
void BinarySearchTree<T, KeyExtractor>::printLevelOrder() {
    if (isTreeEmpty()) {
        std::cout << "Tree is empty.\n";
        return;
    }

    std::deque<std::pair<Node*, std::string>> nodeQueue; // Pair of Node and its type
    nodeQueue.push_back({ root, "Root" }); // Start with the root node
    size_t currentHeight = 0;
    size_t nodesChecked = 0;
    size_t maxNodesAtHeight = calculateMaxNodesInHight(currentHeight);

    size_t nodesInCurrentLevel = 1;  // Start with the root node
    size_t nextNodesInCurrentLevel = 0;

    std::cout << "\n\nStarting Level-Order Traversal\n";

    while (nodesInCurrentLevel > 0) {
        for (size_t i = 0; i < nodesInCurrentLevel; ++i) {
            // Access the front node and remove it from the queue
            auto [currentNode, nodeType] = nodeQueue.front();
            nodeQueue.pop_front();

            // Print node details
            std::cout << "Node Key: "
                << keyExtractor(*currentNode->containedValueObject)
                << ", Type: " << nodeType
                << ", Node Height: " << currentHeight
                << ", Nodes Checked at Height: " << nodesChecked + 1
                << ", Max Nodes in Height: " << maxNodesAtHeight
                << "\n";
            printNodeDetails(currentNode);

            // Add left and right children to the queue
            if (currentNode->leftNodePtr) {
                nodeQueue.push_back({ currentNode->leftNodePtr, "Left Child" });
                ++nextNodesInCurrentLevel; // Increment count for the next level
            }
            if (currentNode->rightNodePtr) {
                nodeQueue.push_back({ currentNode->rightNodePtr, "Right Child" });
                ++nextNodesInCurrentLevel; // Increment count for the next level
            }

            ++nodesChecked;
        }

        // Update for the next level
        nodesInCurrentLevel = nextNodesInCurrentLevel;
        nextNodesInCurrentLevel = 0;  // Reset for the next iteration
        ++currentHeight;
        nodesChecked = 0;
        maxNodesAtHeight = calculateMaxNodesInHight(currentHeight);
    }

    std::cout << "Level-Order Traversal Completed\n\n";
}



template <typename T, typename KeyExtractor>
void BinarySearchTree<T, KeyExtractor>::printInOrder() {
    Node* currentNode = root;
    Node* previousNode = nullptr; // Used to find the rightmost node in the left subtree
    size_t nodeInsertionHeight = 0; // Tracks height of the inserted node
    size_t childNumber = 0;
    std::cout << keyExtractor(*currentNode->containedValueObject) << ": leftChild" << " level : " << currentNode->longestChildPath << std::endl;
    printNodeDetails(currentNode);
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

            }else {
               
                // Left subtree has been processed; remove temporary link
                previousNode->rightNodePtr = nullptr;
                // Print the current node's key
                std::cout << "\n" <<  keyExtractor(*currentNode->containedValueObject) << ": leftChild" << " level : " << currentNode->longestChildPath << ", Child Number : " << ++childNumber << "\n";
                printNodeDetails(currentNode);
                currentNode = currentNode->rightNodePtr;

            }

        }else {
            // Print the current node's key if no left subtree exists
            std::cout << "\n" << keyExtractor(*currentNode->containedValueObject) << ": rightChild" << " level : " << currentNode->longestChildPath << ", Child Number : " << ++childNumber  << "\n";
            printNodeDetails(currentNode);
            currentNode = currentNode->rightNodePtr;
        }
    }
    // This implementation is based on the Morris Traversal algorithm.
    // Reference: Morris, J. H. (1979). Traversing binary trees simply and cheaply.
    // Communications of the ACM, 22(8), 235–239. See README.md for details.
}

//TODO: Improve balancing performance
template <typename T, typename KeyExtractor>
bool BinarySearchTree<T, KeyExtractor>::fixLeftImbalance(Node* subTreeRootNode) {

    Node* subTreeRootNodeLeftChild = nullptr;
    bool isRoot = false;
    cout << "Before fixLeft" << endl;
    printNodeDetails(subTreeRootNode);
    if (subTreeRootNode == nullptr || subTreeRootNode->leftNodePtr == nullptr) {



            return true;


    }

    Node* subTreeRootParent = subTreeRootNode->parentNodePtr;

    if (!isTreeEmpty() && subTreeRootNode == root) {
        isRoot = true;
    }

   
   

    if ((subTreeRootNode->leftNodePtr->longestChildPath >= subTreeRootNode->longestChildPath))  {
      
        printLevelOrder();
        // Step 1: 
        subTreeRootNodeLeftChild = subTreeRootNode->leftNodePtr;
        subTreeRootNodeLeftChild->parentNodePtr = subTreeRootNode->parentNodePtr;

       


      

          // Step 2: Update subTreeRootNode->leftNodePtr to subTreeRootNodeLeftChild->rightNodePtr
         subTreeRootNode->leftNodePtr = subTreeRootNodeLeftChild->rightNodePtr;
 
         subTreeRootNodeLeftChild->rightNodePtr = subTreeRootNode;
         subTreeRootNodeLeftChild->rightNodePtr->parentNodePtr = subTreeRootNodeLeftChild;
        
         cout << "AFTER fixLeft" << endl;

         if (isRoot) {
             root = subTreeRootNodeLeftChild;
             printNodeDetails(subTreeRootNodeLeftChild);
            
         }
         else {

             if(subTreeRootParent->leftNodePtr == subTreeRootNodeLeftChild->rightNodePtr){
             
                 subTreeRootParent->leftNodePtr = subTreeRootNodeLeftChild;
             }
             else {
                 subTreeRootParent->rightNodePtr = subTreeRootNodeLeftChild;
             }

            
             printNodeDetails(subTreeRootNodeLeftChild);
       //      subTreeRootNode = subTreeRootNodeLeftChild;

             
         }
         printLevelOrder();

              return false;
    }

    return true;

    // This implementation is based on AA Trees by Anderson Arne 1993. See README.md for details.

}




//TODO: Improve balancing performance
template <typename T, typename KeyExtractor>
bool BinarySearchTree<T, KeyExtractor>::fixRightImbalance(Node* subTreeRootNode) {


    cout << "Before fixRight" << endl;
    cout << endl;
    printNodeDetails(subTreeRootNode);
    Node* subTreeRootNodeRightChild = nullptr;

    bool isRoot = false;
  

    if (subTreeRootNode == nullptr) {
        std::cerr << "subTreeRootNode is nullptr\n";
        return true;
    }

    if (subTreeRootNode->rightNodePtr == nullptr || (subTreeRootNode->rightNodePtr->rightNodePtr == nullptr)) {
     

        if (currentParentLookUp < maxParentLookUp && subTreeRootNode->parentNodePtr != nullptr) {
            currentParentLookUp++;
            return false;
        }
        else {
            return true;
        }

    }
   


    Node* subTreeRootParent = subTreeRootNode->parentNodePtr;

    // Check if the current node's longest child path equals the level of its right-right grandchild.
    if (subTreeRootNode->longestChildPath == subTreeRootNode->rightNodePtr->rightNodePtr->longestChildPath) {

        printLevelOrder();
      
        subTreeRootNodeRightChild = subTreeRootNode->rightNodePtr;

        if (!isTreeEmpty() && subTreeRootNode == root) {
            isRoot = true;
        }





        //subTreeRootNodeRightChild->rightNodePtr->parentNodePtr = subTreeRootNodeRightChild;



        subTreeRootNode->rightNodePtr = subTreeRootNodeRightChild->leftNodePtr;
        if (subTreeRootNode->rightNodePtr) {

            subTreeRootNode->rightNodePtr->parentNodePtr = subTreeRootNode;
        }

       
        subTreeRootNodeRightChild->leftNodePtr = subTreeRootNode;





        subTreeRootNodeRightChild->longestChildPath++;

        subTreeRootNodeRightChild->leftNodePtr->parentNodePtr = subTreeRootNodeRightChild;


        subTreeRootNodeRightChild->rightNodePtr->parentNodePtr = subTreeRootNodeRightChild;
        subTreeRootNodeRightChild->parentNodePtr = subTreeRootParent;
  

        cout << "After fixRight" << endl;

       // subTreeRootNodeRightChild->leftNodePtr->rightNodePtr->parentNodePtr = subTreeRootNodeRightChild->leftNodePtr;

        if ((!isTreeEmpty()) && (isRoot)) {
            root = subTreeRootNodeRightChild;
            printNodeDetails(subTreeRootNodeRightChild);
           
        }
        else {


            if (subTreeRootParent->leftNodePtr == subTreeRootNodeRightChild->leftNodePtr) {

                subTreeRootParent->leftNodePtr = subTreeRootNodeRightChild;
            }
            else {
                subTreeRootParent->rightNodePtr = subTreeRootNodeRightChild;
            }


           
            printNodeDetails(subTreeRootNodeRightChild);
          
     //       subTreeRootNode = subTreeRootNodeRightChild;
           
        }
        printLevelOrder();

        return false;

    }

    // Return the node unchanged if no balancing needed.
    return false;
}
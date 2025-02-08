

#include "../include/BinarySearchTree.hpp"
#include "BinarySearchTree.hpp"


namespace EzCSCCRUDtoDBConverter {

 
    // Node Constructor Definitions
    template <typename T, typename KeyExtractor>
    BinarySearchTree<T, KeyExtractor>::Node::Node() : parentNodePtr(nullptr), longestChildPath(1), leftNodePtr(nullptr), rightNodePtr(nullptr) {}

    template <typename T, typename KeyExtractor>
    BinarySearchTree<T, KeyExtractor>::Node::Node(std::shared_ptr<T> containedObject)
        : longestChildPath(initialNodeLevel), hasDuplicate(false), parentNodePtr(nullptr), leftNodePtr(nullptr), rightNodePtr(nullptr), containedValueObject(containedObject) {
    }


    template <typename T, typename KeyExtractor>
    BinarySearchTree<T, KeyExtractor>::BinarySearchTree(KeyExtractor extractor)
        : maxParentLookUp(2), currentParentLookUp(0), tempRoot(nullptr), leftTempRoot(nullptr), rightTempRoot(nullptr), root(nullptr), bstNodeCount(0), treeHeight(0), keyExtractor(extractor) {
    }

    template <typename T, typename KeyExtractor>
    BinarySearchTree<T, KeyExtractor>::~BinarySearchTree() {

    }


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
    size_t BinarySearchTree<T, KeyExtractor>::calculateMaxNodesInHight(size_t nodeHeight)
    {
        size_t maxNodesInHight = 1;
        size_t base = 2;
        for (size_t i = 0; i < nodeHeight; ++i) {
            maxNodesInHight *= base;
        }
        return maxNodesInHight;
    }
 
    template<typename T, typename KeyExtractor>
    typename BinarySearchTree<T, KeyExtractor>::Node*
        BinarySearchTree<T, KeyExtractor>::getRoot(const T& value) const {
    }

    template<typename T, typename KeyExtractor>
    void BinarySearchTree<T, KeyExtractor>::setLeftMidpoint() {
        if (!root) {
            return;
        }

        Node* slow = root;
        Node* fast = root;

        // This uses the tortoise and hare approach to find the midpoint
        while (fast && fast->leftNodePtr) {
            slow = slow->leftNodePtr;
            printNodeDetails(slow);
            fast = fast->leftNodePtr->leftNodePtr;
            printNodeDetails(fast);
        }
        leftTempRoot = slow;
    }

    template<typename T, typename KeyExtractor>
    void BinarySearchTree<T, KeyExtractor>::setRightMidpoint() {
        if (!root) {
            return;
        }

        Node* slow = root;
        Node* fast = root;


        while (fast && fast->rightNodePtr) {
            slow = slow->rightNodePtr;
            printNodeDetails(slow);
            fast = fast->rightNodePtr->rightNodePtr;
            printNodeDetails(fast);
        }
        rightTempRoot = slow;
    }

    template<typename T, typename KeyExtractor>
    template <typename InputKey>
    inline void BinarySearchTree<T, KeyExtractor>::setTempRoot(const InputKey& searchKey)
    {
        Key convertedKey = static_cast<Key>(searchKey);
        Key rootKey = keyExtractor(*root->containedValueObject);
        Key leftKey = keyExtractor(*leftTempRoot->containedValueObject);
        Key rightKey = keyExtractor(*rightTempRoot->containedValueObject);
        if (convertedKey > rootKey) {

            if (convertedKey >= rightKey) {
                tempRoot = rightTempRoot;
            }
            else {
                tempRoot = root->rightNodePtr;
            }

            std::cout << "SearchKey > rootKey \n";
            
        }else if (convertedKey < rootKey) {

            if (convertedKey <= leftKey) {
                tempRoot = leftTempRoot;
            }
            else {
                tempRoot = root->leftNodePtr;
            }
            std::cout << "SearchKey < rootKey \n";
        }
        else {
            tempRoot = root;
            std::cout << "SearchKey == rootKey \n";
        }

    }

    template<typename T, typename KeyExtractor>
    inline void BinarySearchTree<T, KeyExtractor>::setRoot(Node* newRoot)
    {
        root = newRoot;
        tempRoot = root;
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
        Node* nextNode = nullptr; // Tracks the parent node during traversal
        size_t nodeInsertionHeight = 0; // Tracks height of the inserted node
        Node* someNode = nullptr;


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

        nextNode = *currentNode;

        bstNodeCount++;

    

        if (parent != nullptr) {
            nextNode->parentNodePtr = parent; // Set parent pointer
          
        }
        else {
            nextNode->parentNodePtr = nullptr; // Root case
        }

      



        bool fixLeft = false;
        bool fixRight = false;
        while (nextNode != nullptr) {
            // Fix left and right imbalance
            fixLeft = fixLeftImbalance(nextNode);
            if (!fixLeft) {

                //  if (nextNode->parentNodePtr->parentNodePtr) {
                //      nextNode = nextNode->parentNodePtr->parentNodePtr;
                //  }
                //  else {
                nextNode = nextNode->parentNodePtr;
                //    }


            }
            fixRight = fixRightImbalance(nextNode);



           if (parent && !(fixLeft && fixRight)) {
          //  if (parent ) {
                nextNode = parent;// Move to the parent node pointer up the tree
                //Get next parent before re-balance because re-balance may shuffle the sub root with one of its children.
                parent = parent->parentNodePtr;
            }
            else {
                currentParentLookUp = 0;
                nextNode = nullptr;
            }


        }

    }

    template <typename T, typename KeyExtractor>
    template <typename InputKey>
    std::vector<std::shared_ptr<T>> BinarySearchTree<T, KeyExtractor>::search(const InputKey& searchKey) {

        std::vector <std::shared_ptr<T>> sameKeyObjects; // Result object to return
        Node* currNodePosition = tempRoot;
        //TODO: implement validation before casting. 
        Key convertedKey = static_cast<Key>(searchKey);
        while (currNodePosition != nullptr) { // Traverse the tree until a match is found or the end is reached

            // Compare the search key with the key of the current node
            if (convertedKey == keyExtractor(*currNodePosition->containedValueObject)) {

                sameKeyObjects.push_back(currNodePosition->containedValueObject);
                if ((currNodePosition->rightNodePtr != nullptr && keyExtractor(*currNodePosition->rightNodePtr->containedValueObject) == convertedKey)) {
                    // This will check if the right child also has a value matching the search key
                 //   sameKeyObjects.push_back(currNodePosition->rightNodePtr->containedValueObject);
                    currNodePosition = currNodePosition->rightNodePtr;

                }
                // This will check if the right child also has a value matching the search key
                else if ((currNodePosition->leftNodePtr != nullptr && keyExtractor(*currNodePosition->leftNodePtr->containedValueObject) == convertedKey)) {
                    //   sameKeyObjects.push_back(currNodePosition->leftNodePtr->containedValueObject);
                    currNodePosition = currNodePosition->leftNodePtr;

                }
                else if (currNodePosition->rightNodePtr != nullptr && (currNodePosition->rightNodePtr)->leftNodePtr != nullptr) {
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
            else  {
                currNodePosition = currNodePosition->leftNodePtr;
            }
        }

        return sameKeyObjects; // Return empty vector if no match is found.









    }
    //TODO: Allow deletion of all objects with the same key.
    template <typename T, typename KeyExtractor>
    template <typename InputKey>
    void BinarySearchTree<T, KeyExtractor>::remove(const InputKey& searchKey) {
        Key convertedKey = static_cast<Key>(searchKey);
        unsigned int deleteRowPosition = 0;

        Node* tmp3;
        Node* tmp2;
        Node* tmp1;
        Node* currNode;
        Node* parent = nullptr;
        currNode = root;
        bool nodeExists = false;
        //Code in this while loop finds the node with the matching searchKey
        while (currNode != nullptr) {
            if (keyExtractor(*currNode->containedValueObject) == convertedKey) {
                nodeExists = true;
                break;
            }
            else {
                parent = currNode;
                if (convertedKey > keyExtractor(*currNode->containedValueObject)) {
                    currNode = currNode->rightNodePtr;
                }
                else {
                    currNode = currNode->leftNodePtr;
                }
            }
        }

        if (!nodeExists) {
            return; // Exit if the node with the specified key does not exist
        }

        // Node with one child
        if ((parent && parent->leftNodePtr && currNode->leftNodePtr == nullptr && currNode->rightNodePtr != nullptr) ||
            (currNode->leftNodePtr != nullptr && currNode->rightNodePtr == nullptr)) {
            if (currNode->leftNodePtr == nullptr && currNode->rightNodePtr != nullptr) {
                if (parent->leftNodePtr == currNode) {
                    parent->leftNodePtr = currNode->rightNodePtr;

          
                    delete currNode;
                    bstNodeCount--;
                }
                else {
                    parent->rightNodePtr = currNode->rightNodePtr;
                    delete currNode;
                    bstNodeCount--;
                }
            }else {
                if (parent && parent->leftNodePtr && parent->leftNodePtr == currNode) {
                    parent->leftNodePtr = currNode->leftNodePtr;
                    bstNodeCount--;
                }
                else {
                    if (parent && parent->rightNodePtr) {
                        parent->rightNodePtr = currNode->leftNodePtr;
                        delete currNode;
                        bstNodeCount--;
                    }
                }
            }

            return;
        }

        
        if (parent && currNode->leftNodePtr == nullptr && currNode->rightNodePtr == nullptr) {
            if (parent->leftNodePtr && parent->leftNodePtr == currNode) {
                parent->leftNodePtr = nullptr; // Remove reference to leaf node
            }
            else {
                parent->rightNodePtr = nullptr;
            }
            delete currNode;
            bstNodeCount--;
            return;
        }

        // Node with two children
        if (currNode->leftNodePtr != nullptr && currNode->rightNodePtr != nullptr) {
            tmp1 = currNode->rightNodePtr;

            if (tmp1->leftNodePtr == nullptr && tmp1->rightNodePtr == nullptr) {
                // Case 1: Right child is a leaf
                *currNode->containedValueObject = *tmp1->containedValueObject;
                delete tmp1;
                bstNodeCount--;
                currNode->rightNodePtr = nullptr;
            }
            else {
                if (currNode->rightNodePtr->leftNodePtr != nullptr) {
                    // Case 2: Find the leftmost child of the right subtree
                    tmp2 = currNode->rightNodePtr;
                    tmp3 = currNode->rightNodePtr->leftNodePtr;

                    while (tmp3->leftNodePtr != nullptr) {
                        tmp2 = tmp3;
                        tmp3 = tmp3->leftNodePtr;
                    }
                    *currNode->containedValueObject = *tmp3->containedValueObject;
                    delete tmp3;
                    bstNodeCount--;
                    tmp2->leftNodePtr = nullptr;
                }
                else {
                    // Case 3: Replace current node with its right child
                    Node* tmp = currNode->rightNodePtr;
                    *currNode->containedValueObject = *tmp->containedValueObject;
                    currNode->rightNodePtr = tmp->rightNodePtr;
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
       
        Node* childrenParent;
        childrenParent = node;
        std::cout << "-----------------" << std::endl;




        // Node ID
        auto nodeID = keyExtractor(*node->containedValueObject);




        // Parent ID
        if (node->parentNodePtr != nullptr) {
            auto parentID = keyExtractor(*node->parentNodePtr->containedValueObject);
            std::cout << "Parent ID: " << parentID << ", " << "Level: " << node->parentNodePtr->longestChildPath << "\n";

            if (node->parentNodePtr->leftNodePtr && node->parentNodePtr->leftNodePtr == node) {
                std::cout << "node: " << nodeID << ", " << "is left child of node: " << parentID << "\n";
            }
            else if (node->parentNodePtr->rightNodePtr && node->parentNodePtr->rightNodePtr == node) {
                std::cout << "node: " << nodeID << ", " << "is right child of node: " << parentID << "\n";
            }
            else {
                std::cout << "ERROR node: " << nodeID << ", " << "is NOT child of node: " << parentID << "\n";
            }


        }
        else {
            std::cout << "Parent ID: null \n";
        }



        // Print current node details
        std::cout << "  Node ID: " << nodeID
            << ", Level: " << node->longestChildPath
            << "\n";




        // Left Child ID
        if (node->leftNodePtr != nullptr) {
            auto leftChildID = keyExtractor(*node->leftNodePtr->containedValueObject);
            auto lParentID = keyExtractor(*node->leftNodePtr->parentNodePtr->containedValueObject);
            std::cout << "      Left Child ID: " << leftChildID << ", " << "Level: " << node->leftNodePtr->longestChildPath << ", Parent ID: " << lParentID << "\n";
            if (node->leftNodePtr != nullptr) {

                if (node->leftNodePtr->leftNodePtr != nullptr) {
                    auto leftLeftChildID = keyExtractor(*node->leftNodePtr->leftNodePtr->containedValueObject);
                    auto lLParentID = keyExtractor(*node->leftNodePtr->leftNodePtr->parentNodePtr->containedValueObject);
                    std::cout << "          Left left Child ID: " << leftLeftChildID << ", " << "Level: " << node->leftNodePtr->leftNodePtr->longestChildPath << ", Parent ID: " << lLParentID << "\n";

                    if (node->leftNodePtr->rightNodePtr != nullptr) {
                        auto leftRightChildID = keyExtractor(*node->leftNodePtr->rightNodePtr->containedValueObject);
                        auto lRParentID = keyExtractor(*node->leftNodePtr->rightNodePtr->parentNodePtr->containedValueObject);
                        std::cout << "          Left right Child ID: " << leftRightChildID << ", " << "Level: " << node->leftNodePtr->rightNodePtr->longestChildPath << ", Parent ID: " << lRParentID << "\n";
                    }


                }


            }



        }
      

        // Right Child ID
        if (node->rightNodePtr != nullptr) {
            auto rightChildID = keyExtractor(*node->rightNodePtr->containedValueObject);
            auto rParentID = keyExtractor(*node->rightNodePtr->parentNodePtr->containedValueObject);
            std::cout << "      Right Child ID: " << rightChildID << ", " << "Level: " << node->rightNodePtr->longestChildPath << ", Parent ID: " << rParentID << "\n";
           
            if (node->rightNodePtr != nullptr) {

                if (node->rightNodePtr->leftNodePtr != nullptr) {
                    auto rightLeftChildID = keyExtractor(*node->rightNodePtr->leftNodePtr->containedValueObject);
                    auto rLParentID = keyExtractor(*node->rightNodePtr->leftNodePtr->parentNodePtr->containedValueObject);
                    std::cout << "          Right Left Child ID : " << rightLeftChildID << ", " << "Level : " << node->rightNodePtr->leftNodePtr->longestChildPath << ", Parent ID : " << rLParentID << "\n";
                }




                if (node->rightNodePtr->rightNodePtr != nullptr) {
                    auto rightRightChildID = keyExtractor(*node->rightNodePtr->rightNodePtr->containedValueObject);
                    auto rRParentID = keyExtractor(*node->rightNodePtr->rightNodePtr->parentNodePtr->containedValueObject);
                    std::cout << "          Right Right Child ID: " << rightRightChildID << ", " << "Level: " << node->rightNodePtr->rightNodePtr->longestChildPath << ", Parent ID: " << rRParentID << "\n";




                }

            }
        }
       

        std::cout << "-----------------" << std::endl;
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

                }
                else {

                    // Left subtree has been processed; remove temporary link
                    previousNode->rightNodePtr = nullptr;
                    // Print the current node's key
                    std::cout << "\n" << keyExtractor(*currentNode->containedValueObject) << ": leftChild" << " level : " << currentNode->longestChildPath << ", Child Number : " << ++childNumber << "\n";
                    printNodeDetails(currentNode);
                    currentNode = currentNode->rightNodePtr;

                }

            }
            else {
                // Print the current node's key if no left subtree exists
                std::cout << "\n" << keyExtractor(*currentNode->containedValueObject) << ": rightChild" << " level : " << currentNode->longestChildPath << ", Child Number : " << ++childNumber << "\n";
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

        if (subTreeRootNode == nullptr || subTreeRootNode->leftNodePtr == nullptr) {



            return true;


        }

        Node* subTreeRootParent = subTreeRootNode->parentNodePtr;

        if (!isTreeEmpty() && subTreeRootNode == root) {
            isRoot = true;
        }




        if ((subTreeRootNode->leftNodePtr->longestChildPath >= subTreeRootNode->longestChildPath)) {
 
            subTreeRootNodeLeftChild = subTreeRootNode->leftNodePtr;
            subTreeRootNodeLeftChild->parentNodePtr = subTreeRootNode->parentNodePtr;





            subTreeRootNode->leftNodePtr = subTreeRootNodeLeftChild->rightNodePtr;

            subTreeRootNodeLeftChild->rightNodePtr = subTreeRootNode;
            subTreeRootNodeLeftChild->rightNodePtr->parentNodePtr = subTreeRootNodeLeftChild;


            if (isRoot) {
                
                setRoot(subTreeRootNodeLeftChild);
              

            }
           
            else {
                if (subTreeRootParent) {
                    if (subTreeRootParent->leftNodePtr == subTreeRootNodeLeftChild->rightNodePtr) {
 
                        subTreeRootParent->leftNodePtr = subTreeRootNodeLeftChild;
                    } else {

                        subTreeRootParent->rightNodePtr = subTreeRootNodeLeftChild;
                    }

                }
                
               



            }
           
            if (subTreeRootNodeLeftChild->rightNodePtr && subTreeRootNodeLeftChild->rightNodePtr->leftNodePtr && subTreeRootNodeLeftChild == subTreeRootNodeLeftChild->rightNodePtr->leftNodePtr->parentNodePtr) {
               
    
                subTreeRootNodeLeftChild->rightNodePtr->leftNodePtr->parentNodePtr = subTreeRootNodeLeftChild->rightNodePtr;

            }


            return false;
        }

        return true;

        // This implementation is based on AA Trees by Anderson Arne 1993. See README.md for details.

    }




    //TODO: Improve balancing performance
    template <typename T, typename KeyExtractor>
    bool BinarySearchTree<T, KeyExtractor>::fixRightImbalance(Node* subTreeRootNode) {


       
        Node* subTreeRootNodeRightChild = nullptr;

        bool isRoot = false;


        if (subTreeRootNode == nullptr) {
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
            return true;
         }



       

        // Check if the current node's longest child path equals the level of its right-right grandchild.
        if (subTreeRootNode->longestChildPath == subTreeRootNode->rightNodePtr->rightNodePtr->longestChildPath) {
         
            Node* subTreeRootParent = subTreeRootNode->parentNodePtr;
         

            subTreeRootNodeRightChild = subTreeRootNode->rightNodePtr;

            if (!isTreeEmpty() && subTreeRootNode == root) {
                isRoot = true;
            }




            subTreeRootNode->rightNodePtr = subTreeRootNodeRightChild->leftNodePtr;
            if (subTreeRootNode->rightNodePtr) {

                subTreeRootNode->rightNodePtr->parentNodePtr = subTreeRootNode;
            }


            subTreeRootNodeRightChild->leftNodePtr = subTreeRootNode;





            subTreeRootNodeRightChild->longestChildPath++;

            subTreeRootNodeRightChild->leftNodePtr->parentNodePtr = subTreeRootNodeRightChild;


            subTreeRootNodeRightChild->rightNodePtr->parentNodePtr = subTreeRootNodeRightChild;
            subTreeRootNodeRightChild->parentNodePtr = subTreeRootParent;




            if ((!isTreeEmpty()) && (isRoot)) {
                setRoot(subTreeRootNodeRightChild);


            }
            
            else {

                if (subTreeRootParent) {

                    if (subTreeRootParent->leftNodePtr && subTreeRootParent->leftNodePtr == subTreeRootNodeRightChild->leftNodePtr) {
                        subTreeRootParent->leftNodePtr = subTreeRootNodeRightChild;
                    }
                    else {
                        subTreeRootParent->rightNodePtr = subTreeRootNodeRightChild;
                    }

                }

            }
            
            return false;

        }
        if (currentParentLookUp < maxParentLookUp && subTreeRootNode->parentNodePtr != nullptr) {
            currentParentLookUp++;
            return false;
        }
        else {
            return true;
        }
        
    }
}
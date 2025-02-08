#include "pch.h"

#include <gtest/gtest.h>
#include "BinarySearchTree.hpp"
#include "DynamicTypedValue.hpp"
#include "StringConverter.hpp"
#include <vector>
#include <memory>
#include <queue>
using EzCSCCRUDtoDBConverter::BinarySearchTree;

class BinarySearchTreeTest : public ::testing::Test {
protected:
    using RowType = std::vector<EzCSCCRUDtoDBConverter::DynamicTypedValue>;

    // Define the key extractor explicitly
    std::function<EzCSCCRUDtoDBConverter::DynamicTypedValue(const RowType&)> keyExtractor =
        [](const RowType& row) { return row[0]; };

    // Create the BinarySearchTree instance
    BinarySearchTree<RowType, decltype(keyExtractor)> tree{ keyExtractor };

    void SetUp() override {
        // Insert test data into the tree
        tree.insert(std::make_shared<RowType>(RowType{ EzCSCCRUDtoDBConverter::DynamicTypedValue(10) }));
        tree.insert(std::make_shared<RowType>(RowType{ EzCSCCRUDtoDBConverter::DynamicTypedValue(20) }));
        tree.insert(std::make_shared<RowType>(RowType{ EzCSCCRUDtoDBConverter::DynamicTypedValue(15) }));
        tree.insert(std::make_shared<RowType>(RowType{ EzCSCCRUDtoDBConverter::DynamicTypedValue(11) }));
        tree.insert(std::make_shared<RowType>(RowType{ EzCSCCRUDtoDBConverter::DynamicTypedValue(89) }));
        tree.insert(std::make_shared<RowType>(RowType{ EzCSCCRUDtoDBConverter::DynamicTypedValue(511) }));
        tree.insert(std::make_shared<RowType>(RowType{ EzCSCCRUDtoDBConverter::DynamicTypedValue(720) }));
        tree.insert(std::make_shared<RowType>(RowType{ EzCSCCRUDtoDBConverter::DynamicTypedValue(500) }));
        tree.insert(std::make_shared<RowType>(RowType{ EzCSCCRUDtoDBConverter::DynamicTypedValue(780) }));
    }
};

// Test: Is the tree empty after insertion?
TEST_F(BinarySearchTreeTest, IsTreeEmpty) {
    EXPECT_FALSE(tree.isTreeEmpty());
}

// Test: BST size is correct after insertion
TEST_F(BinarySearchTreeTest, BSTNodeCount) {
    EXPECT_EQ(tree.getBSTSize(), 9);
}



// Test: Print tree level order (check output format manually)
TEST_F(BinarySearchTreeTest, PrintLevelOrder) {
    testing::internal::CaptureStdout();
    tree.printLevelOrder();
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_NE(output.find("10"), std::string::npos);
    EXPECT_NE(output.find("20"), std::string::npos);
    EXPECT_NE(output.find("15"), std::string::npos);
    EXPECT_NE(output.find("11"), std::string::npos);
    EXPECT_NE(output.find("89"), std::string::npos);
    EXPECT_NE(output.find("511"), std::string::npos);
    EXPECT_NE(output.find("720"), std::string::npos);
}



TEST_F(BinarySearchTreeTest, ValidateParentChildRelationshipsIterative) {
    // Access the root node of the tree
    auto rootNode = tree.root;
    
    // Ensure the tree is not empty
    ASSERT_NE(rootNode, nullptr) << "The tree is empty. Cannot validate relationships.";

    // Use a queue for level-order traversal
    std::queue<decltype(rootNode)> nodeQueue;
    nodeQueue.push(rootNode);

    // Traverse the tree iteratively
    while (!nodeQueue.empty()) {
        auto currentNode = nodeQueue.front();
        nodeQueue.pop();
        // Check left child
        if (currentNode->leftNodePtr) {
            // Validate parent pointer of the left child
            ASSERT_EQ(currentNode->leftNodePtr->parentNodePtr, currentNode)
                << "Left child's parent pointer is incorrect.";

            // Add the left child to the queue
            nodeQueue.push(currentNode->leftNodePtr);
        }

        // Check right child
        if (currentNode->rightNodePtr) {
            // Validate parent pointer of the right child
            ASSERT_EQ(currentNode->rightNodePtr->parentNodePtr, currentNode)
                << "Right child's parent pointer is incorrect.";

            // Add the right child to the queue
            nodeQueue.push(currentNode->rightNodePtr);
        }

        // Extra validation for the parent-child relationship consistency
        if (currentNode->parentNodePtr) {
            ASSERT_TRUE(
                currentNode->parentNodePtr->leftNodePtr == currentNode ||
                currentNode->parentNodePtr->rightNodePtr == currentNode)
                << "Parent does not correctly claim the current node as a child.";
        }
    }
}
TEST_F(BinarySearchTreeTest, ValidateRootLevel) {
    auto rootNode = tree.root;
    EXPECT_EQ(rootNode->longestChildPath, 3);


}


TEST_F(BinarySearchTreeTest, ValidateRootKeyValue) {
    auto rootNode = tree.root;
    auto key =  tree.keyExtractor(*rootNode->containedValueObject);
 
    EXPECT_EQ(key.getValue<int>(), 89);
    auto rightChildKey = tree.keyExtractor(*rootNode->rightNodePtr->containedValueObject);
    EXPECT_EQ(rightChildKey.getValue<int>(), 511);
    auto rightLeftChildKey = tree.keyExtractor(*rootNode->rightNodePtr->leftNodePtr->containedValueObject);
    EXPECT_EQ(rightLeftChildKey.getValue<int>(), 500);
    auto rightRightChildKey = tree.keyExtractor(*rootNode->rightNodePtr->rightNodePtr->containedValueObject);
    EXPECT_EQ(rightRightChildKey.getValue<int>(), 720);
    auto rightRightRightChildKey = tree.keyExtractor(*rootNode->rightNodePtr->rightNodePtr->rightNodePtr->containedValueObject);
    EXPECT_EQ(rightRightRightChildKey.getValue<int>(), 780);
}


TEST_F(BinarySearchTreeTest, ValidateNodeLevels) {
    auto rootNode = tree.root;
  
    EXPECT_EQ(rootNode->longestChildPath, 3);

    EXPECT_EQ(rootNode->leftNodePtr->longestChildPath, 2);
    EXPECT_EQ(rootNode->leftNodePtr->leftNodePtr->longestChildPath, 1);

    EXPECT_EQ(rootNode->rightNodePtr->longestChildPath, 2);
    EXPECT_EQ(rootNode->rightNodePtr->leftNodePtr->longestChildPath, 1);
    EXPECT_EQ(rootNode->rightNodePtr->rightNodePtr->longestChildPath, 1);
    EXPECT_EQ(rootNode->rightNodePtr->rightNodePtr->rightNodePtr->longestChildPath, 1);
}


TEST_F(BinarySearchTreeTest, ValidateSkewPlusSplit) {
    auto rootNode = tree.root;

 
    EXPECT_EQ(rootNode->rightNodePtr->rightNodePtr->longestChildPath, 1);
    EXPECT_EQ(rootNode->rightNodePtr->rightNodePtr->rightNodePtr->longestChildPath, 1);
    auto rrKey = tree.keyExtractor(*rootNode->rightNodePtr->rightNodePtr->containedValueObject);
    EXPECT_EQ(rrKey.getValue<int>(), 720);
    auto rrrKey = tree.keyExtractor(*rootNode->rightNodePtr->rightNodePtr->rightNodePtr->containedValueObject);
    EXPECT_EQ(rrrKey.getValue<int>(), 780);
    tree.insert(std::make_shared<RowType>(RowType{ EzCSCCRUDtoDBConverter::DynamicTypedValue(705) }));
    rrKey = tree.keyExtractor(*rootNode->rightNodePtr->rightNodePtr->containedValueObject);
    EXPECT_EQ(rrKey.getValue<int>(), 720);
    EXPECT_EQ(rootNode->rightNodePtr->rightNodePtr->longestChildPath, 2);
    rrKey = tree.keyExtractor(*rootNode->rightNodePtr->rightNodePtr->leftNodePtr->containedValueObject);
    EXPECT_EQ(rrKey.getValue<int>(), 705);
    EXPECT_EQ(rootNode->rightNodePtr->rightNodePtr->leftNodePtr->longestChildPath, 1);
    rrKey = tree.keyExtractor(*rootNode->rightNodePtr->rightNodePtr->rightNodePtr->containedValueObject);
    EXPECT_EQ(rrKey.getValue<int>(), 780);
    EXPECT_EQ(rootNode->rightNodePtr->rightNodePtr->rightNodePtr->longestChildPath, 1);


}
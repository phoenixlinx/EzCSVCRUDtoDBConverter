
// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef BINARY_SEARCH_TREE_HPP
#define BINARY_SEARCH_TREE_HPP

#include <iostream>
#include <string>
#include <deque>
#include <cmath>
#include "../include/CSVparser.hpp"
#include "../include/StringConverter.hpp"
#include<CSVAnalyzer.hpp>
#include<CSVrow.hpp>



namespace EzCSCCRUDtoDBConverter {

	template <typename T, typename KeyExtractor>
	class BinarySearchTree {

		using Key = decltype(std::declval<KeyExtractor>()(std::declval<T&>()));
		struct Node;
	private:
		size_t maxParentLookUp;
		size_t currentParentLookUp;
		Node* tempRoot;
		Node* leftTempRoot;   // Left midpoint root
		Node* rightTempRoot;  // Right midpoint root
		inline void setRoot(Node* newRoot);
	protected:
		struct Node {


			size_t longestChildPath;
			bool hasDuplicate; //Signifies if there exist nodes with the same key stored in another data structure.
			Node* parentNodePtr;
			Node* leftNodePtr;
			Node* rightNodePtr;
			std::shared_ptr<T> containedValueObject; // The object/value held within this tree node.
			Node();
			Node(std::shared_ptr<T> containedValueObject);


		};

		Node* root;
		size_t bstNodeCount; // Total nodes in BST
		size_t treeHeight;// Height of BST
		KeyExtractor keyExtractor;  // Function to extract BST key value from containedValueObject
		//Incline function to reduce overhead since one call to insert can result to many calls to this function
		inline void updateTreeMetrics(size_t nodeInsertionHeight);
		void debugKeyType() const;
		inline bool fixLeftImbalance(Node* subTreeRootNode);
		inline bool fixRightImbalance(Node* subTreeRootNode);
		void printNodeDetails(Node* node);
		void reSetDeepestLevel();
		//Previous Nodes in height
		inline size_t calculateMaxNodesInHight(size_t previousNodesInHeight);
		

	public:
		static constexpr  unsigned int initialNodeLevel = 1;
		BinarySearchTree(KeyExtractor extractor);
		virtual ~BinarySearchTree();
		template <typename InputKey>
		void remove(const InputKey& searchKey, const std::string& csvPathDeletedNodes);
		void insert(std::shared_ptr<T> containedValueObject);
		template<typename InputKey>
		std::vector<std::shared_ptr<T>> search(const InputKey& searchKey);
		size_t getDeepestLevel() const;
		size_t getBSTSize() const;
		inline bool isTreeEmpty() const;
		void printInOrder();
		void printLevelOrder();
		void setLeftMidpoint();
		void setRightMidpoint();
		template<typename InputKey>
		inline void setTempRoot(const InputKey& searchKey);
	};

}
#include "../src/BinarySearchTree.tpp"

#endif 


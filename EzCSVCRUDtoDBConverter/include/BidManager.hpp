// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef BID_MANAGER_HPP
#define BID_MANAGER_HPP

#include <BinarySearchTree.hpp>
#include <Bid.hpp>
#include <CSVparser.hpp>
#include <StringConverter.hpp>
#include <CSVrow.hpp>
#include <memory>
#include <vector>
#include <string>
#include <map>

namespace EzCSCCRUDtoDBConverter {
    template <typename BinarySearchTree>
    class BidManager {
    private:
        BinarySearchTree& bst;
    public:
        // Constructor: Initializes the BidManager with a reference to the BST
        explicit BidManager(BinarySearchTree& bst);

        std::shared_ptr<std::vector<DynamicTypedValue>> getBid(BinarySearchTree& tree, CSVrow& csvRow);



        //TODO Implement backUpDeletedNode
    };
}
#include "../src/BidManager.tpp"

#endif

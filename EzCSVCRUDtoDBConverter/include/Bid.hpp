// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef BID_HPP
#define BID_HPP

#include <string>
using namespace std;

// BidId is the unique identifier for bid,
// and is used as the key in the Binary Search Tree.
struct Bid {
    int bidId;          // Unique identifier for the bid
    string title;       
    string fund;        
    unsigned int rowPos; // Row index of the bid in the CSV file
    double amount;      

     Bid();
};

#endif

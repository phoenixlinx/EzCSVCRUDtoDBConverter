// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#include "Bid.hpp"

// Constructor initializes numeric fields to 0 because they are not automatically set to a valid value.
Bid::Bid()
    : bidId(0), rowPos(0), amount(0.0) {
}

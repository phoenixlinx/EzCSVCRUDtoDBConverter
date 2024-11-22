#ifndef MENUUTILS_H
#define MENUUTILS_H

#include <string>
#include "PerformanceUtils.hpp"
#include "StringConverter.hpp"
#include "BinarySearchTree.hpp"

void displayInitialMenu();
void displayPrimaryMenu();
bool handleFileSelection(string& filePath, BinarySearchTree* tree);
bool handleCRUD(string& filePath, BinarySearchTree* tree);
#endif

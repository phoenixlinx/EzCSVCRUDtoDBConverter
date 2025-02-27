// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com
#ifndef MENUUTILS_H
#define MENUUTILS_H

#include <string>
#include <PerformanceUtils.hpp>
#include <StringConverter.hpp>
#include <BinarySearchTree.hpp>
#include <CSVMetadata.hpp>
#include <CSVAnalyzer.hpp>
#include <CSVRow.hpp>
#include <BidManager.hpp>
#include <SQLiteHandler.hpp>

namespace EzCSCCRUDtoDBConverter {

	void displayInitialMenu();
	void displayPrimaryMenu();
	void displayFileSelectionMenu();
	std::optional<string> handleFileSelection();
	template <typename BinarySearchTree>
	bool handleCRUD(const string& csvPath, BinarySearchTree& binarySearchTree, CSVrow& csvRow, const string& primaryKey);
}
#include "../src/MenuUtils.tpp" 
#endif

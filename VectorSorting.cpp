//============================================================================
// Name        : VectorSorting.cpp
// Author      : Paula Harris
// Version     : 1.0
// Copyright   : Copyright � 2023 SNHU COCE
// Description : Vector Sorting Algorithms
//============================================================================

#include <algorithm>
#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// define a structure to hold bid information
struct Bid {
	string bidId; // unique identifier
	string title;
	string fund;
	double amount;
	Bid() {
		amount = 0.0;
	}
};

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the bid information to the console (std::out)
 *
 * @param bid struct containing the bid info
 */
void displayBid(Bid bid) {
	cout << bid.bidId << ": " << bid.title << " | " << bid.amount << " | "
			<< bid.fund << endl;
	return;
}

/**
 * Prompt user for bid information using console (std::in)
 *
 * @return Bid struct containing the bid info
 */
Bid getBid() {
	Bid bid;

	cout << "Enter Id: ";
	cin.ignore();
	getline(cin, bid.bidId);

	cout << "Enter title: ";
	getline(cin, bid.title);

	cout << "Enter fund: ";
	cin >> bid.fund;

	cout << "Enter amount: ";
	cin.ignore();
	string strAmount;
	getline(cin, strAmount);
	bid.amount = strToDouble(strAmount, '$');

	return bid;
}

/**
 * Load a CSV file containing bids into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the bids read
 */
vector<Bid> loadBids(string csvPath) {
	cout << "Loading CSV file " << csvPath << endl;

	// Define a vector data structure to hold a collection of bids.
	vector<Bid> bids;

	// initialize the CSV Parser using the given path
	csv::Parser file = csv::Parser(csvPath);

	try {
		// loop to read rows of a CSV file
		for (size_t i = 0; i < file.rowCount(); i++) {

			// Create a data structure and add to the collection of bids
			Bid bid;
			bid.bidId = file[i][1];
			bid.title = file[i][0];
			bid.fund = file[i][8];
			bid.amount = strToDouble(file[i][4], '$');

			//cout << "Item: " << bid.title << ", Fund: " << bid.fund << ", Amount: " << bid.amount << endl;

			// push this bid to the end
			bids.push_back(bid);
		}
	} catch (csv::Error &e) {
		std::cerr << e.what() << std::endl;
	}
	return bids;
}

// FIXME (2a): Implement the quick sort logic over bid.title

/**
 * Partition the vector of bids into two parts, low and high
 *
 * @param bids Address of the vector<Bid> instance to be partitioned
 * @param begin Beginning index to partition
 * @param end Ending index to partition
 */
int partition(vector<Bid> &bids, int begin, int end) {
	//Use the middle bid title as the pivot
	string pivot = bids.at(begin + (end - begin) / 2).title;

	int low = begin;
	int high = end;

	while (true) {
		//Move low right while titles belong on left side of pivot
		while (bids.at(low).title < pivot) {
			++low;

	}

	//Move high left while titles belong on the right side
	//of the pivot
	while (bids.at(high).title > pivot) {
		--high;
	}
	//Stop when the indexes meet or cross
	if (low >= high) {
		return high;
	}
	//Swap bids that are on the wrong sides
	Bid tempBid = bids.at(low);
	bids.at(low) = bids.at(high);
	bids.at(high) = tempBid;

	++low;
	--high;
}
}

/**
 * Perform a quick sort on bid title
 * Average performance: O(n log(n))
 * Worst case performance O(n^2))
 *
 * @param bids address of the vector<Bid> instance to be sorted
 * @param begin the beginning index to sort on
 * @param end the ending index to sort on
 */
void quickSort(vector<Bid> &bids, int begin, int end) {
//A range with zero or one item is already sorted
if (begin >= end) {
	return;
}

int partitionIndex = partition(bids, begin, end);

//Sort the left half
quickSort(bids, begin, partitionIndex);

//Sort the right half
quickSort(bids, partitionIndex + 1, end);

}

// FIXME (1a): Implement the selection sort logic over bid.title

/**
 * Perform a selection sort on bid title
 * Average performance: O(n^2))
 * Worst case performance O(n^2))
 *
 * @param bid address of the vector<Bid>
 *            instance to be sorted
 */
void selectionSort(vector<Bid> &bids) {
int minIndex;
//Move through each position in the vector
for (int currentIndex = 0; currentIndex < static_cast<int>(bids.size()) - 1;
		++currentIndex) {

	minIndex = currentIndex;
	//Find the smallest title alphabetically in the unsorted
	//portion of the vector
	for (int nextIndex = currentIndex + 1;
			nextIndex < static_cast<int>(bids.size()); ++nextIndex) {

		if (bids.at(nextIndex).title < bids.at(minIndex).title) {

			minIndex = nextIndex;
		}
	}

	//Swap the smallest remaining bid into the current position
	if (minIndex != currentIndex) {
		Bid tempBid = bids.at(currentIndex);
		bids.at(currentIndex) = bids.at(minIndex);
		bids.at(minIndex) = tempBid;
	}
}
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
str.erase(remove(str.begin(), str.end(), ch), str.end());
return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char *argv[]) {

// process command line arguments
string csvPath;
switch (argc) {
case 2:
	csvPath = argv[1];
	break;
default:
	csvPath = "eBid_Monthly_Sales.csv";
}

// Define a vector to hold all the bids
vector<Bid> bids;

// Define a timer variable
clock_t ticks;

int choice = 0;
while (choice != 9) {
	cout << "Menu:" << endl;
	cout << "  1. Load Bids" << endl;
	cout << "  2. Display All Bids" << endl;
	cout << "  3. Selection Sort All Bids" << endl;
	cout << "  4. Quick Sort All Bids" << endl;
	cout << "  9. Exit" << endl;
	cout << "Enter choice: ";
	cin >> choice;

	switch (choice) {

	case 1:
		// Initialize a timer variable before loading bids
		ticks = clock();

		// Complete the method call to load the bids
		bids = loadBids(csvPath);

		cout << bids.size() << " bids read" << endl;

		// Calculate elapsed time and display result
		ticks = clock() - ticks; // current clock ticks minus starting clock ticks
		cout << "time: " << ticks << " clock ticks" << endl;
		cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;

		break;

	case 2:
		// Loop and display the bids read
		for (size_t i = 0; i < bids.size(); ++i) {
			displayBid(bids[i]);
		}
		cout << endl;

		break;

	case 3:
		//Start the timer before running selection sort
		ticks = clock();

		//Sort all bids alphabetically by title
		selectionSort(bids);

		//Calculate and display elapsed time
		ticks = clock() - ticks;

		cout << bids.size() << " bids sorted" << endl;
		cout << "time: " << ticks << " clock ticks" << endl;
		cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
		break;

	case 4:
		//Start the timer before running quickSort
		ticks = clock();

		//Only call quickSort if bids have been loaded
		if (!bids.empty()) {
			quickSort(bids, 0, bids.size() - 1);

		}

		//Calculate and display elapsed time
		ticks = clock() - ticks;

		cout << bids.size() << " bids sorted" << endl;
		cout << "time: " << ticks << " clock ticks" << endl;
		cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC
				<< " seconds" << endl;
		break;

	}
}

cout << "Good bye." << endl;

return 0;
}

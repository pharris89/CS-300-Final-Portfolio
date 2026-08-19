//============================================================================
// Name        : ProjectTwo.cpp
// Author      : Paula Harris
// Version     : 1.0
// Copyright   : Paula Harris
// Description : Project Two for CS-300
//============================================================================

//Covers input/output, reading the CSV, splitting lines, strings,
//prerequisite lists, uppercasing input, and general string processing
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;

//Lets one course hold a course number, course title, and zero or more
//prerequisites
struct Course {
	string courseNumber;
	string courseTitle;
	vector<string> prerequisites;
};
//Gives each tree node one course, a pointer to the left child and
//a pointer to the right child
struct Node {
	Course course;
	Node *left;
	Node *right;

	Node(Course aCourse) {
		course = aCourse;
		left = nullptr;
		right = nullptr;
	}
};

//Creating the binary search tree class
class BinarySearchTree {
private:
	Node *root;
	Node* insertNode(Node *node, Course course);
	Node* searchNode(Node *node, string courseNumber);
	void inOrder(Node *node);
	void clearNode(Node* node);

public:
    BinarySearchTree();
    ~BinarySearchTree();
    void Insert(Course course);
    Course* Search(string courseNumber);
    void PrintCourseList();
    void Clear();

};

//Creates a new binary search tree and starts it with an empty root
BinarySearchTree::BinarySearchTree() {
	root = nullptr;
}

//Recursively deletes every node in the tree
void BinarySearchTree::clearNode(Node* node) {
    if (node != nullptr) {
        clearNode(node->left);
        clearNode(node->right);
        delete node;
    }
}

//Removes all courses from the binary search tree
void BinarySearchTree::Clear() {
    clearNode(root);
    root = nullptr;
}

//Deletes all tree nodes when the program ends
BinarySearchTree::~BinarySearchTree() {
    Clear();
}

//Finds the correct location for a course and inserts it
//based on the course number
Node* BinarySearchTree::insertNode(Node *node, Course course) {
	if (node == nullptr) {
		//An empty location was found, so create a new node here
		return new Node(course);
	}

	//Smaller course numbers are placed in the left subtree
	if (course.courseNumber < node->course.courseNumber) {
		node->left = insertNode(node->left, course);
	}

	//Larger course numbers are placed in the right subtree
	else if (course.courseNumber > node->course.courseNumber) {
		node->right = insertNode(node->right, course);
	}

	else {
		//Course number already exists, so don't add a duplicate
		return node;
	}

	return node;
}

//Starts the insertion process at the root of the tree
void BinarySearchTree::Insert(Course course) {
	root = insertNode(root, course);
}

//Recursively searches the binary search tree for a course number
Node* BinarySearchTree::searchNode(Node *node, string courseNumber) {
	if (node == nullptr) {
		//Reached the end of a branch without finding the course
		return nullptr;
	}

	if (courseNumber == node->course.courseNumber) {
		//Matching course was found
		return node;
	}

	//Search the left subtree for a smaller course number
	if (courseNumber < node->course.courseNumber) {
		return searchNode(node->left, courseNumber);
	}

	//Search the right subtree for a larger course number
	return searchNode(node->right, courseNumber);
}

//Searches for a course and returns its course object if found
Course* BinarySearchTree::Search(string courseNumber) {
	Node *result = searchNode(root, courseNumber);

	if (result == nullptr) {
		return nullptr;
	}

	return &result->course;
}

//Traverses the tree in order so courses print from lowest to highest
void BinarySearchTree::inOrder(Node *node) {
	if (node != nullptr) {
		//Visit smaller course numbers first
		inOrder(node->left);

		//Print the current course
		cout << node->course.courseNumber << ", " << node->course.courseTitle
				<< endl;

		//Visit larger course numbers last
		inOrder(node->right);
	}
}

//Starts the ordered course list traversal at the root
void BinarySearchTree::PrintCourseList() {
	inOrder(root);
}

//Removes extra spaces before or after text read from the file
//or user input
string trim(string str) {
	size_t first = str.find_first_not_of(" \t\r\n");

	if (first == string::npos) {
		return "";
	}

	size_t last = str.find_last_not_of(" \t\r\n");

	return str.substr(first, last - first + 1);
}

//Converts course numbers to uppercase so searches aren't case-sensitive
string toUpperCase(string str) {
	transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
		return toupper(c);
	});

	return str;
}

//Opens the CSV file, validates the course data, creates course objects,
//and stores them in the binary search tree
bool loadCourses(string fileName, BinarySearchTree &courses) {
	ifstream file(fileName);

	if (!file.is_open()) {
		cout << "Error: File could not be opened." << endl;
		return false;
	}

	//Store the file data temporarily so prerequisites can be validated
	//before any courses are inserted into the tree
	vector<vector<string>> fileRecords;
	vector<string> courseNumbers;
	string line;

	//Read and validate each line
	while (getline(file, line)) {
		if (line.empty()) {
			continue;
		}

		stringstream ss(line);
		string token;
		vector<string> tokens;

		//Split the current CSV line into individual values
		while (getline(ss, token, ',')) {
			tokens.push_back(trim(token));
		}

		//Each line must have at least a course number and title
		if (tokens.size() < 2) {
			cout << "Error: Invalid course data." << endl;
			file.close();
			return false;
		}

		if (tokens[0].empty() || tokens[1].empty()) {
			cout << "Error: Course number or title is missing." << endl;
			file.close();
			return false;
		}

		tokens[0] = toUpperCase(tokens[0]);

		//Check for duplicate course numbers in the file
		if (find(courseNumbers.begin(), courseNumbers.end(), tokens[0])
				!= courseNumbers.end()) {
			cout << "Error: Duplicate course number found: " << tokens[0]
					<< endl;
			file.close();
			return false;
		}

		//Save the course number and complete record for later validation
		courseNumbers.push_back(tokens[0]);
		fileRecords.push_back(tokens);
	}

	file.close();

	//Validate that every prerequisite exists in the file
	for (vector<string> &tokens : fileRecords) {
		for (size_t i = 2; i < tokens.size(); i++) {
			string prerequisite = toUpperCase(trim(tokens[i]));

			if (prerequisite.empty()) {
				continue;
			}

			if (find(courseNumbers.begin(), courseNumbers.end(), prerequisite)
					== courseNumbers.end()) {
				cout << "Error: Prerequisite " << prerequisite
						<< " does not exist in the course file." << endl;
				return false;
			}
		}
	}

	//After all file data passes validation, create each course object
	//and add it to the binary search tree
	for (vector<string> &tokens : fileRecords) {
		Course course;

		course.courseNumber = toUpperCase(tokens[0]);
		course.courseTitle = tokens[1];

		for (size_t i = 2; i < tokens.size(); i++) {
			string prerequisite = toUpperCase(trim(tokens[i]));

			if (!prerequisite.empty()) {
				course.prerequisites.push_back(prerequisite);
			}
		}

		//Extra duplicate protection before inserting
		if (courses.Search(course.courseNumber) == nullptr) {
			courses.Insert(course);
		}
	}

	cout << "Courses loaded successfully." << endl;
	return true;
}

//Finds one course and displays its title and prerequisite information
void printCourseInformation(BinarySearchTree &courses, string courseNumber) {
	//Standardize the user's input before searching
	courseNumber = toUpperCase(trim(courseNumber));

	Course *course = courses.Search(courseNumber);

	if (course == nullptr) {
		cout << "Course not found." << endl;
		return;
	}

	cout << course->courseNumber << ", " << course->courseTitle << endl;

	if (course->prerequisites.empty()) {
		cout << "Prerequisites: None" << endl;
		return;
	}

	cout << "Prerequisites:" << endl;

	//Look up each prerequisite so both its number and
	//title can be displayed
	for (string prerequisiteNumber : course->prerequisites) {
		Course *prerequisite = courses.Search(prerequisiteNumber);

		if (prerequisite != nullptr) {
			cout << prerequisite->courseNumber << ", "
					<< prerequisite->courseTitle << endl;
		}
	}
}

//Controls the program menu and handles the user's menu selections
int main() {
	BinarySearchTree courses;
	//Track whether course data has been successfully loaded
	bool dataLoaded = false;

	string choice;
	string fileName;
	string courseNumber;

	cout << "Welcome to the course planner." << endl;

	//Continue displaying the menu until the user chooses to exit
	while (choice != "9") {
		cout << endl;
		cout << "1. Load Data Structure." << endl;
		cout << "2. Print Course List." << endl;
		cout << "3. Print Course." << endl;
		cout << "9. Exit" << endl;
		cout << endl;
		cout << "What would you like to do? ";

		getline(cin, choice);
		choice = trim(choice);

		//Option 1: Load and validate course data from a CSV file
		if (choice == "1") {
		    cout << "Enter the file name: ";
		    getline(cin, fileName);
		    fileName = trim(fileName);

		    //Clear any previously loaded course data before loading a new file
		    courses.Clear();
		    dataLoaded = false;

		    if (loadCourses(fileName, courses)) {
		        dataLoaded = true;
		    }
		}

		//Option 2: Print all courses in alphanumeric order
		else if (choice == "2") {
			if (!dataLoaded) {
				cout << "Please load the course data first." << endl;
			} else {
				cout << endl;
				cout << "Here is a sample schedule:" << endl;
				cout << endl;

				courses.PrintCourseList();
			}
		}

		//Option 3: Search for and display one course
		else if (choice == "3") {
			if (!dataLoaded) {
				cout << "Please load the course data first." << endl;
			} else {
				cout << "What course do you want to know about? ";
				getline(cin, courseNumber);

				printCourseInformation(courses, courseNumber);
			}
		}

		//Option 9: Exit the program
		else if (choice == "9") {
			cout << "Thank you for using the course planner!" << endl;
		}

		//Handle any menu choice that isn't valid
		else {
			cout << choice << " is not a valid option." << endl;
		}
	}

	return 0;
}

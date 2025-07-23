// HUSEYIN EREN YILDIZ - 31047 - HOMEWORK2

#include <iostream>
#include "Quadtree.h"
#include <fstream>
#include <sstream>
#include <vector>


void print(const vector<string>& vec) {

    static int count = 0;  //  Static variable to keep track of the number of times this function is called

    if (count % 2 == 0 && count !=0 ) {   // Add extra spacing after every second call (except the first call)
        cout<<endl;
        cout<<endl;
    }

    if (vec.empty()) {  // Check if the vector is empty
        cout << "<None>";
    } else {

        string result = vec[0];  // Initialize the result string with the first element of the vector

        for (int i = 1; i < vec.size(); ++i) {   // Append the rest of the elements, separated by commas
            result += ", " + vec[i];
        }
        cout << result;
    }

    if (count % 2 == 0) {  // Add a newline after printing if the count is even
        cout<<endl;
    }
    count++;
}


bool openFile(ifstream& file, const string& filename) {
// This function opens a file and checks for success

    file.open(filename);

    if (!file) {   // Check if the file failed to open
        cerr << "Cannot open the file: " << filename << endl;
        return false;
    }
    return true;
}


void readFirstLine(ifstream& file, int& maximum_x, int& maximum_y) {
// This function reads the first line and get maximum_x and maximum_y

    string line;
    getline(file, line);
    istringstream firstLine(line);
    firstLine >> maximum_x >> maximum_y;  //Extracting two integers from the line and store them in maximum_x and maximum_y
}


void importCities(ifstream& file, Quadtree& tree, int maximum_x, int maximum_y) {
// This function loads city data from a file into a Quadtree

    string line;
    while (getline(file, line)) {  // Loop to read each line of the file
        string nameOfCities;
        int x, y;               // Variables to store the city's coordinates

        istringstream iss(line);
        iss >> nameOfCities >> x >> y;

        // Creating a coordinate_points object to represent the city
        coordinate_points points(nameOfCities, x, y);

        if (x <= maximum_x && y <= maximum_y) {  // Checking if the city's coordinates are within the allowed bounds
            tree.insert(points);
        }
    }
}


void handleQueries(const string& filename, Quadtree& tree) {
// This function process queries from the query file
    ifstream file;
    if (!openFile(file, filename)) {
        return;
    }

    string line;
    while (getline(file, line)) {
        int value1, value2, value3;  // Variables to hold query values
        char comma;                  // To handle the commas in the file format
        istringstream iss(line);
        iss >> value1 >> comma >> value2 >> comma >> value3;    // Extract three integers separated by commas from the query line

        tree.search_cities(value1, value2, value3);     // Search the Quadtree for cities matching the query criteria
        print(tree.city_name);
        print(tree.path);

        // Clear the results for the next query
        tree.city_name.clear();
        tree.path.clear();
    }
    file.close();
}


// Main function
int main() {

    // Open the cities file
    ifstream file;
    if (!openFile(file, "cities.txt")) {
        return 1;
    }

    // Read maximum_X and maximum_Y from the first line
    int maximum_X, maximum_Y;
    readFirstLine(file, maximum_X, maximum_Y);

    // Initialize Quadtree and load city data
    Quadtree tree(nullptr);
    importCities(file, tree, maximum_X, maximum_Y);
    file.close();

    // Print the tree
    tree.pretty_print();

    // Process queries from the queries file
    handleQueries("queries0.txt", tree);

    return 0;
}
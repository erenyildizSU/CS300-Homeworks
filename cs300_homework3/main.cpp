#include <iostream>
#include <string>
#include <fstream>
#include <vector>

#ifndef CS300_HOMEWORK3_HASHTABLE_H
#define CS300_HOMEWORK3_HASHTABLE_H


using namespace std;

//HashTable class definition
class HashTable {

public:
    explicit HashTable( const string & notFound = "ITEM_NOT_FOUND", int size = 101 );
    const string & find( const string & x ) const;//method to find an element in the hash table
    void insert( const string & x );//method to insert an element into the hash table
    void makeEmpty( );//method to clear the hash table
    enum EntryType { ACTIVE, EMPTY, DELETED }; //enum to define the state of an entry

private:
    struct HashEntry {
        string element;//the stored element
        EntryType info; //the state of the entry
        HashEntry(const string & e = string(), EntryType i = EMPTY )
                :element( e ), info( i ) {}
    };

    vector<HashEntry> array; //the hash table
    int currentSize; //current number of elements in the hash table
    int nextPrime( int );//helper method to find the next prime number
    bool isPrime( int ); //helper method to check if a number is prime
    const string ITEM_NOT_FOUND;//default value for not found
    bool isActive( int currentPos ) const;//helper method to check if a position is active
    int findPos( const string & x ) const;//helper method to find the position of an element
    void rehash();
};

#endif //HW3_HASHTABLE_H

//hashTable constructor
HashTable::HashTable(const string & notFound, int size):ITEM_NOT_FOUND( notFound ), array( nextPrime( size )){
    makeEmpty();
}

//hash function to compute hash values for strings
int hashFunction(const string& rleString, int tableSize) {
    int asciiSum = 0;
    for (char ch : rleString) {
        asciiSum += static_cast<int>(ch);//summing ASCII values of characters
    }
    return asciiSum % tableSize;
}

//helper method to check if a number is prime
bool HashTable::isPrime(int n) {
    if (n <= 1)
        return false; // Numbers <= 1 are not prime
    if (n <= 3)
        return true; // 2 and 3 are prime
    if (n % 2 == 0 || n % 3 == 0)
        return false; // Eliminate multiples of 2 and 3

    // Check for factors starting from 5, incrementing by 6 (5, 11, 17, ...)
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    }
    return true;
}


// Helper method to find the next prime number greater than or equal to n
int HashTable::nextPrime(int n) {
    if (n <= 2)
        return 2; // The smallest prime number is 2
    if (n % 2 == 0)
        n++; // Start with the next odd number if n is even

    while (!isPrime(n)) {
        n += 2; // Increment by 2 to check only odd numbers
    }

    return n;
}

// Helper method to check if a position in the hash table is active
bool HashTable::isActive(int currentPos) const {
    if (currentPos < 0 || currentPos >= array.size()) {
        return false; // Ensure the position is within bounds
    }
    return (array[currentPos].info == ACTIVE);
}


//helper method to find the position of an element in the hash table
int HashTable::findPos(const string &x) const {
    int currentPos = hashFunction(x, array.size()); // Compute initial hash position
    int initialPos = currentPos; // Keep track of the starting position

    do {
        if (array[currentPos].info == EMPTY || array[currentPos].element == x) {
            break; // Stop if an empty slot is found or the element matches
        }
        currentPos = (currentPos + 1) % array.size(); // Move to the next slot with wrap-around
    } while (currentPos != initialPos); // Ensure we don't loop infinitely

    return currentPos;
}


//method to find an element in the hash table
const string& HashTable::find(const string& x) const {
    int currentPos = findPos(x);

    // Return the element if the position is active, otherwise return ITEM_NOT_FOUND
    return (isActive(currentPos)) ? array[currentPos].element : ITEM_NOT_FOUND;
}


void HashTable::insert(const string& x) {
    // Compute the initial hash position
    int currentPos = hashFunction(x, array.size());

    // Find the appropriate position for the new element
    while (array[currentPos].info == ACTIVE) {
        if (array[currentPos].element == x) {
            return; // Element already exists, no insertion needed
        }
        currentPos = (currentPos + 1) % array.size(); // Linear probing
    }

    // Insert the element at the determined position
    array[currentPos] = HashEntry(x, ACTIVE);
    currentSize++;

    // Check if rehashing is needed
    if (currentSize > array.size() / 2) {
        // Save the current array
        vector<HashEntry> oldArray = array;

        // Resize the hash table to the next prime size
        int newSize = nextPrime(array.size() * 2);
        array = vector<HashEntry>(newSize, HashEntry());
        currentSize = 0;

        // Reinsert all active elements into the new table
        for (int i = 0; i < oldArray.size(); i++) {
            if (oldArray[i].info == ACTIVE) {
                // Recompute position in the new table
                int newPos = hashFunction(oldArray[i].element, array.size());
                while (array[newPos].info == ACTIVE) {
                    newPos = (newPos + 1) % array.size(); // Linear probing
                }
                array[newPos] = HashEntry(oldArray[i].element, ACTIVE);
                currentSize++;
            }
        }
    }
}


void HashTable::makeEmpty() {
    for (int i = 0; i < array.size(); i++) {
        array[i].info = EMPTY; // Mark each entry as empty
    }
}

void HashTable::rehash() {
    // Step 1: Create a new larger array with the next prime size
    int newSize = nextPrime(2 * array.size());
    vector<HashEntry> newTable(newSize, HashEntry()); // Default-initialize to EMPTY

    // Step 2: Transfer active elements from the old table to the new table
    for (int i = 0; i < array.size(); ++i) {
        if (array[i].info == ACTIVE) {
            const string& element = array[i].element; // Get the element
            int hashPos = hashFunction(element, newSize); // Compute hash position

            // Handle collisions using linear probing
            while (newTable[hashPos].info == ACTIVE) {
                hashPos = (hashPos + 1) % newSize;
            }

            newTable[hashPos] = HashEntry(element, ACTIVE); // Place element in new table
        }
    }

    // Step 3: Replace the old table with the new table
    array = newTable; // Copy new table into the existing array
}


//function to convert a file's binary content into RLE
// Function to convert a file's binary content into RLE
string convert_to_rle(const string& concfile) {
    // Normalize the binary input
    string normalized = "";
    for (int i = 0; i < concfile.size(); i++) {
        if (concfile[i] == '1' || concfile[i] == '0') {
            normalized += concfile[i];
        }
    }

    if (normalized.empty()) {
        return ""; // Return an empty RLE string for invalid input
    }

    // Build the RLE string manually
    string rle = "";
    char current = normalized[0];
    int count = 0;

    for (int i = 0; i < normalized.size(); i++) {
        if (normalized[i] == current) {
            count++; // Increment the count for the current character
        } else {
            // Append the run length and character to the RLE string
            rle += to_string(count);
            rle += (current == '0' ? 'B' : 'W');
            current = normalized[i]; // Update the current character
            count = 1; // Reset the count for the new character
        }
    }

    // Append the final run length and character
    rle += to_string(count);
    rle += (current == '0' ? 'B' : 'W');

    return rle;
}


// Helper function to read a file and concatenate its contents
string readFile(const string& fileName) {
    ifstream file(fileName);
    string line, concfile;

    if (!file) {
        cout << "Cannot open the file!" << endl; // No unintended space
        return ""; // Return empty string on error
    }

    while (getline(file, line)) {
        concfile += line; // Concatenate file contents
    }
    return concfile;
}

// Function to handle image insertion
void handleInsertion(HashTable& hashTable) {
    string input;

    while (true) {
        cout << "Enter image number to insert into the hash table (or 'query' to continue): ";
        cin >> input;

        cout << endl;

        if (input == "query") break;

        string fileName = "image" + input + ".txt";
        string fileContents = readFile(fileName);

        if (!fileContents.empty()) {
            string rle = convert_to_rle(fileContents);
            hashTable.insert(rle);
            cout << "Image " << input << " inserted into the hash table." << endl;
        }
    }
}

// Function to handle image querying
void handleQuery(HashTable& hashTable) {
    string input;

    while (true) {
        cout << "Enter image number to query (or 'exit' to quit): ";
        cin >> input;

        if (input == "exit") break;

        string fileName = "query" + input + ".txt";
        string fileContents = readFile(fileName);

        if (!fileContents.empty()) {
            string rle = convert_to_rle(fileContents);

            if (hashTable.find(rle) == "ITEM_NOT_FOUND") {
                cout << endl;
                cout << "No match for the image with encoding: " << rle << endl;
            } else {
                cout << endl;
                cout << "RLE String for query" << input << ".txt found in hash table." << endl;
                for (size_t i = 0; i < fileContents.size(); ++i) {
                    if (fileContents[i] == '\r') {
                        cout << endl;
                    } else {
                        cout << fileContents[i];
                    }
                }
                cout << endl;
            }
        }
    }
}

// Main function
int main() {
    HashTable hashTable("ITEM_NOT_FOUND", 101); // Initialize hash table

    handleInsertion(hashTable); // Handle image insertion
    handleQuery(hashTable);     // Handle image querying

    cout << endl;
    cout << "Exiting the program!" << endl;
    return 0;
}
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#ifndef HW3_HASHTABLE_H
#define HW3_HASHTABLE_H

using namespace std;

//HashTable class 
class HashTable {

public:
    explicit HashTable( const string & notFound = "ITEM_NOT_FOUND", int size = 101 );
    const string & find( const string & x ) const;
    void insert( const string & x );
    void makeEmpty( );
    enum EntryType { ACTIVE, EMPTY, DELETED }; //enum to define the state of an entry

private:
    struct Hash_Entry {

        string data;                  // Holds the actual stored element in the hash table
        EntryType status;             // Represents the state of the entry (e.g., ACTIVE, EMPTY)
        Hash_Entry(const string & d = string(), EntryType s = EMPTY )
                :data( d ), status( s ) {}
    };

    vector<Hash_Entry> hashTable; //the hash table
    int elementCount;         // number of elements in the hash table
    const string ITEM_NOT_FOUND;
    bool checkPrime( int );
    int get_NextPrime( int );
    int findPos (const string & key ) const;
    bool isItActive (int currentPos ) const;
    void rehash();
};

#endif //HW3_HASHTABLE_H

//hashTable constructor
HashTable::HashTable(const string & notFound, int size):ITEM_NOT_FOUND( notFound ), hashTable( get_NextPrime( size )){
    makeEmpty();
}


int hashFunction(const string& rleString, int tableSize) {
    int hashVal = 0;

    for (int i = 0; i < rleString.length(); i++) {
        hashVal += (rleString[i] * (i + 1)); // Weighted sum: character value multiplied by its 1-based position
    }

    hashVal = (hashVal * hashVal) ^ (hashVal >> 3); // Square and XOR for additional randomness
    return abs(hashVal) % tableSize; // Ensure non-negative hash value fits within the table size
}


bool HashTable::checkPrime(int n) {
// This function checks if a number is prime

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


int HashTable::get_NextPrime(int n) {
// This function finds the next prime number greater than or equal to n
    if (n <= 2)
        return 2; // The smallest prime number is 2

    if (n % 2 == 0)
        n++; // Start with the next odd number if n is even

    while (!checkPrime(n)) {
        n += 2; // Increment by 2 to check only odd numbers
    }

    return n;
}


int HashTable::findPos(const string & key) const {
//

    int curPosition = hashFunction(key, hashTable.size()); // Compute initial hash position
    int startPosition  = curPosition;                                  // Record the initial position for reference

    do {
        // Check if the slot is empty or the key matches the data in the slot
        if (hashTable[curPosition].status == EMPTY || hashTable[curPosition].data == key) {
            break; // Exit the loop if a match is found or an empty slot is encountered
        }
        curPosition = (curPosition + 1) % hashTable.size(); // Move to the next slot, wrapping around
    } while (curPosition != startPosition ); // Loop until we return to the starting position

    return curPosition;    // Return the position of the key or the next empty slot
}

bool HashTable::isItActive(int currentPos) const {
    // Check if the position in the hash table is within valid bounds

    if (currentPos < 0 || currentPos >= hashTable.size()) {
        return false; // Return false if the position is out of bounds
    }
    return (hashTable[currentPos].status == ACTIVE);  // Check if the slot is active
}


const string& HashTable::find(const string& x) const {
// This function finds the element x in the hash table

    int position  = findPos(x);

    // Return the data if the position is active; otherwise, return ITEM_NOT_FOUND
    return (isItActive(position )) ? hashTable[position ].data : ITEM_NOT_FOUND;
}


void HashTable::insert(const string& x) {

    // Determine the starting position for the key in the hash table
    int position = hashFunction(x, hashTable.size());

    // Search for an available slot for the new key
    while (hashTable[position].status == ACTIVE) {
        if (hashTable[position].data == x) {
            return; // Element already exists, no need to insert it again
        }
        position = (position + 1) % hashTable.size(); // Move to the next slot (linear probing)
    }

    // Insert the key into the hash table at the identified position
    hashTable[position] = Hash_Entry(x, ACTIVE);  // Mark the position as active and store the key
    elementCount++;

    // Check if rehashing is needed (if the table is more than half full)
    if (elementCount > hashTable.size() / 2) {
        rehash();  // Resize and rehash the table for better performance
    }
}


void HashTable::makeEmpty() {

    for (int i = 0; i < hashTable.size(); i++) {
        // Reset the state of the current slot to EMPTY
        hashTable[i].status = EMPTY; // This removes any data and marks the slot as unused
    }
}

void HashTable::rehash() {
    // Create a new larger hashTable with the next prime size
    int newSize = get_NextPrime(2 * hashTable.size());
    vector<Hash_Entry> newTable(newSize, Hash_Entry()); // Default-initialize to EMPTY

    // Transfer active elements from the old table to the new table
    for (int i = 0; i < hashTable.size(); ++i) {
        if (hashTable[i].status == ACTIVE) {
            const string& element = hashTable[i].data; // Get the element
            int hashPos = hashFunction(element, newSize); // Compute hash position

            // Handle collisions using linear probing
            while (newTable[hashPos].status == ACTIVE) {
                hashPos = (hashPos + 1) % newSize;
            }

            newTable[hashPos] = Hash_Entry(element, ACTIVE); // Place element in new table
        }
    }

    // Replace the old table with the new table
    hashTable = newTable; // Copy new table into the existing array
}

using namespace std;

string convert_to_rle(const string& binaryContent) {
// This function converts a file's binary content into RLE

    // Normalize the binary input by filtering out non-binary characters
    string normalized = "";  // Holds only '1' and '0' characters from the input
    for (int i = 0; i < binaryContent.size(); i++) {
        if (binaryContent[i] == '1' || binaryContent[i] == '0') {
            normalized += binaryContent[i];
        }
    }

    // If the normalized content is empty, return an empty RLE string
    if (normalized.empty()) {
        return ""; // Return an empty RLE string for invalid input
    }

    // Build the RLE string by counting consecutive binary characters
    string rle = "";                  // The resulting RLE string
    char current = normalized[0];     // Tracks the current binary character ('0' or '1')
    int count = 0;                    // Tracks the number of consecutive occurrences of the current character

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

    // Return the final RLE result
    return rle;
}


string readFile(const string& fileName) {
// This function reads a file and concatenate its contents

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

void handleInsertion(HashTable& hashTable) {
// This function processes the insertion of images into the hash table

    string userInput;

    while (true) {
        cout << "Enter image number to insert into the hash table (or 'query' to continue): ";
        cin >> userInput;

        cout<<endl;

        if (userInput == "query") break;  // Exit the loop if the user enters "query"

        // Construct the file name for the image based on the user's input
        string fileName = "image" + userInput + ".txt";
        string fileContents = readFile(fileName);                   // Read the contents of the specified file


        if (!fileContents.empty()) {

            // Convert the binary file contents into an RLE (Run-Length Encoded) string
            string rleData = convert_to_rle(fileContents);
            hashTable.insert(rleData );
            cout << "Image " << userInput << " inserted into the hash table." << endl;
        }
    }
}


void handleQuery(HashTable& hashTable) {
// This function processes image queries in the hash table

    string userInput;

    while (true) {
        // Prompt the user to enter an image number to query or exit the program
        cout << "Enter image number to query (or 'exit' to quit): ";
        cin >> userInput;

        if (userInput == "exit") break;

        string fileName = "query" + userInput + ".txt";
        string fileContents = readFile(fileName);

        if (!fileContents.empty()) {
            // Convert the binary query file contents into an RLE string
            string rleData  = convert_to_rle(fileContents);

            if (hashTable.find(rleData ) == "ITEM_NOT_FOUND") {  // Check if the RLE string exists in the hash table

                cout<<endl;
                cout << "No match for the image with encoding: " << rleData << endl;

            } else {    // If a match is found, print the corresponding file contents
                cout << endl;
                cout << "RLE String for query" << userInput << ".txt found in hash table." << endl;
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
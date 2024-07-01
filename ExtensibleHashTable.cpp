// Steven Ao
// 301464127

#include "ExtensibleHashTable.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdexcept>

using namespace std;

// Default constructor fo table
ExtensibleHashTable::ExtensibleHashTable() {
    buckets = 4;
    globalDepth = 1;
    directory.resize(2); // Create 2 entries

    for (int i = 0; i < directory.size(); i++) {
        directory[i] = make_shared<Bucket>(globalDepth, buckets); // Creates the buckets
    }
}

// Constructor with bucket size
ExtensibleHashTable::ExtensibleHashTable(int num) {
    buckets = num;
    globalDepth = 1;
    directory.resize(2); // Create 2 entries

    for (int i = 0; i < directory.size(); i++) {
        directory[i] = make_shared<Bucket>(globalDepth, buckets); // Creates the buckets
    }
}

// Copy constructor that deep copies
ExtensibleHashTable::ExtensibleHashTable(const ExtensibleHashTable &table) {
    buckets = table.buckets;
    globalDepth = table.globalDepth;
    directory.resize(table.directory.size());
    for (int i = 0; i < directory.size(); i++) {
        directory[i] = make_shared<Bucket>(*table.directory[i]);
    }
}

// Overloaded assignment operator that shallow copies
ExtensibleHashTable& ExtensibleHashTable::operator=(const ExtensibleHashTable &table) {
    if (this == &table) {
        return *this;
    }
    buckets = table.buckets;
    globalDepth = table.globalDepth;
    directory = table.directory;
    return *this;
}

// Destructor that cleans memory allocation
ExtensibleHashTable::~ExtensibleHashTable() {
    // automatically handled by shared_ptr
}

// Searches for the key from the table
bool ExtensibleHashTable::find(int key) const {
    int index = hashValue(key); // Obtain hash
    return directory[index]->search(key); // Searches the bucket
}

// Inserts the key to the table
void ExtensibleHashTable::insert(int key) {
    int index = hashValue(key); // Obtain hash
    auto bucket = directory[index]; // Obtain the bucket

    if (!bucket->insert(key)) { // Bucket is full
        if (globalDepth > bucket->depth()) { // Global depth is greater than local depth
            split(index, key); // Make new bucket
        }
        else if (globalDepth == bucket->depth()) { // Depth are same, double the size of directory
            doubleDirectory();
            split(index, key);
        }
        else { // Insert failed
            throw runtime_error("Insertion Failed");
        }
    }
}

// Removes a key from the table
bool ExtensibleHashTable::remove(int key) {
    int index = hashValue(key); // Obtain hash
    return directory[index]->remove(key); // Removes from bucket
}

// Outputs the extendible hash index
void ExtensibleHashTable::print() const {
    std::vector<void*> printedPointers; // To not output duplicate pointers

    // Outputs the pointers value in hex
    for (int i = 0; i < directory.size(); i++)  {
        void* bucketPtr = directory[i].get();
        cout << dec << i << ": " << hex << bucketPtr << " --> ";

        // Prevents from requesting the print of duplicate pointer bucket
        if (std::find(printedPointers.begin(), printedPointers.end(), bucketPtr) != printedPointers.end()) {
            cout << endl;
            continue;
        }

        // Prints the keys from the bucket
        directory[i]->print();
        
        // Tracks used pointer/buckets
        printedPointers.push_back(bucketPtr);
    }
}

// Helper Functions

// Computes the hash value of key
int ExtensibleHashTable::hashValue(int key) const {
    int divisor = pow(2, globalDepth);
    return key % divisor;
}

// Splits a bucket when overflows
void ExtensibleHashTable::split(int index, int key) {
    // Increases the local depth of both bucket
    auto firstBucket = directory[index];
    firstBucket->incrDepth();
    auto secondBucket = make_shared<Bucket>(firstBucket->depth(), buckets);

    // Collect all keys from firstBucket and count duplicates
    vector<int> redistribute;
    int duplicateCounter = 0;
    int firstKey = -1;
    for (int i = 0; i < buckets; i++) {
        int poppedKey = firstBucket->pop();
        if (firstKey == -1) {
            firstKey = poppedKey; // Store the first key encountered
        } else if (poppedKey == firstKey) {
            duplicateCounter++; // Increment duplicate counter
        }
        redistribute.push_back(poppedKey); // Storing all keys
    }
    if (duplicateCounter == buckets - 1) { // Check if bucket is full of duplicates
        throw runtime_error("Error occured inserting into bucket of duplicates");
    }
    
    // Determine the largest index for reassigning the pointer
    int maxIndex = 0;
    for (int key : redistribute) {
        int newIndex = secondBucket->localHashValue(key);
        if (newIndex > maxIndex) {
            maxIndex = newIndex;
        }
    }
    directory[maxIndex] = secondBucket; // Changes pointer

    redistribute.push_back(key); // Insert new key to be organized

    for (int key: redistribute) { // Redistributes keys to buckets based on their hash
        int index = secondBucket->localHashValue(key);
        directory[index]->insert(key);
    }

    // Update other pointers to point to secondBucket
    for (int i = directory.size() - 1; i > secondBucket->localHashValue(key); i--) {
        if (directory[i] == firstBucket) {
            directory[i] = secondBucket; 
        }
    }
}

// Doubles the size of directory
void ExtensibleHashTable::doubleDirectory() {
    int prev = directory.size();
    directory.resize(prev * 2); // Increase directory size

    // Reassign pointers
    for (int i = 0; i < prev; i++) {
        directory[i + prev] = directory[i];
    }
    globalDepth++;
}

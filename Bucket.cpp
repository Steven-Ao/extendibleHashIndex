// Steven Ao
// 301464127

#include "Bucket.h"
#include <iostream>
#include <algorithm>
#include <cmath>

using namespace std;

// Initializes bucket given depth and size
Bucket::Bucket(int depth, int size) : localDepth(depth), size(size) {}

// Inserts key into bucket if possible
bool Bucket::insert(int key) {
    if (keys.size() >= size) { // Checks if there is room in bucket
        return false;
    }
    else { // Inserts key in bucket
        keys.push_back(key);
        return true;
    }
}

// Searches for key within the bucket
bool Bucket::search(int key) const {
    auto result = find(keys.begin(), keys.end(), key); // Searches through key vector
    if (result != keys.end()) { // Key is found
        return true;
    }
    else { // No key found
        return false;
    }
}

// Removes a key from the bucket if possible
bool Bucket::remove(int key) {
    auto result = find(keys.begin(), keys.end(), key); // Searches through key vector
    if (result != keys.end()) {  // Key is found
        keys.erase(result); // Removes key
        return true;
    }
    else { // No key found
        return false;
    }
}

// Prints the keys inside the bucket and local depth
void Bucket::print() const {
    cout << "[";
    if (keys.size() == 0) { // For empty bucket
        for (int i = 0; i < size; i++) {
            cout << "-";
            if (i + 1 < size) {
                cout << ",";
            }
        }
    } else {
        for (int i = 0; i < size; i++) {
            if (i >= keys.size()) { // For empty spots
                cout << "-";
            } else {
                cout << dec << keys[i];  // Outputs key
            }
            if (i + 1 < size) {
                cout << ",";
            }
        }
    }
    cout << "] (" << localDepth << ")" << endl;
}

// Retrieves the local depth of the bucket
int Bucket::depth() const {
    return localDepth;
}

// Increases the local depth of the bucket
void Bucket::incrDepth() {
    localDepth++;
}

// Removes and returns the key in the bucket
int Bucket::pop() { 
    int key = keys.front();
    keys.erase(keys.begin());
    return key;
}

// Comptues hash value using local depth of bucket
int Bucket::localHashValue(int key) {
    int divisor = pow(2, localDepth);
    return key % divisor;
}
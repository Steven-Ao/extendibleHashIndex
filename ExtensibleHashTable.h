// Steven Ao
// 301464127

#ifndef EXTENSIBLEHASHTABLE_H
#define EXTENSIBLEHASHTABLE_H

#include "Bucket.h"
#include <vector>
#include <memory>

using namespace std;

class ExtensibleHashTable {
    public: 
        // Constructors
        ExtensibleHashTable();
        ExtensibleHashTable(int num);
        ExtensibleHashTable(const ExtensibleHashTable &table); // Copy constructor
        ExtensibleHashTable& operator=(const ExtensibleHashTable &table); // Overloaded assignment operator
        ~ExtensibleHashTable(); // Destructor
        bool find(int key) const; // Searches for key, returns true if found, false otherwise
        void insert(int key); // Inserts key into table
        bool remove(int key); // Removes a key, returns true if done, false otherwise
        void print() const; // Outputs the table

        private:
            int buckets; // Size of buckets
            int globalDepth; // Depth of indexes
            vector<shared_ptr<Bucket>> directory; // Stores indexes
            int hashValue(int key) const; // Computes hash
            void split(int index, int key); // Creates a bucket and redistributes keys
            void doubleDirectory(); // Doubles the directory size
};
#endif
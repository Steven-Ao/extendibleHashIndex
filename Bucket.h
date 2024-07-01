// Steven Ao
// 301464127

#ifndef BUCKET_H
#define BUCKET_H

#include <vector>

using namespace std;

class Bucket {
    public:
        // Constructor: Initializes the bucket with depth and size
        Bucket(int localDepth, int size);

        // Searches for a key, Returns true if found, false otherwise
        bool search(int key) const;

        // Inserts a key, Returns true if successful, false otherwise
        bool insert(int key);

        // Removes a key, Returns true if successful, false otherwise
        bool remove(int key);

        // Prints the contents of the bucket and local depth
        void print() const;

        // Returns the local depth
        int depth() const;

        // Increaes the local depth
        void incrDepth();

        // Removes and return key from bucket
        int pop();

        // Computes hash using local depth
        int localHashValue(int key);

    private:
        vector<int> keys; // To store keys in the bucket
        int localDepth; // Track depth of bucket
        int size; // Max size of bucket
};

#endif
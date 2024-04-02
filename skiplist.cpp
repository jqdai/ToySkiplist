#include<iostream>
#include<vector>

using std::vector;


template<typename K, typename V>
class Node {
    private:
        K key;
        V value;
        int level;
        vector<Node*> next;
    public:
};


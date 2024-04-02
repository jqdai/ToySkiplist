#include<iostream>
#include<vector>

using std::vector;
using std::ofstream;
using std::ifstream;


template<typename K, typename V>
class Node {
    private:
        K key;
        V value;
        int level;
        vector<Node<K, V>*> next;
    public:
        Node() {};
        Node(K k, V v, int l) : key(k), value(v), level(l) {}
        ~Node() {};
        K getKey() const { return key; }
        V getValue() const { return value; }
        int getLevel() const { return level; }
        void setValue(V v) { value = v;}
        void setLevel(int l);
};

template<typename K, typename V>
class SkipList {
    private:
        int max_level;
        int list_level;
        int length;
        Node<K, V>* head;
        ofstream file_writer;
        ifstream file_reader;
    public:
        SkipList(int ml): max_level(ml), level(0);
        ~SkipList();
        int randomLevel();
        Node<K, V>* createNode(K k, V v, int l);
        int size() { return length; }
};


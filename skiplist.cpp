#include<iostream>
#include<string>
#include<vector>
#include<mutex>

#define STORE_FILE "store/dumpFile"

using std::vector;
using std::ofstream;
using std::ifstream;


template<typename K, typename V>
class Node {
    private:
        K key;
        V value;
    public:
        Node() {};
        Node(K k, V v, int l) : key(k), value(v), level(l) {
            forward = vector<Node<K, V>*>(level+1);
        }
        ~Node() {};
        K getKey() const { return key; }
        V getValue() const { return value; }
        void setValue(V v) { value = v;}
        int level;
        vector<Node<K, V>*> forward;
};


template<typename K, typename V>
class SkipList {
    private:
        int max_level;
        int list_level;
        int length;
        Node<K, V>* header;
        ofstream file_writer;
        ifstream file_reader;
        std::mutex mtx;
    public:
        SkipList(int);
        ~SkipList();
        int randomLevel();
        Node<K, V>* createNode(K, V, int);
        int insertElement(K, V v);
        V searchElement(K);
        void deleteElement(K);
        void dumpFile();
        void loadFile();
        int size() { return length; }
        void clear(Node<K, V>*)
};


template<typename K, typename V>
SkipList<K, V>::SkipList(int ml) : max_level(ml), list_level(0), length(0) {
    K k, V v;
    header = new Node<K, V>(k, v, max_level);
}


template<typename K, typename V>
SkipList<K, V>::~SkipList(){
    if (file_reader.is_open()) {
        file_reader.close();
    }
    if (file_writer.is_open()) {
        file_writer.close();
    }
    if(header->forward[0] != nullptr) {
        clear(header->forward[0]);
    }
    delete header;
}


template<typename K, typename V>
int SkipList<K, V>::randomLevel() {
    int k = 1;
    while (rand() % 2) {
        k++;
    }
    return (k < max_level) ? k : max_level;
}


template<typename K, typename V>
Node<K, V>* SkipList<K, V>::createNode(K k, V v, int l) {
    Node<k, V>* node = new Node<K, V>(k, v, l);
    return node;
}


template<typename K, typename V>
int SkipList<K, V>::insertElement(K k, V v) {
    mtx.lock();
    Node<K, V>* current = header;
    vector<Node<K, V>*> update(max_level+1, nullptr);
    for (int i = list_level; i >= 0; i--) {
        while(current->forward[i] != NULL && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];

    if (current != NULL && current->get_key() == k) {
        std::cout << "key: " << k << ", exists" << std::endl;
        mtx.unlock();
        return 1;
    }
    // Generate a random level for node
    int random_level = get_random_level();

    // create new node with random level generated 
    Node<K, V>* insertedNode = create_node(k, v, random_level);

    // If random level is greater thar skip list's current level, initialize update value with pointer to header
    if (random_level > list_level) {
        for (int i = list_level+1; i < random_level+1; i++) {
            update[i] = header;
        }
        list_level = random_level;
    }
    
    for (int i = 0; i <= random_level; i++) {
        insertedNode->forward[i] = update[i]->forward[i];
        update[i]->forward[i] = insertedNode;
    }
    std::cout << "Successfully inserted key:" << k << ", value:" << v << std::endl;
    length ++;
    mtx.unlock();
    return 0;
}


template<typename K, typename V>
V SkipList<K, V>::searchElement(K k) {
    Node<K, V>* current = header;
    vector<Node<K, V>*> update(max_level+1, nullptr);
    for (int i = list_level; i >= 0; i--) {
        while(current->forward[i] != NULL && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];

    if (current != NULL && current->get_key() == k) {
        std::cout << "Found key: " << k << ", value: " << current->get_value() << std::endl;
        return current->getValue();
    }

    V v;
    return v;
}


template<typename K, typename V>
void SkipList<K, V>::deleteElement(K k) {
    mtx.lock();
    Node<K, V>* current = header;
    vector<Node<K, V>*> update(max_level+1, nullptr);
    for (int i = list_level; i >= 0; i--) {
        while(current->forward[i] != NULL && current->forward[i]->get_key() < key) {
            current = current->forward[i];
        }
        update[i] = current;
    }
    current = current->forward[0];
    if (current != NULL && current->get_key() == k) {
       
        // start for lowest level and delete the current node of each level
        for (int i = 0; i <= list_level; i++) {

            // if at level i, next node is not target node, break the loop.
            if (update[i]->forward[i] != current) 
                break;

            update[i]->forward[i] = current->forward[i];
        }

        // Remove levels which have no elements
        while (list_level > 0 && _header->forward[list_level] == 0) {
            list_level --; 
        }

        std::cout << "Successfully deleted key "<< key << std::endl;
        delete current;
        length --;
    }
    mtx.unlock();
    return;
}


template <typename K, typename V>
void SkipList<K, V>::clear(Node<K, V>* curr)
{
    if (curr->forward[0] != nullptr) {
        clear(curr->forward[0]);
    }
    delete curr;
}




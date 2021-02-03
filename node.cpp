#include <iostream>
#include <vector>

#include "node.h"

using namespace std;

namespace Tree {
    /**
      * Create an Node.
      */
    Node::Node(unsigned int capacity)
        : capacity_(capacity), key_({}), type_(TREE_ROOT_LEAF), child_(new Node* [capacity + 1]), degree_(0)
        {
        for (unsigned int i = 0; i < capacity + 1; i++) {this->child_[i] = NULL;}
        }


    /**
      * Destructor: free all memory associated with a given Node object.
      * Invoked by the system.
      */
    Node::~Node() {
        for (unsigned int i = 0; i < capacity_; i++) {
            delete[] child_[i];
        }
        delete[] child_;
    }


    /**
      * Get the branching factor... capacity of the key list
      * @return the integer branching factor.
      */
    int Node::get_capacity() {
        return static_cast <int> (this->capacity_);
    }


    /**
      * Get full list of keys
      * @return the full list of integer keys as vector
      */
    vector<int> Node::get_keylist() {
        return key_;
    }


    /**
      * Get a key from the list
      * @return key of a specific index from the key list.
      */
    int Node::get_key(int index) {
        if (!key_.empty()) {
            return key_[index];
        }
        else {
            return NULL;
        }
    }


    /**
      * Add a key to the list with specific order
      * @return index of the inserted key.
      */
    int Node::add_key(int key) {
        int size = get_keylist().size();
        if (size == 0) {
            this->key_.insert(this->key_.begin(), key);
            return 0;
        }
        else {
            unsigned int i;
            for (i = 0; i < get_keylist().size(); i++) {
                if (key < get_keylist()[i]) {
                    this->key_.insert(this->key_.begin() + i, key);
                    return i;
                }
            }
            this->key_.insert(this->key_.end(), key);
            return i;
        }
    }


    /**
      * Delete a key from the list with specific order
      * @return true if successfully delete, else false.
      */
    void Node::del_key(int key) {
        for (unsigned int i = 0; i < key_.size(); i++) {
            if (key == key_[i]) {
                key_.erase(key_.begin() + i);
                return;
            }
        }
        cout << "key not in tree" << endl;
        return;
    }


    /**
      * Get a list of Node pointers to its children
      * @return lists of pointers to children.
      */
    Node** Node::get_child() {
        return child_;
    }


    /**
      * Set a child to the list of children at the specific index
      */
    void Node::set_child(Node* child, int index) {
        this->child_[index] = child;
    }


    void Node::del_child(int index) {
        for (int i = index; i < key_.size(); i++) {
            this->child_[index] = this->child_[index + 1];
        }
        this->child_[key_.size()] = NULL;
    }

    /**
      * Get a pointer to the neighbor node
      * @return pointer to the neighbor node if exist.
      */
    Node* Node::get_next() {
        return child_[capacity_];
    }


    /**
      * Set neighbor node as next node
      */
    void Node::set_next(Node* node) {
        this->child_[capacity_] = node;
    }


    /**
      * Get the type of current node
      * @return type of currrent node.
      */
    TreeNodeType Node::get_type() {
        return type_;
    }


    /**
      * Set type of current node as specific type
      */
    void Node::set_type(TreeNodeType type) {
        this->type_ = type;
    }


    /**
      * Get the degree of current node
      * @return degree of current node.
      */
    int Node::get_degree() {
        return degree_;
    }

    /**
      * Set type of current node as specific degree
      */
    void Node::set_degree(int degree) {
        this->degree_ = degree;
    }

    /**
      * Check whether the node is full
      */
    bool Node::isFull() {
        if (key_.size() >= capacity_) {
            return true;
        }
        else { 
            return false; 
        }
    }

    /**
      * Check whether the node is empty
      */
    bool Node::isEmpty() {
        if (key_.empty()) {
            return true;
        }
        else {
            return false;
        }
    }
}
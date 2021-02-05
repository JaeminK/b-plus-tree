#include <iostream>
#include <vector>

#include "node.h"

using namespace std;

namespace Tree
{
    /** Create an Node.
      */
    Node::Node(unsigned int capacity)
        : capacity_(capacity), key_({}), type_(TREE_ROOT_LEAF), child_(new Node *[capacity + 1])
    {
        for (unsigned int i = 0; i < capacity + 1; i++)
        {
            this->child_[i] = NULL;
        }
    }

    /** Destructor: free all memory associated with a given Node object.
      * Invoked by the system.
      */
    Node::~Node()
    {
        delete[] child_;
    }

    /** Get the branching factor... capacity of the key list
      * @return the integer branching factor.
      */
    int Node::get_capacity()
    {
        return static_cast<int>(this->capacity_);
    }

    /** Get full list of keys
      * @return the full list of integer keys as vector
      */
    vector<int> Node::get_keylist()
    {
        return key_;
    }

    /** Get a key from the list
      * @return key of a specific index from the key list.
      */
    int Node::get_key(int index)
    {
        if (index < key_.size())
        {
            return key_[index];
        }
        else
        {
            return NULL;
        }
    }

    int Node::get_keysize()
    {
        return this->key_.size();
    }

    /** Add a key to the list with ascending order
      * @return index of where the inserted key have been placed.
      */
    int Node::add_key(int key)
    {
        int size = key_.size();
        if (size == 0)
        {
            this->key_.insert(this->key_.begin(), key);
            return 0;
        }
        else
        {
            unsigned int i;
            for (i = 0; i < key_.size(); i++)
            {
                if (key < get_keylist()[i])
                {
                    this->key_.insert(this->key_.begin() + i, key);
                    return i;
                }
            }
            this->key_.insert(this->key_.end(), key);
            return i;
        }
    }

    /** Delete a key from the list with ascending order
      * if key was not found, print "key not in tree"
      */
    void Node::del_key(int key)
    {
        for (unsigned int i = 0; i < key_.size(); i++)
        {
            if (key == key_[i])
            {
                key_.erase(key_.begin() + i);
                return;
            }
        }
        cout << "key not in tree!" << endl;
        return;
    }

    /** Get a list of Node pointers to its children
      * @return lists of pointers to children.
      */
    Node **Node::get_child()
    {
        return child_;
    }

    /** Set a child to the list of children at the specific index
      */
    void Node::set_child(Node *child, int index)
    {
        this->child_[index] = child;
    }

    /** Delete a child from the list of children at the specific index
      */
    void Node::del_child(int index)
    {
        for (unsigned int i = index; i < key_.size(); i++)
        {
            this->child_[index] = this->child_[index + 1];
        }
        this->child_[key_.size()] = NULL;
    }

    /** Copy contents from other node, without copying the actual address
      */
    void Node::copy_child(Node *node)
    {
        this->capacity_ = node->get_capacity();
        this->key_ = node->get_keylist();
        this->type_ = node->get_type();
        this->child_ = node->get_child();
    }

    /** Get a pointer to the neighbor node
      * @return pointer to the neighbor node.
      */
    Node *Node::get_next()
    {
        return child_[capacity_];
    }

    /** Set input node as next node
      */
    void Node::set_next(Node *node)
    {
        this->child_[capacity_] = node;
    }

    /** Get the type of current node
      * @return type of currrent node.
      */
    TreeNodeType Node::get_type()
    {
        return type_;
    }

    /** Set type of current node as specific type
      */
    void Node::set_type(TreeNodeType type)
    {
        this->type_ = type;
    }

    /** Check whether the node is full
      * @return true if key size == capacity, else false
      */
    bool Node::isFull()
    {
        if (key_.size() >= capacity_)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    /** Check whether the node is empty
      * @return true if key size == 0, else false
      */
    bool Node::isEmpty()
    {
        if (key_.empty())
        {
            return true;
        }
        else
        {
            return false;
        }
    }
} // namespace Tree
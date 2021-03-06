#pragma once
#include <vector>

using namespace std;

namespace Tree
{
    enum TreeNodeType
    {
        TREE_LEAF,
        TREE_INTERNAL,
        TREE_ROOT_INTERNAL,
        TREE_ROOT_LEAF
    };

    class Node
    {
    public:
        Node(unsigned int capacity);
        ~Node();
        int get_capacity();
        vector<int> get_keylist();
        int get_key(int index);
        int get_keysize();
        int add_key(int key);
        void del_key(int key);
        Node **get_child();
        void set_child(Node *child, int index);
        void del_child(int index);
        void copy_child(Node *node);
        Node *get_next();
        void set_next(Node *node);
        TreeNodeType get_type();
        void set_type(TreeNodeType type);
        bool isFull();
        bool isEmpty();

    private:
        unsigned int capacity_;
        vector<int> key_;
        TreeNodeType type_;
        Node **child_;
    };
} // namespace Tree
#include <iostream>
#include <vector>
#include <iomanip>

#include "node.h"
#include "b-plus-tree.h"

using namespace std;

using Tree::Node;


/**	************************************************************
INITIAL INPUT	: Root node pointer, integer key to insert
RECURSIVE INPUT	: proper child node pointer from parent node.
OPERATION		: Recursively dive into proper child.
				 When "child node" is found to be overflow, 
				 rearrange nodes based on proper cases, 
				 from bottom to top.
************************************************************* */
void insert_node(Node* node, int key) {
	if (node->get_type() == TREE_ROOT_LEAF) {		// inserting when I'm at the root-leaf node
		node->add_key(key);

		if (node->isFull()) {
			insert_arrange(node);	// I'm root-tree, and I'm full, arrange the tree..			// [[CASE 1]] ROOT-LEAF node is FULL
		}
	}
	else if (node->get_type() == TREE_LEAF) {		// inserting when I'm at the leaf
		node->add_key(key);
	}
	else {											// inserting when I'm at root-internal node or internal node
		int i = 0;
		for (int keys : node->get_keylist()) {			// find index of proper child to dive into
			if (key < keys) {
				break;
			}
			i++;
		}
		insert_node(node->get_child()[i], key);			// recursively dive into proper child
		if (node->get_child()[i]->isFull()) {													// [[CASE 3]] Child node is FULL.
			insert_arrange(node);	// I'm not full, but child is full. arrange the tree..		// [[CASE 3 - 1]] child node is LEAF node..			
		}																						// [[CASE 3 - 2]] child node is INTERNAL node..
		if (node->get_type() == TREE_ROOT_INTERNAL && node->isFull()) {
			insert_arrange(node);	// my child is not full, but I'm full. arrange the tree..	// [[CASE 2]] ROOT - INTERNAL node is FULL
		}
	}
}												


/**	************************************************************
INITIAL INPUT	: Root node pointer, integer key to delete
RECURSIVE INPUT	: proper child node pointer from parent node.
OPERATION		: Recursively dive into proper child.
				 When "child node" is found to be empty,
				 rearrange nodes based on proper cases,
				 from bottom to top.
************************************************************* */
Node* delete_node(Node* node, int key) {
	if (node->get_type() == TREE_LEAF || node->get_type() == TREE_ROOT_LEAF) {
		node->del_key(key);							// deleting when I'm at the root-leaf node or leaf node
	}
	else {											// deleting when I'm at the root-internal node or internal node
		int i = 0;
		for (int keys : node->get_keylist()) {			// find index of proper child to dive into
			if (key < keys) {
				break;
			}
			i++;
		}
		delete_node(node->get_child()[i], key);			// recursively dive into proper child
		key_update(node, key);							// if my key is deleted at the leaf, update to remove conflict
		/*
		*				3
		*			/		|
		*			2		5						Ex... delete 3.. all it need is to update root key 3 to 4
		*		/	|	   /	|	
		*	   1	2	   3,4	5,6
		*/
		if (node->get_child()[i]->isEmpty()) {													// [[CASE 1]] Child node is LEAF node..
			delete_arrange(node);	// I'm not empty, but child is empty. arrange the tree..	// [[CASE 2]] Child node is INTERNAL node..
		}
		if (node->get_type() == TREE_ROOT_INTERNAL && node->isEmpty()) {	// I'm at ROOT_INTERNAL and I"m EMPTY!!		
			// update root pointer to my first child... 
			// since when my key is empty, It means I only have one child
			node = node->get_child()[0];	
		}
	}
	return node;								// return node pointer to eventually return proper root pointer
}



/**	************************************************************
INPUT		: node pointer where overflow happened
OPERATION	: decompose overflow node depending on each case.
************************************************************* */
void insert_arrange(Node* node) {
	int capacity = node->get_capacity();
	int divider = capacity / 2;

	// CASE 1	..	ROOT-LEAF node is FULL
	if (node->get_type() == TREE_ROOT_LEAF) {
		/*	┌───-┐  ...  capacity = 3... divider = 1
		*	│ 1 2 3 │ << ... # of keys = capacity ... FULL!
		*	└───-┘
		*/
		Node* child1 = new Node(capacity);				// create left child node
		child1->set_type(TREE_LEAF);
		Node* child2 = new Node(capacity);				// create right child node
		child2->set_type(TREE_LEAF);

		/*		┌───-┐
		*		│ 1 2 3 │ <<
		*		└───-┘
		*	┌──┐┌──┐
		*	│    ││    │
		*	└──┘└──┘
		*/
		for (int i = 0; i < divider; i++) {
			/*		┌──-┐
			*		│ 2 3 │ <<
			*		└──-┘
			*	┌─-┐┌──┐
			*	│ 1 ││    │
			*	└─-┘└──┘
			*/
			child1->add_key(node->get_key(0));
			node->del_key(node->get_key(0));
		}
		for (unsigned int j = 0; j < node->get_keylist().size(); j++) {
			/*		┌──-┐
			*		│ 2 3 │ <<
			*		└──-┘
			*	┌─-┐┌──-┐
			*	│ 1 ││ 2 3 │
			*	└─-┘└──-┘
			*/
			child2->add_key(node->get_key(j));
		}
		for (unsigned int k = 1; k < node->get_keylist().size(); k++) {
			/*		┌─-┐
			*		│ 2 │ <<
			*		└─-┘
			*	┌─-┐┌──-┐
			*	│ 1 ││ 2 3 │
			*	└─-┘└──-┘
			*/
			node->del_key(node->get_key(1));
		}

		/*		┌─-┐
		*		│ 2 │ <<
		*		├─-┤
		*	┌─-┐┌──-┐
		*	│ 1 ││ 2 3 │
		*	└─-┘└──-┘
		*/
		node->set_child(child1, 0);
		node->set_child(child2, 1);

		child1->set_next(child2);		// set next

		node->set_type(TREE_ROOT_INTERNAL);
		node->set_degree(1);
		return;


	}

	// CASE 2	..	ROOT - INTERNAL node is FULL
	else if (node->get_type() == TREE_ROOT_INTERNAL && node->isFull()) {
		/*			┌──-─┐  ...  capacity = 3... divider = 1
		*			│ 2 3 4 │ << ... # of keys = capacity ... FULL!
		*		 ┌-┴─┬┬-┴─┐
		*	┌─-┤┌─-┤├─-┐├─-┐
		*	│ 1 ││ 2 ││ 3 ││ 4 │
		*	└─-┘└─-┘└─-┘└─-┘
		*/
		Node* child3 = new Node(capacity);
		child3->set_type(TREE_INTERNAL);
		child3->set_degree(node->get_degree());
		Node* child4 = new Node(capacity);
		child4->set_type(TREE_INTERNAL);
		child4->set_degree(node->get_degree());

		/*  ┌─-┐					┌─-┐
		*	│	 │	┌──-─┐		│	 │
		*	└─-┘	│ 2 3 4 │ <<	└─-┘
		*		 ┌-┴─┬┬-┴─┐
		*	┌─-┤┌─-┤├─-┐├─-┐
		*	│ 1 ││ 2 ││ 3 ││ 4 │
		*	└─-┘└─-┘└─-┘└─-┘
		*/

		for (int i = 0; i < divider; i++) {
			/*  ┌─-┐					┌─-┐
			*	│ 2 │	  ┌──-┐		│	 │
			*	├─-┤	  │ 3 4 │ <<	└─-┘
			*	 | 	 └┐-┴┬┬-┴─┐
			*	┌─-┤┌─-┤├─-┐├─-┐
			*	│ 1 ││ 2 ││ 3 ││ 4 │
			*	└─-┘└─-┘└─-┘└─-┘
			*/
			child3->add_key(node->get_key(0));
			node->del_key(node->get_key(0));
			child3->set_child(node->get_child()[i], i);
		}
		child3->set_child(node->get_child()[divider], divider);

		for (int j = divider+1; j < capacity; j++) {
			/*  ┌─-┐					┌─-┐
			*	│ 2 │	  ┌-─--┐		│ 4 │
			*	├─-┤	  │  3  │ <<	├┬-┘
			*	 | 	 └┐-┴┬┬-┴─┐-┘│
			*	├─-┤├─-┤├─-┤├─-┤
			*	│ 1 ││ 2 ││ 3 ││ 4 │
			*	└─-┘└─-┘└─-┘└─-┘
			*/
			child4->add_key(node->get_key(1));
			node->del_key(node->get_key(1));
			child4->set_child(node->get_child()[j], j-divider-1);
		}
		child4->set_child(node->get_child()[capacity], capacity - divider - 1);

		for (int k = 0; k < capacity+1; k++) {
			/*  ┌─-┐					 ┌─-┐
			*	│ 2 │	   ┌─-┐		 │ 4 │
			*	├─-┤	   │ 3 │ <<	 ├┬-┘
			*	 |	 └┐  └─-┘ ┌──┘|
			*	├─-┐├─-┐┌─-┤┌─-┤
			*	│ 1 ││ 2 ││ 3 ││ 4 │
			*	└─-┘└─-┘└─-┘└─-┘
			*/
			node->set_child(nullptr, k);
		}

		/*			   ┌─-┐
		*			   │ 3 │	<<
		*			 ┌┴─-┴┐
		*		┌─-┐		 ┌─-┐
		*		│ 2 │		 │ 4 │
		*		├─-┤		 ├─-┤
		*	┌─-┐┌─-┐┌─-┐┌─-┐
		*	│ 1 ││ 2 ││ 3 ││ 4 │
		*	└─-┘└─-┘└─-┘└─-┘
		*/
		node->set_child(child3, 0);
		node->set_child(child4, 1);
		node->set_degree(node->get_degree() + 1);
		return;
	}
	// CASE 3	..	Child node is FULL..
	else if (node->get_type() == TREE_INTERNAL || node->get_type() == TREE_ROOT_INTERNAL) {
		int parent_size = node->get_keylist().size();
		int divider = capacity / 2;
		int overflow;
		Node** child = node->get_child();
		for (overflow = 0; overflow < parent_size; overflow++) {
			if (child[overflow]->isFull()) {
				break;
			}
		}
		int split_key = child[overflow]->get_key(divider);

		// CASE 3-1	..	child node is LEAF node..
		if (child[overflow]->get_type() == TREE_LEAF) {
			/*		┌─-┐  ...  capacity = 3... divider = 1
			*		│ 2 │ << ... child node type... LEAF!
			*		├─-┤  ... child # of keys = capacity ... child is FULL!
			*	┌─-┐┌───-┐
			*	│ 1 ││ 2 3 4 │
			*	└─-┘└───-┘
			*/
			Node* child5 = new Node(capacity);
			child5->set_type(TREE_LEAF);
			child5->set_degree(child[overflow]->get_degree());
			int index = node->add_key(split_key);

			/*		┌──-┐ 
			*		│ 2 3 │ <<	... split_key = 3, index = 1
			*		├──-┤
			*	┌─-┐┌───-┐┌─-┐
			*	│ 1 ││ 2 3 4 ││   │
			*	└─-┘└───-┘└─-┘
			*/

			for (int i = capacity; i > index + 1; i--) {
				node->set_child(node->get_child()[i - 1], i);	// make space for new node
			}
			/*		┌──-┐
			*		│ 2 3 │ <<
			*		├──-┼───┐
			*	┌─-┐┌───-┐┌─-┐
			*	│ 1 ││ 2 3 4 ││   │
			*	└─-┘└───-┘└─-┘
			*/
			node->set_child(child5, index + 1);

			for (int j = divider; j < capacity; j++) {
				/*		┌──-┐
				*		│ 2 3 │ <<
				*		├──-┼─┐
				*	┌─-┐┌─-┐┌──-┐
				*	│ 1 ││ 2 ││ 3 4 │
				*	└─-┘└─-┘└─-─┘
				*/
				child5->add_key(child[overflow]->get_key(divider));
				child[overflow]->del_key(child[overflow]->get_key(divider));
			}

			child5->set_next(child[overflow]->get_next());		// set next
			child[overflow]->set_next(child5);
		}

		// CASE 3-2	..	child node is INTERNAL node..
		else if (child[overflow]->get_type() == TREE_INTERNAL) {
			/*		┌─-┐  ...  capacity = 3... divider = 1
			*		│ 2 │ << ... child node type... TREE_INTERNAL!
			*		├─-┤  ... child # of keys = capacity ... child is FULL!
			*	┌─-┐┌───-┐
			*	│ 1 ││ 3 4 5 │
			*	├─-┤├┬─-┬┤
			*/
			Node* child6 = new Node(capacity);
			child6->set_type(TREE_INTERNAL);
			child6->set_degree(child[overflow]->get_degree());
			int index = node->add_key(split_key);
			child[overflow]->del_key(split_key);

			/*		┌─-─┐
			*		│ 2 4 │ << ... split_key = 4, index = 1
			*		├─-─┤ 
			*	┌─-┐┌──-┐┌─-┐
			*	│ 1 ││ 3 5 ││   │
			*	├─-┤├┬-┬┤└─-┘
			*/

			for (int i = capacity; i > index + 1; i--) {
				node->set_child(node->get_child()[i - 1], i);	// make space for new node
			}
			/*		┌─-─┐
			*		│ 2 4 │ <<
			*		├─-─┼──┐
			*	┌─-┐┌──-┐┌─-┐
			*	│ 1 ││ 3 5 ││   │
			*	├─-┤├┬-┬┤└─-┘
			*/
			node->set_child(child6, index + 1);

			for (int j = divider; j < capacity-1; j++) {
				/*		┌─-─┐
				*		│ 2 4 │ << 
				*		├─-─┼──┐
				*	┌─-┐┌─--┐┌─-┐
				*	│ 1 ││ 3  ││ 5 │
				*	├─-┤├┬┬┤└─-┘
				*/
				child6->add_key(child[overflow]->get_key(divider));
				child[overflow]->del_key(child[overflow]->get_key(divider));
			}

			for (int k = divider; k < capacity; k++) {
				/*		┌─-─┐
				*		│ 2 4 │ <<
				*		├─-─┼──┐
				*	┌─-┐┌─-┐┌─-┐
				*	│ 1 ││ 3 ││ 5 │
				*	├─-┤├─-┤├─-┤
				*/
				child6->set_child(child[overflow]->get_child()[k + 1], k - divider);
				child[overflow]->set_child(nullptr, k + 1);
			}
		}
		return;
	}
	else {
		cout << "Why error!!!" << endl;
		exit(1);
	}
}


/**	************************************************************
INPUT		: node pointer where underflow happened
OPERATION	: merge underflow node depending on each case.
************************************************************* */
void delete_arrange(Node* node) {
	if (node->get_type() == TREE_ROOT_INTERNAL || node->get_type() == TREE_INTERNAL) {
		int parent_size = node->get_keylist().size();
		if (parent_size == 0) {
			return;
		}
		int underflow;
		Node** child = node->get_child();
		for (underflow = 0; underflow < parent_size; underflow++) {
			if (child[underflow]->isEmpty()) {
				break;
			}
		}
		int tmp = check_side(node, underflow);
		
		// CASE 1	..	Child node is LEAF node..
		if (child[underflow]->get_type() == TREE_LEAF) {
			if (tmp == 1) {			// when left adjacent child has more than one key
				/*	    ┌───-┐
				*	    │ 3  4  │ <<  ... child node type... LEAF!
				*	    ├─┬─-┤	... child # of keys = 0 ... child is Empty!
				* ┌──-┐┌-┐┌─-┐
				* │ 1 2 ││ ││ 4 │
				* └──-┘└-┘└─-┘
				*/
				Node* leftchild = child[underflow - 1];
				int shift_key = leftchild->get_keylist().back();

				/*	   ┌───-┐
				*	   │ 2  4  │ <<
				*	   ├─┬─-┤
				* ┌─-┐┌─-┐┌──-┐
				* │ 1 ││ 2 ││ 4 5 │
				* └─-┘└─-┘└──-┘
				*/
				node->del_key(node->get_key(underflow - 1));
				child[underflow]->add_key(shift_key);
				node->add_key(shift_key);
				leftchild->del_key(shift_key);
			}
			else if (tmp == 2) {			// when right adjacent child has more than one key
				/*	   ┌───-┐
				*	   │ 3  4  │ << 
				*	   ├─┬─-┤
				* ┌─-┐┌─-┐┌──-┐
				* │ 1 ││   ││ 4 5 │
				* └─-┘└─-┘└──-┘
				*/
				if (underflow == 0) {		// when left-most child is empty
					Node* rightchild = child[underflow + 1];
					int shift_key = rightchild->get_keylist().front();

					node->del_key(node->get_key(underflow));
					rightchild->del_key(shift_key);
					child[underflow]->add_key(shift_key);
					node->add_key(rightchild->get_keylist().front());
				}
				else {
					/*	   ┌───-┐
					*	   │ 3  5  │ <<
					*	   ├─┬─-┤
					* ┌─-┐┌─-┐┌─-┐
					* │ 1 ││ 4 ││ 5 │
					* └─-┘└─-┘└─-┘
					*/
					Node* rightchild = child[underflow + 1];
					int shift_key = rightchild->get_keylist().front();

					node->del_key(node->get_key(underflow-1));
					rightchild->del_key(shift_key);
					child[underflow]->add_key(shift_key);
					node->add_key(rightchild->get_keylist().front());

				}
			}
			else {							// when both left and right adjacent child has less than one key
				if (underflow == parent_size) {		// when right-most child is empty
					/*	   ┌───-┐
					*	   │ 2  3  │ <<
					*	   ├─┬─-┤
					* ┌─-┐┌─-┐┌─-┐
					* │ 1 ││ 2 ││   │
					* └─-┘└─-┘└─-┘
					*/
					child[underflow - 1]->set_next(child[underflow]->get_next());
					node->del_child(underflow);
					node->del_key(node->get_key(underflow - 1));
				}
				else {
					/*	   ┌───-┐
					*	   │ 2  3  │ <<
					*	   ├─┬─-┤
					* ┌─-┐┌─-┐┌─-┐
					* │ 1 ││   ││ 3 │
					* └─-┘└─-┘└─-┘
					*/
					for (int i = underflow; i < parent_size; i++) {
						node->set_child(child[i + 1], i);
					}
					node->del_child(parent_size);
					node->del_key(node->get_key(0));
				}
			}
		}

		// CASE 2	..	Child node is INTERNAL node..
		else if (child[underflow]->get_type() == TREE_INTERNAL) {
			if (tmp == 1) {					// when left adjacent child has more than one key
				/*	     ┌───-┐
				*	     │ 5  7  │ <<  ... child node type... INTERNAL!
				*	     ├─┬─-┤	... child # of keys = 0 ... child is Empty!
				* ┌──-┐┌-─┐┌─-┐
				* │ 2 3 ││   ││ 8 │
				* ├┬─-┤├-─┘├─-┤
				*/
				Node* leftchild = child[underflow - 1];
				vector<int> leftchild_key = leftchild->get_keylist();
				child[underflow]->add_key(node->get_key(underflow - 1));
				node->del_key(node->get_key(underflow - 1));
				node->add_key(leftchild_key.back());
				leftchild_key.pop_back();
				/*	     ┌───-┐
				*	     │ 3  7  │ << 
				*	     ├─┬─-┤	
				* ┌──-┐┌-─┐┌─-┐
				* │ 2   ││ 5 ││ 8 │
				* ├┬─-┤├-─┘├─-┤
				*/
				child[underflow]->set_child(child[underflow]->get_child()[0], 1);
				child[underflow]->set_child(leftchild->get_child()[leftchild_key.size() + 1], 0);
				leftchild->set_child(nullptr, leftchild_key.size() + 1);
				/*	   ┌───-┐
				*	   │ 3  7  │ <<
				*	   ├─┬─-┤
				* ┌─-┐┌-─┐┌─-┐
				* │ 2 ││ 5 ││ 8 │
				* ├─-┤├-─┤├─-┤
				*/
			}
			else if (tmp == 2) {			// when right adjacent child has more than one key
				/*	   ┌───-┐
				*	   │ 3  5  │ <<  ... child node type... INTERNAL!
				*	   ├─┬─-┤	... child # of keys = 0 ... child is Empty!
				* ┌─-┐┌-─┐┌──-┐
				* │ 2 ││   ││ 6 7 │
				* ├─-┤├-─┘├─┬-┤
				*/
				Node* rightchild = child[underflow + 1];
				vector <int> rightchild_key = rightchild->get_keylist();
				child[underflow]->add_key(node->get_key(underflow + 1));
				node->del_key(node->get_key(underflow + 1));
				node->add_key(rightchild_key.front());
				rightchild_key.erase(rightchild_key.begin());
				/*	   ┌───-┐
				*	   │ 3  6  │ <<
				*	   ├─┬─-┤
				* ┌─-┐┌-─┐┌──-┐
				* │ 2 ││ 5 ││ 7   │
				* ├─-┤├-─┘├─┬-┤
				*/
				child[underflow]->set_child(rightchild->get_child()[0], 1);
				for (unsigned int i = 0; i < rightchild_key.size(); i++) {
					rightchild->set_child(rightchild->get_child()[i + 1], i);
				}
				rightchild->set_child(nullptr, rightchild_key.size() + 1);
				/*	   ┌───-┐
				*	   │ 3  6  │ <<
				*	   ├─┬─-┤
				* ┌─-┐┌-─┐┌─-┐
				* │ 2 ││ 5 ││ 7 │
				* ├─-┤├-─┤├─-┤
				*/
			}
			else {							// when both left and right adjacent child has less than one key
				// this may cause root node to be empty...
				// require additional check whether root is empty...
				if (underflow == parent_size) {		// when right-most child is empty
					/*	   ┌───-┐
					*	   │ 3  5  │ <<
					*	   ├─┬─-┤
					* ┌─-┐┌─-┐┌─-┐
					* │ 2 ││ 4 ││   │
					* ├─-┤├-─┤├─-┘
					*/
					Node* prevchild = child[underflow - 1];
					int until = prevchild->add_key(node->get_key(underflow - 1));
					prevchild->set_child(child[underflow]->get_child()[0], prevchild->get_keylist().size());
					node->del_child(underflow);
					node->del_key(node->get_key(parent_size - 1));
					/*	  ┌─-┐
					*	  │ 3 │ <<
					*	  ├─-┤
					* ┌─-┐┌───┐
					* │ 2 ││ 4  5 │
					* ├─-┤├─┬─┤
					*/
				}
				else {
					/*	   ┌───-┐
					*	   │ 3  5  │ <<
					*	   ├─┬─-┤
					* ┌─-┐┌─-┐┌─-┐
					* │ 2 ││   ││ 6 │
					* ├─-┤├-─┘├─-┤
					*/
					Node* nextchild = child[underflow + 1];
					int number = node->get_key(underflow);
					nextchild->add_key(number);
					for (int i = nextchild->get_keylist().size(); i > 0; i--) {
						nextchild->set_child(nextchild->get_child()[i - 1], i);
					}
					nextchild->set_child(child[underflow]->get_child()[0], 0);
					for (int j = underflow; j < parent_size; j++) {
						node->set_child(node->get_child()[j + 1], j);
					}
					node->del_child(parent_size);
					node->del_key(number);
					/*	  ┌─-┐
					*	  │ 3 │ <<
					*	  ├─-┤
					* ┌─-┐┌───┐
					* │ 2 ││ 5  6 │
					* ├─-┤├─┬─┤
					*/
				}
			}
		}
		
	}
	return;
}

int check_side(Node* node, int index) {
	if (index > 0 && node->get_child()[index - 1]->get_keylist().size() > 1) {
		return 1;
	}
	else if (node->get_child()[index + 1] != nullptr && node->get_child()[index + 1]->get_keylist().size() > 1) {
		return 2;
	}
	else {
		return 0;
	}
}

void key_update(Node* node, int key) {
	if (node->get_type() == TREE_LEAF || node->get_type() == TREE_ROOT_LEAF) {
		return;
	}
	else {
		int i = 0;
		for (int keys : node->get_keylist()) {
			i++;
			if (key == keys) {
				Node* leftmost_leaf = get_leftmost_leaf(node->get_child()[i]);
				int nextkey;
				if (leftmost_leaf->isEmpty()){
					if (node->get_child()[i]->isEmpty() || leftmost_leaf->get_next() == NULL) {
						return;
					}
					nextkey = leftmost_leaf->get_next()->get_key(0);
				}
				else {
					nextkey = leftmost_leaf->get_key(0);
				}
				node->del_key(key);
				node->add_key(nextkey);
				return;
			}
		}
		return;
	}
}


Node* get_leftmost_leaf(Node* node) {
	if (node->get_type() == TREE_LEAF) {
		return node;
	}
	else {
		return get_leftmost_leaf(node->get_child()[0]);
	}
}

void print_leaf(Node* node) {
	if (node->get_type() == TREE_LEAF || node->get_type() == TREE_ROOT_LEAF) {
		vector<int> list = node->get_keylist();
		for (unsigned int i = 0; i < list.size(); i++) {
			cout << list[i] << " ";
		}
		Node* next = node->get_next();
		if (next != NULL) {
			cout << "|" << " ";
			print_leaf(next);
		}
		else {
			cout << "" << endl;
		}
	}
	else {
		print_leaf(node->get_child()[0]);
	}
}

void print_tree(Node* node, int pointer) {
	int size = node->get_keylist().size();
	if (node->get_type() == TREE_LEAF || node->get_type() == TREE_ROOT_LEAF) {
		if (pointer < size) {
			cout << node->get_key(pointer) << endl;
			pointer++;
			print_tree(node, pointer);
			return;
		}
	}
	else {
		int capacity = node->get_capacity();
		int degree = node->get_degree();
		
		for (int i = 0; i < size; i++) {
			print_tree(node->get_child()[i], 0);
			for (int j = 0; j < degree; j++) {
				cout << "  ";
			}
			cout << node->get_key(i) << endl;
		}
		print_tree(node->get_child()[size], 0);
		return;
	}
}

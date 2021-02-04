#pragma once

#include "node.h"

using namespace Tree;

void insert_node(Node* node, int key);

Node* delete_node(Node* node, int key);

void insert_arrange(Node* node);

void delete_arrange(Node* node);

int check_side(Node* node, int index);

void key_update(Node* node, int key);

Node* get_leftmost_leaf(Node* node);

void print_leaf(Node* node);

void print_tree(Node* node);
	

/*
 * MIT License
 *
 * Copyright (c) 2018 Guillaume St-Onge
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "BinaryTree.hpp"
#include <iostream>
#include <set>

using namespace std;

namespace sset
{//start of namespace sset


//Default constructor of the class BinaryTree
BinaryTree::BinaryTree() :
    root_(nullptr),
    current_node_(nullptr),
    leaves_vector_(),
    leaves_index_map_()
{
}

//Constructor of the class BinaryTree with specified leaves number
BinaryTree::BinaryTree(unsigned int n_leaves) :
    root_(nullptr),
    current_node_(nullptr),
    leaves_vector_(),
    leaves_index_map_()
{
    if (n_leaves < 1)
    {
        cout << "Impossible tree" << endl;
    }
    else
    {
        BinaryTreeNode* root = new BinaryTreeNode;
        root_ = root;
        current_node_ = root; //assign current node to the root
        unsigned int n_nodes = 2*n_leaves - 1; //number of nodes for
                                               //a full binary tree
        root->child_left = branch(root, 1, n_nodes);
        root->child_right = branch(root, 2, n_nodes);
    }
}

//Copy constructor of the class BinaryTree
BinaryTree::BinaryTree(const BinaryTree& tree) :
    root_(nullptr),
    current_node_(nullptr),
    leaves_vector_(),
    leaves_index_map_()
{
    //Construct a new tree with n_leaves
    unsigned int n_leaves = tree.leaves_vector_.size();
    BinaryTreeNode* root = new BinaryTreeNode;
    root_ = root;
    current_node_ = root; //assign current node to the root
    unsigned int n_nodes = 2*n_leaves - 1; //number of nodes for
                                           //a full binary tree
    root->child_left = branch(root, 1, n_nodes);
    root->child_right = branch(root, 2, n_nodes);

    //Give the same values to the leaves
    LeafIndex leaf_index = 0;
    for (auto iter = tree.leaves_vector_.begin();
        iter != tree.leaves_vector_.end(); ++iter)
    {
        update_value(leaf_index, (*iter)->value);
        leaf_index += 1;
    }
}

//Overload the assignement operator
BinaryTree& BinaryTree::operator=(const BinaryTree& tree)
{
    //destroy the current tree
    this->destroy_tree(root_);
    this->leaves_vector_.clear();
    this->leaves_index_map_.clear();

    //Construct a new tree with n_leaves
    unsigned int n_leaves = tree.leaves_vector_.size();
    BinaryTreeNode* root = new BinaryTreeNode;
    root_ = root;
    current_node_ = root; //assign current node to the root
    unsigned int n_nodes = 2*n_leaves - 1; //number of nodes for
                                           //a full binary tree
    root->child_left = branch(root, 1, n_nodes);
    root->child_right = branch(root, 2, n_nodes);

    //Give the same values to the leaves
    LeafIndex leaf_index = 0;
    for (auto iter = tree.leaves_vector_.begin();
        iter != tree.leaves_vector_.end(); ++iter)
    {
        update_value(leaf_index, (*iter)->value);
        leaf_index += 1;
    }

    return *this;
}

//Destructor of the class
BinaryTree::~BinaryTree()
{
    destroy_tree(root_);
}

//Get the leaf index associated to the cumulative fraction r
LeafIndex BinaryTree::get_leaf_index(double r)
{
    double cumul = 0;
    double total_value = get_value();
    while (not is_leaf())
    {
        if (r <= (cumul + get_value_left())/total_value)
        {
            move_down_left();
        }
        else
        {
            cumul += get_value_left();
            move_down_right();
        }
    }
    LeafIndex chosen_leaf = get_leaf_index();
    reset_current_node();
    return chosen_leaf;
}

//Recursive method to construct a full binary tree
BinaryTreeNode* BinaryTree::branch(BinaryTreeNode* parent, int node_index,
    int n_nodes)
{

    BinaryTreeNode* node;
    if(node_index < n_nodes)
    {
        //Branch and create childs if possible
        node = new BinaryTreeNode;
        node->parent = parent;
        node->child_left = branch(node, 2*node_index+1, n_nodes);
        node->child_right = branch(node, 2*node_index+2, n_nodes);
    }
    else
    {
        node = nullptr;
        if (leaves_index_map_.find(parent) == leaves_index_map_.end())
        {
            //the parent node is identified as a leaf
            leaves_index_map_[parent] = leaves_vector_.size();
            leaves_vector_.push_back(parent);
        }
    }

    return node;
}

//Recursively destroy the subtree associated to the node
void BinaryTree::destroy_tree(BinaryTreeNode* node)
{
  if(node != nullptr)
  {
    destroy_tree(node->child_left);
    destroy_tree(node->child_right);
    delete node;
  }
}

//update value for the leaf and parents
void BinaryTree::update_value(LeafIndex leaf_index, double variation)
{
    current_node_ = leaves_vector_[leaf_index];
    (current_node_->value) += variation;
    while(not is_root())
    {
        move_up();
        (current_node_->value) += variation;
    }
}

//update value for current leaf
void BinaryTree::update_value(double variation)
{
    if (is_leaf())
    {
        (current_node_->value) += variation;
        while(not is_root())
        {
            move_up();
            (current_node_->value) += variation;
        }
    }
    else
    {
        cout << "not a leaf" << endl;
    }
}

void BinaryTree::update_zero()
{
    if (is_leaf())
    {
        (current_node_->value) = 0;
        while(not is_root())
        {
            move_up();
            (current_node_->value) = 0;
        }
    }
    else
    {
        cout << "not a leaf" << endl;
    }
}

//remove value for all nodes
void BinaryTree::clear()
{
    for (auto& leaf : leaves_vector_)
    {
        move_at(leaf);
        update_zero();
    }
}

}//end of namespace sset

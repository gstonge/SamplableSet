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

#ifndef BINARYTREE_HPP
#define BINARYTREE_HPP

#include <vector>
#include <unordered_map>
#include <cmath>

namespace sset
{//start of namespace sset

//Define new types
typedef unsigned int LeafIndex;

//Define a structure for the nodes of a binary tree
struct BinaryTreeNode
{
    BinaryTreeNode() : value(0.), parent(nullptr), child_left(nullptr),
    child_right(nullptr) {}
    double value;
    BinaryTreeNode* parent;
    BinaryTreeNode* child_left;
    BinaryTreeNode* child_right;
};

//Binary tree to udpate dynamically a cumulative distribution
class BinaryTree
{
public:
    //Constructor
    BinaryTree();
    BinaryTree(unsigned int n_leaves);
    BinaryTree(const BinaryTree& tree);

    //Assignement operator
    BinaryTree& operator=(const BinaryTree& tree);

    //Destructor
    ~BinaryTree();

    //Accessors
    bool is_root()
        {return current_node_->parent == nullptr;}
    bool is_leaf()
        {return current_node_->child_left == nullptr and
            current_node_->child_right == nullptr ;}
    double get_value() const
        {return current_node_->value;}
    double get_value_left() const
        {return (current_node_->child_left)->value;}
    double get_value_right() const
        {return (current_node_->child_right)->value;}
    LeafIndex get_leaf_index() const
        {return leaves_index_map_.at(current_node_);}
    LeafIndex get_leaf_index(double r);

    //Mutators
    void reset_current_node()
        {current_node_ = root_;}
    void move_down_left()
        {current_node_ = current_node_->child_left;}
    void move_down_right()
        {current_node_ = current_node_->child_right;}
    void move_up()
        {current_node_ = current_node_->parent;}
    void move_at(BinaryTreeNode* node)
        {current_node_ = node;}
    void update_value(LeafIndex leaf_index, double variation);
    void update_value(double variation);
    void update_zero();
    void clear();


private:
    //Members
    BinaryTreeNode* root_;
    BinaryTreeNode* current_node_;
    std::vector<BinaryTreeNode*> leaves_vector_;
    std::unordered_map<BinaryTreeNode*,LeafIndex> leaves_index_map_;

    //To be called by constructors and assignement operator
    BinaryTreeNode* branch(BinaryTreeNode* parent, int node_index,
    int n_nodes);

    //to be called by destructor
    void destroy_tree(BinaryTreeNode* node);

};


}//end of namespace sset

#endif /* BINARYTREE_HPP */

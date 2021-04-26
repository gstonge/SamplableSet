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

#ifndef SAMPLABLESET_HPP_
#define SAMPLABLESET_HPP_

#include "HashPropensity.hpp"
#include "BinaryTree.hpp"
#include "pcg-cpp/include/pcg_random.hpp"
#include <utility>
#include <random>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <string>
#include <stdexcept>

namespace sset
{//start of namespace sset

typedef unsigned int GroupIndex;
typedef unsigned int InGroupIndex;
typedef std::pair<GroupIndex, InGroupIndex> SSetPosition; //Set element Id
typedef pcg32 RNGType;

//Base class to contain the shared RNG for derived template classes
class BaseSamplableSet
{
    public:
        static void seed(unsigned int seed_value);
    protected:
        static RNGType gen_;
};


/*
 * Set of elements, samplable efficiently using composition and rejection
 * sampling.
 */
template <class T>
class SamplableSet : public BaseSamplableSet
{
public:
    //Definition
    typedef std::vector<std::pair<T,double> > PropensityGroup;

    //Default constructor
    SamplableSet(double min_weight, double max_weight);
    //Copy constructor
    SamplableSet(const SamplableSet<T>& s);

    //Accessors
    std::size_t size() const {return position_map_.size();}
    bool empty() const {return size() == 0;}
    std::size_t inline count(const T& element) const
        {return position_map_.count(element);}
    std::pair<T,double> sample() const;
    template <typename ExtRNG>
    std::pair<T,double> sample_ext_RNG(ExtRNG& gen) const;
    double total_weight() const {return sampling_tree_.get_value();}
    double get_weight(const T& element) const;
    std::pair<T,double> get_at_iterator() const;

    //Mutators
    void insert(const T& element, double weight = 0);
    void set_weight(const T& element, double weight);
    void erase(const T& element);
    void next();
    void init_iterator();
    void clear();


private:
    double min_weight_;
    double max_weight_;
    mutable std::uniform_real_distribution<double> random_01_;
    HashPropensity hash_;
    unsigned int number_of_group_;
    std::vector<double> max_propensity_vector_;
    std::unordered_map<T,SSetPosition> position_map_;
    mutable BinaryTree sampling_tree_;
    std::vector<PropensityGroup> propensity_group_vector_;
    mutable typename PropensityGroup::iterator iterator_;
    mutable GroupIndex iterator_group_index_;
    //private method
    void weight_checkup(double weight) const;
};


//Default constructor for the class SamplableSet
template <typename T>
SamplableSet<T>::SamplableSet(double min_weight, double max_weight) :
    min_weight_(min_weight),
    max_weight_(max_weight),
    random_01_(0.,1.),
    hash_(min_weight, max_weight),
    number_of_group_(hash_(max_weight)+1),
    max_propensity_vector_(number_of_group_, 2*min_weight),
    position_map_(),
    sampling_tree_(number_of_group_),
    propensity_group_vector_(number_of_group_),
    iterator_(),
    iterator_group_index_(0)
{
    //Initialize max propensity vector
    if (number_of_group_ > 2)
    {
        for (unsigned int i = 0; i < number_of_group_-2; i++)
        {
            max_propensity_vector_[i+1] = max_propensity_vector_[i]*2;
        }
    }
    max_propensity_vector_.back() = max_weight;
}

//Copy constructor
template <typename T>
SamplableSet<T>::SamplableSet(const SamplableSet<T>& s) :
    min_weight_(s.min_weight_),
    max_weight_(s.max_weight_),
    random_01_(0.,1.),
    hash_(s.hash_),
    number_of_group_(s.number_of_group_),
    max_propensity_vector_(s.max_propensity_vector_),
    position_map_(s.position_map_),
    sampling_tree_(s.sampling_tree_),
    propensity_group_vector_(s.propensity_group_vector_),
    iterator_(),
    iterator_group_index_(0)
{
}

//throw a invalid_argument error if the weight is out of bounds
template <typename T>
void SamplableSet<T>::weight_checkup(double weight) const
{
    if (weight < min_weight_ or weight > max_weight_)
    {
        std::string out = "Weight " + std::to_string(weight) + " out of bounds [" +
            std::to_string(min_weight_) + "," + std::to_string(max_weight_) + "]";
        throw std::invalid_argument(out);
    }
}

//sample an element according to its weight
template <typename T>
std::pair<T,double> SamplableSet<T>::sample() const
{
    GroupIndex group_index;
    InGroupIndex in_group_index;
    if (not empty())
    {
        group_index = sampling_tree_.get_leaf_index(random_01_(gen_));
        bool element_not_chosen = true;
        while (element_not_chosen)
        {
            in_group_index = floor(random_01_(gen_)*propensity_group_vector_.at(
                        group_index).size());

            if (random_01_(gen_) <
                    propensity_group_vector_.at(group_index).at(
                        in_group_index).second/(max_propensity_vector_.at(
                            group_index)))
            {
                element_not_chosen = false;
            }
        }
    }
    else
    {
        std::string out = "The samplable set is empty";
        throw std::out_of_range(out);
    }

    return propensity_group_vector_.at(group_index).at(in_group_index);
}

//sample an element according to its weight using an external RNG
template <typename T>
template <typename ExtRNG>
std::pair<T,double>SamplableSet<T>::sample_ext_RNG(ExtRNG& gen) const
{
    GroupIndex group_index;
    InGroupIndex in_group_index;
    if (not empty())
    {
        group_index = sampling_tree_.get_leaf_index(random_01_(gen));
        bool element_not_chosen = true;
        while (element_not_chosen)
        {
            in_group_index = floor(random_01_(gen)*propensity_group_vector_.at(
                        group_index).size());

            if (random_01_(gen) <
                    propensity_group_vector_.at(group_index).at(
                        in_group_index).second/(max_propensity_vector_.at(
                            group_index)))
            {
                element_not_chosen = false;
            }
        }
    }
    else
    {
        std::string out = "The samplable set is empty";
        throw std::out_of_range(out);
    }

    return propensity_group_vector_.at(group_index).at(in_group_index);
}

//get the weight of an element if it exists
template <typename T>
double SamplableSet<T>::get_weight(const T& element) const
{
    double weight;
    if(count(element))
    {
        const SSetPosition& position = position_map_.at(element);
        weight = (propensity_group_vector_[position.first][position.second]).second;
    }
     else
    {
        std::string out = "Key error, the element is not in the set";
        throw std::out_of_range(out);
    }

    return weight;
}

//insert an element in the set with its associated weight
//if the element is already there, do nothing
template <typename T>
void SamplableSet<T>::insert(const T& element, double weight)
{
    weight_checkup(weight);
    //insert element only if not present
    if (position_map_.find(element) == position_map_.end())
    {
        GroupIndex group_index = hash_(weight);
        InGroupIndex in_group_index =
            propensity_group_vector_[group_index].size();
        propensity_group_vector_[group_index].push_back(
                std::make_pair(element,weight));
        position_map_[element] = SSetPosition(group_index, in_group_index);
        sampling_tree_.update_value(group_index, weight);
    }
}

//set a new weight for the element in the set
//if the element does not exists, same as insert
template <typename T>
void SamplableSet<T>::set_weight(const T& element, double weight)
{
    weight_checkup(weight);
    erase(element);
    insert(element, weight);
}

//Remove element from the set
template <typename T>
void SamplableSet<T>::erase(const T& element)
{
    //remove element if present
    if (count(element))
    {
        const SSetPosition& position = position_map_.at(element);
        //create alias for element and its weight pair
        std::pair<T, double>& element_weight_pair =
            propensity_group_vector_[position.first][position.second];
        sampling_tree_.update_value(position.first, -element_weight_pair.second);
        //gives position to last element of propensity group and swap
        position_map_[
            (propensity_group_vector_[position.first].back()).first] = position;
        std::swap(element_weight_pair,
                propensity_group_vector_[position.first].back());
        //remove
        propensity_group_vector_[position.first].pop_back();
        position_map_.erase(element);
    }
}

//Remove all elements from the set
template <typename T>
void SamplableSet<T>::clear()
{
    sampling_tree_.clear();
    position_map_.clear();
    iterator_group_index_ = 0;
    for (auto &group_vector : propensity_group_vector_)
    {
        group_vector.clear();
    }
}




template <typename T>
void SamplableSet<T>::next()
{
    if (iterator_ != propensity_group_vector_.back().end())
    {
        iterator_ ++;
        //change group if necessary
        while (iterator_ == propensity_group_vector_[
            iterator_group_index_].end() and
            iterator_group_index_ < propensity_group_vector_.size()-1)
        {
            iterator_group_index_ += 1;
            iterator_ = propensity_group_vector_[
                iterator_group_index_].begin();
        }
    }
    if (iterator_ == propensity_group_vector_.back().end())
    {
        throw std::out_of_range("");
    }
}

template <typename T>
std::pair<T,double> SamplableSet<T>::get_at_iterator() const
{
    if (iterator_ == (propensity_group_vector_.back()).end())
    {
        std::string out = "At the end of the SamplableSet";
        throw std::out_of_range(out);
    }

    return *iterator_;
}

template <typename T>
void SamplableSet<T>::init_iterator()
{
    iterator_group_index_ = 0;
    iterator_ = propensity_group_vector_[0].begin();
    //it is possible the group is empty, look for subsequent containers
    //note that if the set is empty, it will just point to the end
    while (iterator_ == propensity_group_vector_[
            iterator_group_index_].end() and
            iterator_group_index_ < propensity_group_vector_.size()-1)
    {
        iterator_group_index_ += 1;
        iterator_ = propensity_group_vector_[
            iterator_group_index_].begin();
    }
    //throw stop iteration error if empty
    if (iterator_ == propensity_group_vector_.back().end())
    {
        throw std::out_of_range("");
    }
}

}//end of namespace sset

#endif /* SAMPLABLESET_HPP_ */

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
#include <optional>
#include <stdio.h>
#include <time.h>

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

RNGType BaseSamplableSet::gen_ = RNGType(time(NULL));

//seed the RNG
void BaseSamplableSet::seed(unsigned int seed_value)
{
    BaseSamplableSet::gen_.seed(seed_value);
}

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
    std::size_t inline count(const T& element) const
        {return position_map_.count(element);}
    std::optional<std::pair<T,double> > sample() const;
    template <typename ExtRNG>
    std::optional<std::pair<T,double> > sample_ext_RNG(ExtRNG& gen) const;
    double total_weight() const {return sampling_tree_.get_value();}
    std::optional<double> get_weight(const T& element) const;
    std::optional<std::pair<T,double> > get_at_iterator() const;

    //Mutators
    void insert(const T& element, double weight = 0);
    void set_weight(const T& element, double weight);
    void erase(const T& element);
    void next();
    void init_iterator();
    void clear();


private:
    mutable std::uniform_real_distribution<double> random_01_;
    HashPropensity hash_;
    unsigned int number_of_group_;
    std::vector<double> max_propensity_vector_;
    std::unordered_map<T,SSetPosition> position_map_;
    mutable BinaryTree sampling_tree_;
    std::vector<PropensityGroup> propensity_group_vector_;
    mutable typename PropensityGroup::iterator iterator_;
    mutable GroupIndex iterator_group_index_;
};


//Default constructor for the class SamplableSet
template <typename T>
SamplableSet<T>::SamplableSet(double min_weight, double max_weight) :
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

//sample an element according to its weight
template <typename T>
std::optional<std::pair<T,double> > SamplableSet<T>::sample() const
{
    if (sampling_tree_.get_value() > 0)
    {
        GroupIndex group_index = sampling_tree_.get_leaf_index(random_01_(gen_));
        bool element_not_chosen = true;
        InGroupIndex in_group_index;
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
        return propensity_group_vector_.at(group_index).at(in_group_index);
    }
    else
    {
        return std::nullopt;
    }
}

//sample an element according to its weight using an external RNG
template <typename T>
template <typename ExtRNG>
std::optional<std::pair<T,double> > SamplableSet<T>::sample_ext_RNG(ExtRNG& gen) const
{
    if (sampling_tree_.get_value() > 0)
    {
        GroupIndex group_index = sampling_tree_.get_leaf_index(random_01_(gen));
        bool element_not_chosen = true;
        InGroupIndex in_group_index;
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
        return propensity_group_vector_.at(group_index).at(in_group_index);
    }
    else
    {
        return std::nullopt;
    }
}

//get the weight of an element if it exists
template <typename T>
std::optional<double> SamplableSet<T>::get_weight(const T& element) const
{
    if(count(element))
    {
        const SSetPosition& position = position_map_.at(element);
        return (propensity_group_vector_[position.first][position.second]).second;
    }
    else
    {
        return std::nullopt;
    }
}

//insert an element in the set with its associated weight
//if the element is already there, do nothing
template <typename T>
void SamplableSet<T>::insert(const T& element, double weight)
{
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
}

template <typename T>
std::optional<std::pair<T,double> > SamplableSet<T>::get_at_iterator() const
{
    if (iterator_ != (propensity_group_vector_.back()).end())
    {
        return *iterator_;
    }
    else
    {
        return std::nullopt;
    }
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
    }

}//end of namespace sset

#endif /* SAMPLABLESET_HPP_ */

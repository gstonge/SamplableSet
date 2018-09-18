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

#ifndef SAMPLABLESETCR_HPP_
#define SAMPLABLESETCR_HPP_

#include "HashPropensity.hpp"
#include "BinaryTree.hpp"
#include "pcg-cpp/include/pcg_random.hpp"
#include <utility>
#include <random>
#include <iostream>
#include <optional>

namespace sset
{//start of namespace sset

typedef unsigned int GroupIndex;
typedef unsigned int InGroupIndex;
typedef std::pair<GroupIndex, InGroupIndex> SSetPosition; //Set element Id
typedef pcg32 RNGType;

/*
 * Set of elements, samplable efficiently using composition and rejection
 * sampling.
 */
template <class T>
class SamplableSetCR
{
public:
    //Definition
    typedef std::vector<std::pair<T,double> > PropensityGroup;

    //Default constructor
    SamplableSetCR(double min_weight, double max_weight,
            unsigned int seed = 42);
    //Copy constructor
    SamplableSetCR(const SamplableSetCR<T>& s);
    SamplableSetCR(const SamplableSetCR<T>& s, unsigned int  seed);

    /*
     * On the birthday problem. By default, the copy constructor uses the
     * generator from the previous object to initialize the seconde one. It has
     * to be noted that for many copies, this may lead to collisions for the
     * seed. Even if seeds can be chosen within 2^32 numbers, the expected
     * number of copies for which it is expected that we have 1 collison on
     * average is 9300 approximately--roughly the square root of 2^32. For
     * safer use of this constructor, pass a sequence of seeds.
     */

    //Accessors
    std::size_t size() const {return position_map_.size();}
    std::size_t inline count(const T& element) const
        {return position_map_.count(element);}
    std::optional<std::pair<T,double> > sample() const;
    double total_weight() const {return sampling_tree_.get_value();}

    //Mutators
    void insert(const T& element, double weight = 0);
    void set_weight(const T& element, double weight);
    void erase(const T& element);

private:
    //unvarying
    mutable RNGType gen_;
    mutable std::uniform_real_distribution<double> random_01_;
    HashPropensity hash_;
    unsigned int number_of_group_;
    std::vector<double> max_propensity_vector_;
    //varying
    std::unordered_map<T,SSetPosition> position_map_;
    mutable BinaryTree sampling_tree_;
    std::vector<PropensityGroup> propensity_group_vector_;
};

//Default constructor for the class SamplableSetCR
template <typename T>
SamplableSetCR<T>::SamplableSetCR(double min_weight, double max_weight,
        unsigned int seed) : gen_(seed), hash_(min_weight, max_weight),
    number_of_group_(hash_(max_weight)+1), position_map_(),
    sampling_tree_(number_of_group_), random_01_(0.,1.),
    propensity_group_vector_(number_of_group_),
    max_propensity_vector_(number_of_group_, 2*min_weight)
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

//Copy constructor without seed
template <typename T>
SamplableSetCR<T>::SamplableSetCR(const SamplableSetCR<T>& s) : gen_(s.gen_()),
    hash_(s.hash_), number_of_group_(s.number_of_group_),
    position_map_(s.position_map_), sampling_tree_(s.sampling_tree_),
    random_01_(0.,1.), propensity_group_vector_(s.propensity_group_vector_),
    max_propensity_vector_(s.max_propensity_vector_)
{
}

//Copy constructor with seed
template <typename T>
SamplableSetCR<T>::SamplableSetCR(const SamplableSetCR<T>& s,
        unsigned int seed) : gen_(seed),
    hash_(s.hash_), number_of_group_(s.number_of_group_),
    position_map_(s.position_map_), sampling_tree_(s.sampling_tree_),
    random_01_(0.,1.), propensity_group_vector_(s.propensity_group_vector_),
    max_propensity_vector_(s.max_propensity_vector_)
{
}

template <typename T>
std::optional<std::pair<T,double> > SamplableSetCR<T>::sample() const
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

//Insert an element in the set with its associated weight
template <typename T>
void SamplableSetCR<T>::insert(const T& element, double weight)
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
template <typename T>
void SamplableSetCR<T>::set_weight(const T& element, double weight)
{
    erase(element);
    insert(element, weight);
}

//Remove element from the set
template <typename T>
void SamplableSetCR<T>::erase(const T& element)
{
    //remove element if present
    if (position_map_.find(element) != position_map_.end())
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



}//end of namespace sset

#endif /* SAMPLABLESETCR_HPP_ */

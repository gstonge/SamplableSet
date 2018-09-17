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


#ifndef HASHPAIR_HPP_
#define HASHPAIR_HPP_

#include <functional>
#include <utility>

namespace std
{//start of namespace std

//Hash combine to map multiple dimension data to 1D index
template <class T>
inline void hash_combine(size_t& seed, const T& v)
{
    hash<T> hasher;
    seed ^= hasher(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

//Hash specialization for pairs
template <typename T, typename U>
struct hash<pair<T, U> >
{
    size_t operator()(const pair<T, U>& p) const
    {
        size_t seed = 0;
        hash_combine(seed, p.first);
        hash_combine(seed, p.second);
        return seed;
    }
};

//Recursive template code derived from Matthieu M. to hash arbitrary tuples
template <class Tuple, size_t Index = tuple_size<Tuple>::value - 1>
struct HashValueImpl
{
    static void apply(size_t& seed, Tuple const& tuple)
    {
        HashValueImpl<Tuple, Index-1>::apply(seed, tuple);
        hash_combine(seed, get<Index>(tuple));
    }
};

template <class Tuple>
struct HashValueImpl<Tuple,0>
{
    static void apply(size_t& seed, Tuple const& tuple)
    {
        hash_combine(seed, get<0>(tuple));
    }
};

template <typename... TT>
struct hash<tuple<TT...>>
{
    size_t operator()(tuple<TT...> const& tt) const
    {
        size_t seed = 0;
        HashValueImpl<tuple<TT...> >::apply(seed, tt);
        return seed;
    }
};

}//end of namespace std

#endif /* HASHPAIR_HPP_ */

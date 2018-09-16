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

#include "HashPropensity.hpp"
#include <cmath>

using namespace std;

namespace sset
{//start of namespace sset


//Constructor
HashPropensity::HashPropensity(double propensity_min, double propensity_max) :
    propensity_min_(propensity_min), propensity_max_(propensity_max),
    power_of_two_(false)
{
    if (floor(log2(propensity_max/propensity_min)) ==
        ceil(log2(propensity_max/propensity_min)) and
        propensity_max != propensity_min)
    {
        power_of_two_ = true;
    }
}

//Call operator
size_t HashPropensity::operator()(double propensity) const
{
    size_t index = floor(log2(propensity/propensity_min_));
    if (power_of_two_ and propensity == propensity_max_)
    {
        index -=1 ;
    }
    return index;
}

}//end of namespace sset

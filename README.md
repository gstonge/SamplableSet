# SamplableSet

An implementation of sets that can be randomly sampled according to the weights of the elements. One only needs to specify the minimal and maximal expected weights in the set. Insertion, erase and sample of elements are constant time in average-case for all practical purposes. It makes use of the [pybind11](https://github.com/pybind/pybind11) project to bind C++ objects to python.

This family of classes is very useful when one needs to udpate dynamically (insertion, erase) a set elements, as well as sample from it. It uses a composition and rejection scheme, making it efficient even if the weights span multiples order of magnitude. For instance, this kind of situation can arise in markov chains, when a collection of independant processes have multiscale propensities.

## Requirements

* A compiler with C++17 support

## Installation

First, clone this repository.
```bash
git clone https://github.com/gstonge/SamplableSet.git
```
Second, use pip to install the module.
```bash
pip install ./SamplableSet
```

## A peak under the hood

On the C++ side, samplable sets are created using a template class. This template accept any kind of class elements to be put in the set if a hash object exists in the standard library. Otherwise, one needs to specialize the std::hash structure.

```
├── src
    ├── hash_specialization.hpp
```
To expose a new C++ samplable set to python, one needs to bind the class to pybind.

```
├── src
    ├── bind_SamplableSetCR.hpp
```

To further wrap this new object to SamplableSet, it needs to be added to the python wrapper.

```
├── SamplableSet
    ├── _wrapper.py
```

Once this is done, the class can be used elegantly in python. A few types are already implemented :

* `int`
* `tuple<int,int,int>`

## Basic usage

The current version of SamplableSet offers a C++ style for the interface of the class in python. New versions should migrate towards a more pythonic interface.

### Set creation

First to create an samplable set, one needs to specify the minimal (maximal) weight for elements in the set, as well as the C++ type of the elements that will be inserted in the set.

```python
from SamplableSet import SamplableSet

# Calling the default constructor
s = SamplableSet(min_weight=1, max_weight=100, cpp_type='int')
```

### Pythonic way to do set insertion, setting new weights, sampling and removing elements

```python
# Give preferential weights to elements
for i in range(1,101):
    s[i] = i
    # Equivalent to s.insert(element=i, weight=i)

# Syntax to change the weight of an element is the same as insertion
# Set new weight:
s[1] = 10
# Equivalent to s.set_weight(element=1, weight=10)

# Sample according to the distribution in O(1)
element, weight = s.sample()

# Remove the element
s.erase(element)

# Get the number of elements in the set
len(s)
# Equivalent to s.size()
```

Other accessors methods are defined, such as `total_weight` for the sum of weights.

### Copy constructor

There are two ways to call the copy constructor. The first one is without specifying a seed for the new RNG. In this case, the seed is obtained from the previous RNG, which is OK, except if you need to make a lot of copies of a same object--see the [Birthday problem](https://en.wikipedia.org/wiki/Birthday_problem). With the current implementation, there is a 1% chance collision for the seed if you make 9300 copies. The second method requires you to specify the seed.

```python
s_copy1 = SamplableSet(samplable_set=s)
s_copy2 = SamplableSet(samplable_set=s, seed=42)
```

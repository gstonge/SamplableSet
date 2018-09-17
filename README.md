# SamplableSet

An implementation of sets that can be randomly sampled according to the weights of the elements. One only needs to specify the minimal and maximal expected weights in the set. Insertion, erase and sample of elements are constant time in average-case for all practical purposes.

This family of classes is very useful when one needs to udpate dynamically (insertion, erase) a set elements, as well as sample from it. It uses a composition and rejection scheme, making it efficient even if the weights span multiples order of magnitude. For instance, this kind of situation can arise in markov chains, when a collection of independant processes have multiscale propensities.

## Requirements

**On Unix (Linux, OS X)**

* A compiler with C++11 support

**On Windows**

* Visual Studio 2015 (required for all Python versions)

## Installation

First, clone this repository.
```bash
git clone https://github.com/gstonge/SamplableSet.git
```
Second, use pip to install the module.
```bash
pip install ./SamplableSet
```

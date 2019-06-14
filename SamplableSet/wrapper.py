# MIT License

# Copyright (c) 2018 Guillaume St-Onge

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# S

from functools import singledispatch
from _SamplableSetCR import *
import copy

implemented_type_dict={
    "int": IntSamplableSet,
    "tuple<int,int,int>": EdgeSamplableSet,
    "tuple<tuple<int,int>,tuple<int,int>>": DoubleEdgeSamplableSet
}
implemented_type_list=implemented_type_dict.keys()

#wrapper for general default constructor
@singledispatch
def SamplableSet(min_weight, max_weight, seed=42, cpp_type="int"):
    return implemented_type_dict[cpp_type](min_weight, max_weight, seed)

#wrapper for specific copy constructors
# for cpp_type in implemented_type_dict.values():
    # @SamplableSet.register(cpp_type)
    # def _(samplable_set):
        # return cpp_type(samplable_set)

@SamplableSet.register(IntSamplableSet)
def _(samplable_set, seed = None):
    if seed is None:
        return IntSamplableSet(samplable_set)
    else:
        return IntSamplableSet(samplable_set, seed)

@SamplableSet.register(EdgeSamplableSet)
def _(samplable_set, seed = None):
    if seed is None:
        return EdgeSamplableSet(samplable_set)
    else:
        return EdgeSamplableSet(samplable_set, seed)

@SamplableSet.register(DoubleEdgeSamplableSet)
def _(samplable_set, seed = None):
    if seed is None:
        return DoubleEdgeSamplableSet(samplable_set)
    else:
        return DoubleEdgeSamplableSet(samplable_set, seed)

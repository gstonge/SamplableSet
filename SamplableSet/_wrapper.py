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
# SOFTWARE.

from _SamplableSetCR import *

template_classes = {
    'int': IntSamplableSet,
    'edge': EdgeSamplableSet
}

class SamplableSet:
    """
    This class implements a set which is samplable in O(1) time according to the weight distribution of the elements.

    The SamplableSet can be instanciated with parameters or from a pre-existing one.

    This class is a wrapper around a C++ implementation.
    """
    def __init__(self, *, samplable_set=None, min_weight=None, max_weight=None, seed=None, cpp_type='int'):
        """
        Creates a new SamplableSet instance.

        Args:
            samplable_set (SamplableSet instance, optional): An instance of SamplableSet to be copied. The new object will have the same elements with the same weights. If not None, all other parameters will be ignored.
            min_weight (float, optional): Minimum weight a given element can have. This is needed for a good repartition of the elements inside the internal tree structure. Necessary if 'samplable_set' is None.
            max_weight (float, optional): Minimum weight a given element can have. This is needed for a good repartition of the elements inside the internal tree structure. Necessary if 'samplable_set' is None.
            seed (float, optional): Seed used to sample elements from the set.
            cpp_type (str, optional, either 'int' or 'edge'): Type used in the C++ implementation.
        """
        if samplable_set is not None:
            self._samplable_set = samplable_set.copy()
        else:
            if seed is None:
                self._samplable_set = template_classes[cpp_type](min_weight, max_weight)
            else:
                self._samplable_set = template_classes[cpp_type](min_weight, max_weight, seed)

            for func_name in ['size', 'total_weight', 'count', 'sample', 'insert', 'set_weight', 'erase']:
                setattr(self, func_name, getattr(self._samplable_set, func_name))

        self.max_weight = max_weight
        self.min_weight = min_weight

    def __contains__(self, element):
        return True if self.count(element) else False

    def __getitem__(self, element):
        return self.get_weight(element)

    def __setitem__(self, element, weight):
        if self.min_weight <= weight <= self.max_weight:
            if element in self:
                self.set_weight(element, weight)
            else:
                self.insert(element, weight)
        else:
            raise ValueError(f'Cannot assign weight outside range [{self.min_weight}, {self.max_weight}].')

    def __delitem__(self, element):
        self.erase(element)

    def __str__(self):
        return f'SamplableSet of {len(self)} element' + ('s' if len(self) > 1 else '')

    def __repr__(self):
        return str(self)

    def __len__(self):
        return self.size()

    def copy(self):
        return type(self._samplable_set)(self._samplable_set)

    def __deepcopy__(self, memo_dict):
        return self.copy()

    def __copy__(self):
        return self.copy()

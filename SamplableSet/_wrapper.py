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
    'str': StringSamplableSet
}

class SamplableSet:
    """
    This class implements a set which is samplable in O(1) time according to the weight distribution of the elements. Elements can either be an integer or a tuple of 3 integers.

    The SamplableSet can be instanciated empty or from an iterable of pairs of elements and weights respectively.

    This class is a wrapper around a C++ implementation.
    """
    def __init__(self, min_weight, max_weight, elements_weights=None, seed=None, cpp_type='int'):
        """
        Creates a new SamplableSet instance.

        Args:
            min_weight (float): Minimum weight a given element can have. This is needed for a good repartition of the elements inside the internal tree structure.
            max_weight (float): Maximum weight a given element can have. This is needed for a good repartition of the elements inside the internal tree structure.
            elements_weights (iterable of iterables or dict, optional): If an iterable, should be yield iterables of 2 items (element, weight) with which the set will be instanciated. If a dict, keys should be the elements and values should be the weights. If not specified, the set will be empty.
            seed (float, optional): Seed used to sample elements from the set.
            cpp_type (str, optional, either 'int' or 'str'): Type used in the C++ implementation. If 'elements_weights' is specified, the type will be infered from it.
        """
        self.max_weight = max_weight
        self.min_weight = min_weight
        self.seed = seed or 42
        self.cpp_type = cpp_type

        # Unpacking
        if elements_weights:
            if isinstance(elements_weights, dict):
                elements_weights = elements_weights.items()

            # Infering cpp_type
            first_element, first_weight = next(iter(elements_weights))
            if isinstance(first_element, int):
                self.cpp_type = 'int'
            elif isinstance(first_element, str):
                self.cpp_type = 'str'
            else:
                raise ValueError('Cannot infer the type from the elements')

        # Instanciate the set
        self._samplable_set = template_classes[self.cpp_type](min_weight, max_weight, self.seed)
        self._wrap_methods()

        # Initialize the set
        if elements_weights:
            self[first_element] = first_weight
            for element, weight in elements_weights:
                self[element] = weight

    def _wrap_methods(self):
        """
        Assigns the methods of the C++ class to the wrapper.
        """
        for func_name in ['size', 'total_weight', 'count', 'insert', 'next',
                          'init_iterator', 'set_weight', 'get_weight',
                          'get_at_iterator', 'erase']:
            setattr(self, func_name, getattr(self._samplable_set, func_name))

    def __contains__(self, element):
        return True if self.count(element) else False

    def __getitem__(self, element):
        return self.get_weight(element) or 0

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

    def copy(self, seed=None):
        seed = seed or self.seed
        cpp_copy_samplable_set = type(self._samplable_set)(self._samplable_set, seed) # Copy of the C++ class with the copy constructor
        wrapped_samplable_set_copy = type(self)(self.min_weight, self.max_weight, seed=seed, cpp_type=self.cpp_type) # New wrapper object to be returned

        # Link the wrapper with the wrappee
        wrapped_samplable_set_copy._samplable_set = cpp_copy_samplable_set
        wrapped_samplable_set_copy._wrap_methods()

        return wrapped_samplable_set_copy

    def __deepcopy__(self, memo_dict):
        return self.copy()

    def __copy__(self):
        return self.copy()

    def __iter__(self):
        return self.element_generator()

    def sample(self, n_samples=1):
        """
        Randomly samples the set according to the weights of each element in O(1) time.

        Args:
            n_samples (int, optional): If equal to 1, returns one element. If greater than 1, returns a generator that will return 'n_samples' elements.

        Returns: An element of the set or a generator of 'n_samples' elements.
        """
        if n_samples == 1:
            return self._samplable_set.sample()
        else:
            return self.sample_generator(n_samples)

    def sample_generator(self, n_samples):
            for _ in range(n_samples):
                yield self._samplable_set.sample()

    def element_generator(self):
        self.init_iterator()
        while self.get_at_iterator() is not None:
            yield self.get_at_iterator()
            self.next()



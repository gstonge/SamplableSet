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

from _SamplableSet import *

#wrap some methods to return python type errors
def error_decorator(error_type):
    def decorator(func):
        def wrapper(*args, **kwargs):
            try:
                val = func(*args,**kwargs)
            except IndexError as err:
                raise error_type(err)
            return val
        return wrapper
    return decorator

template_classes = {
    'int': IntSamplableSet,
    'str': StringSamplableSet,
    '2int': Tuple2IntSamplableSet,
    '3int': Tuple3IntSamplableSet,
    '2str': Tuple2StringSamplableSet,
    '3str': Tuple3StringSamplableSet,
    'Int': IntSamplableSet,
    'String': StringSamplableSet,
    'Tuple2Int': Tuple2IntSamplableSet,
    'Tuple3Int': Tuple3IntSamplableSet,
    'Tuple2String': Tuple2StringSamplableSet,
    'Tuple3String': Tuple3StringSamplableSet
}

cpp_methods = ['size', 'total_weight', 'count', 'insert', 'next',
               'init_iterator', 'set_weight', 'get_weight', 'empty',
               'get_at_iterator', 'erase', 'clear']

class SamplableSet:
    """
    This class implements a set which is samplable according to the weight distribution of the elements.

    The SamplableSet can be instanciated empty or from an iterable of pairs of elements and weights respectively.

    This class is a wrapper around a C++ implementation.
    """
    def __init__(self, min_weight, max_weight, elements_weights=None,
                 cpp_type=None):
        """
        Creates a new SamplableSet instance.

        Args:
            min_weight (float): Minimum weight a given element can have. This is needed for a good repartition of the elements inside the internal tree structure.
            max_weight (float): Maximum weight a given element can have. This is needed for a good repartition of the elements inside the internal tree structure.
            elements_weights (iterable of iterables or dict, optional): If an iterable, should be yield iterables of 2 items (element, weight) with which the set will be instanciated. If a dict, keys should be the elements and values should be the weights. If not specified, the set will be empty.
            cpp_type (str, optional): Type used in the C++ implementation. If 'elements_weights' is specified, the type will be infered from it.
        """
        if min_weight <= 0 or max_weight == float('inf') or\
           max_weight < min_weight:
            raise ValueError('Invalid min_weight or max_weight')

        self.max_weight = max_weight
        self.min_weight = min_weight
        self.cpp_type = cpp_type

        # Unpacking
        if elements_weights:
            if isinstance(elements_weights, dict):
                elements_weights = elements_weights.items()
            first_element, first_weight = next(iter(elements_weights))
            # Inferring cpp_type
            if self.cpp_type is None:
                self._infer_type(first_element)

        # Instanciate the set
        if self.cpp_type is not None:
            self._samplable_set = template_classes[self.cpp_type](min_weight, max_weight)
            self._wrap_methods()
        else:
            self._wrap_methods_unspecified()

        # Initialize the set
        if elements_weights:
            self[first_element] = first_weight
            for element, weight in elements_weights:
                self[element] = weight

    def _infer_type(self,element):
            if isinstance(element, int):
                self.cpp_type = 'int'
            elif isinstance(element, str):
                self.cpp_type = 'str'
            elif isinstance(element, tuple):
                m = len(element)
                if m in {2,3}:
                    if isinstance(element[0], int):
                        self.cpp_type = f'{m}int'
                    elif isinstance(element[0], str):
                        self.cpp_type = f'{m}str'
                    else:
                        raise ValueError('Cannot infer the type from the element')
                else:
                    raise ValueError('Cannot infer the type from the element')
            else:
                raise ValueError('Cannot infer the type from the element')

    def _unspecified_method(self):
        raise RuntimeError('The method is undefined until the underlying type is known')

    def _wrap_methods_unspecified(self):
        """
        Assigns the methods of the C++ class to a dummy method that raises
        a RuntimeError
        """
        for func_name in cpp_methods:
            setattr(self, func_name, self._unspecified_method)

    def _wrap_methods(self):
        """
        Assigns the methods of the C++ class to the wrapper.
        """
        for func_name in cpp_methods:
            setattr(self, func_name, getattr(self._samplable_set, func_name))
        #decorates method to return python error
        self.next = error_decorator(StopIteration)(self.next)
        self.init_iterator = error_decorator(StopIteration)(self.init_iterator)
        self.get_weight = error_decorator(KeyError)(self.get_weight)
        self.cpp_sample = error_decorator(KeyError)(self._samplable_set.sample)

    def __contains__(self, element):
        return True if self.count(element) else False

    def __getitem__(self, element):
        return self.get_weight(element)

    def __setitem__(self, element, weight):
        if self.cpp_type is None:
            self._infer_type(element)
            #instanciate the set
            self._samplable_set = template_classes[self.cpp_type](
                self.min_weight,self.max_weight)
            self._wrap_methods()
        self.set_weight(element,weight)

    def __delitem__(self, element):
        self.erase(element)

    def __str__(self):
        if self.cpp_type is None:
            outstr = 'SamplableSet of unspecified type'
        else:
            outstr = f'SamplableSet of {self.cpp_type} '\
                    + f'containing {len(self)} element'\
                    + ('s' if len(self) > 1 else '')
        return outstr

    def __repr__(self):
        return str(self)

    def __len__(self):
        return self.size()

    def copy(self):
        cpp_copy_samplable_set = type(self._samplable_set)(self._samplable_set) # Copy of the C++ class with the copy constructor
        wrapped_samplable_set_copy = type(self)(self.min_weight, self.max_weight, cpp_type=self.cpp_type) # New wrapper object to be returned

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

    def sample(self, n_samples=1, replace=True):
        """
        Randomly samples the set according to the weights of each element.

        Args:
            n_samples (int, optional): If equal to 1, returns one element. If greater than 1, returns a generator that will return 'n_samples' elements.

        Returns: An element of the set or a generator of 'n_samples' elements.
        """
        if n_samples == 1:
            x = self.cpp_sample()
            if not replace:
                self.erase(x[0])
            return x
        else:
            return self.sample_generator(n_samples, replace)

    def sample_generator(self, n_samples, replace):
        for _ in range(n_samples):
            x = self.cpp_sample()
            if not replace:
                self.erase(x[0])
            yield x

    def element_generator(self):
        try:
            self.init_iterator()
            while True:
                yield self.get_at_iterator()
                self.next()
        except StopIteration:
            pass

    @staticmethod
    def seed(seed_value):
        IntSamplableSet.seed(seed_value) #does not matter which seeds the RNG


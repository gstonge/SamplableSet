# MIT License

# Copyright (c) 2018 Jean-Samuel Leboeuf, Guillaume St-Onge

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
# SOFTWARE.from _SamplableSetCR import *

template_classes = {
    'int': IntSamplableSet,
    'edge': EdgeSamplableSet
}

class SamplableSet:
    def __init__(self, *, samplable_set=None, min_weight=None, max_weight=None, seed=None, cpp_type="int"):
        if samplable_set is not None:
            self._samplable_set = samplable_set.copy()
        else:
            if seed is None:
                self._samplable_set = template_classes[cpp_type](min_weight, max_weight)
            else:
                self._samplable_set = template_classes[cpp_type](min_weight, max_weight, seed)

            for func_name in ['size', 'total_weight', 'count', 'sample', 'insert', 'set_weight', 'erase']:
                setattr(self, func_name, getattr(self._samplable_set, func_name))

    def __len__(self):
        return self.size()

    def copy(self):
        return type(self._samplable_set)(self._samplable_set)

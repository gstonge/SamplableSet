#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Test the initializaiton, insertion, sampling, etc. methods for samplable set

Author: Guillaume St-Onge <guillaume.st-onge.4@ulaval.ca>
"""

from SamplableSet import SamplableSet

class TestSampling:
    def test_sampling_single(self):
        elements = ['a']
        weights = [33.3]
        elements_weights = zip(elements, weights)
        s = SamplableSet(1, 100, elements_weights)
        element, weight = s.sample()
        assert element == 'a' and weight == 33.3

    def test_sampling_generator(self):
        elements = ['a']
        weights = [33.3]
        elements_weights = zip(elements, weights)
        s = SamplableSet(1, 100, elements_weights)
        element_list = []
        weight_list = []
        for element, weight in s.sample(n_samples=5):
            element_list.append(element)
            weight_list.append(weight)
        assert element_list == ['a']*5 and weight_list == [33.3]*5


class TestInitialization:
    def test_dict_init(self):
        elements_weights = {3:33.3, 6:66.6}
        s = SamplableSet(1, 100, elements_weights)
        assert 3 in s and 6 in s

    def test_iterable_init(self):
        elements = ['a', 'b']
        weights = [33.3, 66.6]
        elements_weights = zip(elements, weights)
        s = SamplableSet(1, 100, elements_weights)
        assert 'a' in s and 'b' in s



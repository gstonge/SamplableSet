#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Test the iterator methods for the samplable set

Author: Guillaume St-Onge <guillaume.st-onge.4@ulaval.ca>
"""

from SamplableSet import SamplableSet

class TestCppMethod:
    def test_init_iterator_1(self):
        #elements in same group
        elements = {1:10, 2:10}
        s = SamplableSet(1,100,elements)
        s.init_iterator()
        assert s.get_at_iterator() == (1,10.)

    def test_init_iterator_2(self):
        #elements in different groups
        elements = {1:10, 2:50}
        s = SamplableSet(1,100,elements)
        s.init_iterator()
        assert s.get_at_iterator() == (1,10.)

    def test_next_1(self):
        #elements in same group
        elements = {1:10, 2:10}
        s = SamplableSet(1,100,elements)
        s.init_iterator()
        s.next()
        assert s.get_at_iterator() == (2,10.)

    def test_next_2(self):
        #elements in different groups
        elements = {1:10, 2:50}
        s = SamplableSet(1,100,elements)
        s.init_iterator()
        s.next()
        assert s.get_at_iterator() == (2,50.)

    def test_next_get_None(self):
        elements = {1:10, 2:50}
        s = SamplableSet(1,100,elements)
        s.init_iterator()
        s.next()
        s.next()
        assert s.get_at_iterator() is None


def test_python_generator():
    elements = range(1,10)
    weights = range(1,10)
    s = SamplableSet(1,10,zip(elements,weights))
    expected = zip(elements,weights)
    assert list(expected) == list(s.__iter__())


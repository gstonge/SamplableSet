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

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <SamplableSetCR.hpp>
#include <hash_specialization.hpp>

using namespace std;
using namespace sset;

namespace py = pybind11;

typedef tuple<int,int,int> Edge;
typedef tuple<tuple<int,int>,tuple<int,int>> DoubleEdge;
typedef tuple<int,int> DoubleInt;

//template function to declare different types of samplable set
template<typename T>
void declare_samplable_set(py::module &m, string typestr)
{
    string pyclass_name = typestr + string("SamplableSet");

    py::class_<SamplableSetCR<T> >(m, pyclass_name.c_str())

        .def(py::init<double, double, unsigned int>(), R"pbdoc(
            Default constructor of the class.

            Args:
               min_weight: Minimal weight for elements in the set.
               max_weight: Maximal weight for elements in the set.
               seed: Seed for the RNG.
            )pbdoc", py::arg("min_weight"), py::arg("max_weight"),
            py::arg("seed") = 42)

        .def(py::init<const SamplableSetCR<T>&>(), R"pbdoc(
            Copy constructor without seed of the class.

            Args:
               samplable_set: Copied set
            )pbdoc", py::arg("samplable_set"))

        .def(py::init<const SamplableSetCR<T>&, unsigned int>(), R"pbdoc(
            Copy constructor with seed of the class.

            Args:
               samplable_set: Copied set
               seed: New seed for the RNG.
            )pbdoc", py::arg("samplable_set"), py::arg("seed"))

        .def("size", &SamplableSetCR<T>::size, R"pbdoc(
            Returns the number of elements in the set.
            )pbdoc")

        .def("total_weight", &SamplableSetCR<T>::total_weight, R"pbdoc(
            Returns the sum of the weights of the elements in the set.
            )pbdoc")

        .def("count", &SamplableSetCR<T>::count, R"pbdoc(
            Returns the count of a certain element (0 or 1 since it is a set).

            Args:
               element: Element of the set.
            )pbdoc", py::arg("element"))

        .def("sample", &SamplableSetCR<T>::sample, R"pbdoc(
            Returns an element of the set randomly (according to weights) and
            its weight as a tuple.
            )pbdoc")

        .def("get_weight", &SamplableSetCR<T>::get_weight, R"pbdoc(
            Returns the weight of an element in the set.
            )pbdoc")

        .def("get_at_iterator", &SamplableSetCR<T>::get_at_iterator, R"pbdoc(
            Returns the element at iterator in the set.
            )pbdoc")

        .def("insert", &SamplableSetCR<T>::insert, R"pbdoc(
            Insert an element in the set with its associated weight.

            Args:
               element: Element of the set.
               weight: Weight for random sampling.
            )pbdoc", py::arg("element"), py::arg("weight") = 0)

        .def("set_weight", &SamplableSetCR<T>::set_weight, R"pbdoc(
            Set weight for an element in the set.

            Args:
               element: Element of the set.
               weight: Weight for random sampling.
            )pbdoc", py::arg("element"), py::arg("weight"))

        .def("erase", &SamplableSetCR<T>::erase, R"pbdoc(
            Remove an element from the set.

            Args:
               element: Element of the set.
            )pbdoc", py::arg("element"))

        .def("clear", &SamplableSetCR<T>::clear, R"pbdoc(
            Remove all elements from the container.
            )pbdoc")

        .def("next", &SamplableSetCR<T>::next, R"pbdoc(
            Move the iterator one element ahead.
            )pbdoc")

        .def("init_iterator", &SamplableSetCR<T>::init_iterator, R"pbdoc(
            Put the iterator at the beginning of the set.
            )pbdoc");
}

PYBIND11_MODULE(_SamplableSetCR, m)
{
    declare_samplable_set<int>(m, "Int");
    declare_samplable_set<string>(m, "String");
    declare_samplable_set<Edge>(m, "Edge");
    declare_samplable_set<DoubleEdge>(m, "DoubleEdge");
    declare_samplable_set<DoubleInt>(m, "DoubleInt");
}

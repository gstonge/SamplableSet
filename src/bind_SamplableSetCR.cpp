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

PYBIND11_MODULE(_SamplableSetCR, m)
{
    py::class_<SamplableSetCR<Edge> >(m, "EdgeSamplableSet")

        .def(py::init<double, double, unsigned int>(), R"pbdoc(
            Default constructor of the class EdgeSamplableSet.

            Args:
               min_weight: Minimal weight for elements in the set.
               max_weight: Maximal weight for elements in the set.
               seed: Seed for the RNG.
            )pbdoc", py::arg("min_weight"), py::arg("max_weight"),
            py::arg("seed") = 42)

        .def(py::init<const SamplableSetCR<Edge>&>(), R"pbdoc(
            Copy constructor without seed of the class EdgeSamplableSet.

            Args:
               samplable_set: Copied set
            )pbdoc", py::arg("samplable_set"))

        .def(py::init<const SamplableSetCR<Edge>&, unsigned int>(), R"pbdoc(
            Copy constructor with seed of the class EdgeSamplableSet.

            Args:
               samplable_set: Copied set
               seed: New seed for the RNG.
            )pbdoc", py::arg("samplable_set"), py::arg("seed"))

        .def("size", &SamplableSetCR<Edge>::size, R"pbdoc(
            Returns the number of elements in the set.
            )pbdoc")

        .def("total_weight", &SamplableSetCR<Edge>::total_weight, R"pbdoc(
            Returns the sum of the weights of the elements in the set.
            )pbdoc")

        .def("count", &SamplableSetCR<Edge>::count, R"pbdoc(
            Returns the count of a certain element (0 or 1 since it is a set).

            Args:
               element: Element of the set.
            )pbdoc", py::arg("element"))

        .def("sample", &SamplableSetCR<Edge>::sample, R"pbdoc(
            Returns an element of the set randomly (according to weights) and
            its weight as a tuple.
            )pbdoc")

        .def("get_weight", &SamplableSetCR<Edge>::get_weight, R"pbdoc(
            Returns the weight of an element in the set.
            )pbdoc")

        .def("insert", &SamplableSetCR<Edge>::insert, R"pbdoc(
            Insert an element in the set with its associated weight.

            Args:
               element: Element of the set.
               weight: Weight for random sampling.
            )pbdoc", py::arg("element"), py::arg("weight") = 0)

        .def("set_weight", &SamplableSetCR<Edge>::set_weight, R"pbdoc(
            Set weight for an element in the set.

            Args:
               element: Element of the set.
               weight: Weight for random sampling.
            )pbdoc", py::arg("element"), py::arg("weight"))

        .def("erase", &SamplableSetCR<Edge>::erase, R"pbdoc(
            Remove an element from the set.

            Args:
               element: Element of the set.
            )pbdoc", py::arg("element"));

    py::class_<SamplableSetCR<int> >(m, "IntSamplableSet")

        .def(py::init<double, double, unsigned int>(), R"pbdoc(
            Default constructor of the class IntSamplableSet.

            Args:
               min_weight: Minimal weight for elements in the set.
               max_weight: Maximal weight for elements in the set.
               seed: Seed for the RNG.
            )pbdoc", py::arg("min_weight"), py::arg("max_weight"),
            py::arg("seed") = 42)

        .def(py::init<const SamplableSetCR<int>&>(), R"pbdoc(
            Copy constructor without seed of the class IntSamplableSet.

            Args:
               samplable_set: Copied set
            )pbdoc", py::arg("samplable_set"))

        .def(py::init<const SamplableSetCR<int>&, unsigned int>(), R"pbdoc(
            Copy constructor with seed of the class IntSamplableSet.

            Args:
               samplable_set: Copied set
               seed: New seed for the RNG.
            )pbdoc", py::arg("samplable_set"), py::arg("seed"))

        .def("size", &SamplableSetCR<int>::size, R"pbdoc(
            Returns the number of elements in the set.
            )pbdoc")

        .def("total_weight", &SamplableSetCR<int>::total_weight, R"pbdoc(
            Returns the sum of the weights of the elements in the set.
            )pbdoc")

        .def("count", &SamplableSetCR<int>::count, R"pbdoc(
            Returns the count of a certain element (0 or 1 since it is a set).

            Args:
               element: Element of the set.
            )pbdoc", py::arg("element"))

        .def("sample", &SamplableSetCR<int>::sample, R"pbdoc(
            Returns an element of the set randomly (according to weights) and
            its weight as a tuple.
            )pbdoc")

        .def("get_weight", &SamplableSetCR<int>::get_weight, R"pbdoc(
            Returns the weight of an element in the set.
            )pbdoc")

        .def("insert", &SamplableSetCR<int>::insert, R"pbdoc(
            Insert an element in the set with its associated weight.

            Args:
               element: Element of the set.
               weight: Weight for random sampling.
            )pbdoc", py::arg("element"), py::arg("weight") = 0)

        .def("set_weight", &SamplableSetCR<int>::set_weight, R"pbdoc(
            Set weight for an element in the set.

            Args:
               element: Element of the set.
               weight: Weight for random sampling.
            )pbdoc", py::arg("element"), py::arg("weight"))

        .def("erase", &SamplableSetCR<int>::erase, R"pbdoc(
            Remove an element from the set.

            Args:
               element: Element of the set.
            )pbdoc", py::arg("element"));
}

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

PYBIND11_MODULE(SamplableSet, m)
{
    m.doc() = R"pbdoc(
        EdgeSamplableSet
        -----------------

        Classes of set with an efficient sampling option.

        .. currentmodule:: SamplableSet

        .. autosummary::
           :toctree: _generate


           EdgeSamplableSet.__init__
           EdgeSamplableSet.size
           EdgeSamplableSet.count
           EdgeSamplableSet.sample
           EdgeSamplableSet.insert
           EdgeSamplableSet.set_weight
           EdgeSamplableSet.erase
    )pbdoc";

    py::class_<SamplableSetCR<Edge> >(m, "EdgeSamplableSet")
        .def(py::init<double, double, unsigned int>(), R"pbdoc(
            Constructor of the class EdgeSamplableSet.

            Args:
               min_weight: Minimal weight for elements in the set.
               max_weight: Maximal weight for elements in the set.
               seed: Seed for the RNG
            )pbdoc", py::arg("min_weight"), py::arg("max_weight"),
            py::arg("seed") = 42)

        .def("size", &SamplableSetCR<Edge>::size, R"pbdoc(
            Returns the number of elements in the set.
            )pbdoc")

        .def("count", &SamplableSetCR<Edge>::count, R"pbdoc(
            Returns the count of a certain element (0 or 1 since it is a set).

            Args:
               element: Element of the set.
            )pbdoc", py::arg("element"))
        .def("sample", &SamplableSetCR<Edge>::sample, R"pbdoc(
            Returns an element of the set randomly (according to weights).
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
}

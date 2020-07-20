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
#include <SamplableSet.hpp>
#include <hash_specialization.hpp>

using namespace std;
using namespace sset;

namespace py = pybind11;

typedef tuple<int,int> Tuple2Int;
typedef tuple<int,int,int> Tuple3Int;
typedef tuple<string,string> Tuple2String;
typedef tuple<string,string,string> Tuple3String;

//template function to declare different types of samplable set
template<typename T>
void declare_samplable_set(py::module &m, string typestr)
{
    string pyclass_name = typestr + string("SamplableSet");

    py::class_<SamplableSet<T> >(m, pyclass_name.c_str())

        .def(py::init<double, double>(), R"pbdoc(
            Default constructor of the class.

            Args:
               min_weight: Minimal weight for elements in the set.
               max_weight: Maximal weight for elements in the set.
            )pbdoc", py::arg("min_weight"), py::arg("max_weight"))

        .def(py::init<const SamplableSet<T>&>(), R"pbdoc(
            Copy constructor

            Args:
               samplable_set: Copied set
            )pbdoc", py::arg("samplable_set"))

        .def("size", &SamplableSet<T>::size, R"pbdoc(
            Returns the number of elements in the set.
            )pbdoc")

        .def("empty", &SamplableSet<T>::empty, R"pbdoc(
            Returns true if the set is empty.
            )pbdoc")

        .def("total_weight", &SamplableSet<T>::total_weight, R"pbdoc(
            Returns the sum of the weights of the elements in the set.
            )pbdoc")

        .def("count", &SamplableSet<T>::count, R"pbdoc(
            Returns the count of a certain element (0 or 1 since it is a set).

            Args:
               element: Element of the set.
            )pbdoc", py::arg("element"))

        .def("sample", &SamplableSet<T>::sample, R"pbdoc(
            Returns an element of the set randomly (according to weights) and
            its weight as a tuple.
            )pbdoc")

        .def("get_weight", &SamplableSet<T>::get_weight, R"pbdoc(
            Returns the weight of an element in the set.
            )pbdoc")

        .def("get_at_iterator", &SamplableSet<T>::get_at_iterator, R"pbdoc(
            Returns the element at iterator in the set.
            )pbdoc")

        .def_static("seed", &BaseSamplableSet::seed, R"pbdoc(
            Seed the RNG with a new value.

            Args:
               seed_value: New value for the seed of the RNG.
            )pbdoc", py::arg("seed_value"))

        .def("insert", &SamplableSet<T>::insert, R"pbdoc(
            Insert an element in the set with its associated weight.

            Args:
               element: Element of the set.
               weight: Weight for random sampling.
            )pbdoc", py::arg("element"), py::arg("weight") = 0)

        .def("set_weight", &SamplableSet<T>::set_weight, R"pbdoc(
            Set weight for an element in the set.

            Args:
               element: Element of the set.
               weight: Weight for random sampling.
            )pbdoc", py::arg("element"), py::arg("weight"))

        .def("erase", &SamplableSet<T>::erase, R"pbdoc(
            Remove an element from the set.

            Args:
               element: Element of the set.
            )pbdoc", py::arg("element"))

        .def("clear", &SamplableSet<T>::clear, R"pbdoc(
            Remove all elements from the container.
            )pbdoc")

        .def("next", &SamplableSet<T>::next, R"pbdoc(
            Move the iterator one element ahead.
            )pbdoc")

        .def("init_iterator", &SamplableSet<T>::init_iterator, R"pbdoc(
            Put the iterator at the beginning of the set.
            )pbdoc");
}

PYBIND11_MODULE(_SamplableSet, m)
{
    declare_samplable_set<int>(m, "Int");
    declare_samplable_set<string>(m, "String");
    declare_samplable_set<Tuple2Int>(m, "Tuple2Int");
    declare_samplable_set<Tuple3Int>(m, "Tuple3Int");
    declare_samplable_set<Tuple2String>(m, "Tuple2String");
    declare_samplable_set<Tuple3String>(m, "Tuple3String");
}

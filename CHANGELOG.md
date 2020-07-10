# Changelog

All notable changes to this project will be documented in this file.

The format is inspired by [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html) since v2.0.0.


## [2.1.1] - 2020-07-10
### Added
- New tuple 2 and 3-tuple of `int` and `str` as available `cpp_type`.
- A samplable set can be instanciated empty without specifying the `cpp_type`.
  It will be inferred at the first insertion.

### Changed
- Structure of C++ source files to allow the compilation of a static library.

### Fixed
Test for out of bound weight.

## [v2.0.1] - 2020-06-26
### Changed
- Class name in C++
- RNG is now a static member of C++ `BaseSamplableSet` class. This means all
  objects created (irrespective of the C++ type of the set) share the same RNG.

### Removed
- Obsolete C++ type (Edge, DoubleInt, etc.). Only `int` and `str` cpp types
remain.
- Initialization of the seed for the RNG at construction. Since the RNG now
  belong to the class, it exits without any object created, and it is
  initialized with current time.

### Added
A new static member function for the class SamplableSet to seed the RNG.
Seeding the generator influences all ensuing (or existing) sets objects.


## [v1.0.8] - 2020-06-24
### Added
Sampling without replacement is now possible.

### Changed
Sets are seeded with current time instead of a fixed value by default.

### Fixed
Compilation bug on Mac.


## [v1.0.6] - 2019-10-02
### Fixed
- Compilation warnings fixed by reordering initialization of members in C++
- Bug fix for C++ object only. There was an error in the assignement operator implementation. This wouldn't cause issue for python only usage, but could lead to memory issues when using the C++ object directly.

## [v1.0.2] - 2019-09-08
### Added
- New template declaration function to facilitate the addition of new types in the c++ binder file
- New default SamplableSet with strings
- Implemented C++ methods to iterate over all the elements of a set
- Implemented Python generator/iterator methods
- SamplableSet objects are now iterable

## [v1.0.0] - 2019-09-06
### Changed
Merged the fork from jsleb333, providing a pythonic wrapper.

## [v0.0.1] - 2019-09-06

First release with the old wrapper using C++ style.

[v2.1.1]: https://github.com/gstonge/SamplableSet/compare/v2.0.1...v2.1.1
[v2.0.1]: https://github.com/gstonge/SamplableSet/compare/v1.0.8...v2.0.1
[v1.0.8]: https://github.com/gstonge/SamplableSet/compare/v1.0.6...v1.0.8
[v1.0.6]: https://github.com/gstonge/SamplableSet/compare/v1.0.2...v1.0.6
[v1.0.2]: https://github.com/gstonge/SamplableSet/compare/v1.0.0...v1.0.2
[v1.0.0]: https://github.com/gstonge/SamplableSet/compare/v0.0.1...v1.0.0
[v0.0.1]: https://github.com/gstonge/SamplableSet/releases/tag/v0.0.1

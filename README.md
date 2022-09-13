# generic-pipeline
A C++ repository to pipe operations functional and Unix style!

## The Generic Pipeline
All the pipeline code is in the [generic_pipeline.hpp](/inc/generic_pipeline.hpp). Jump straight to it to the usable pipelines. This currently compiles with `-std=c++20`. I shall be updating the code to add conditional constructs to enable backward compatability. 

The repository is organised into sections as per the coverage of my talk:

## Section 0 - Unix pipelines & C++ journey from `<algorithm>` to `<ranges>`
Introduction and initial code snippets

## Section 1 - C++11
Sequentially building the solution. Follow files from `1.1_*.cpp` to `1.c_*.cpp`.

## Section 2 - C++17
Use of `std::optional` and handling thereof - unpacking & packing.

## Section 3 - C++20
Using `concept`s to make sure `ranges` pipelines are not impacted by generic-pipeline. 

## Section 4 - C++23
Improvise and make concise the continuations of the `std::optional` monadic interface introduced in C++23.

## Compiling & testing
Steps are: 

```sh
git clone git@github.com:sankurm/generic-pipeline.git

mkdir build
cd build

# To compile the file 1.2_op1_concrete.cpp in directory 1_cpp11/src:
ls ../1_cpp11/src/1.2_op1_concrete.cpp
../build_test.sh 1.2

# Similarly, to compile file 2.3_with_constref_overload.cpp inside directory 2_cpp17/src:
ls ../2_cpp17/src/2.3_with_constref_overload.cpp
../build_test.sh 2.3

# Syntax is
../build_test.sh <file-prefix-no>
```

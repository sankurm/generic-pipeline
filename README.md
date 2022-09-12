# generic-pipeline
A C++ repository to pipe operations functional and Unix style!

## The Generic Pipeline
All the pipeline code is in the [generic_pipeline.hpp](/inc/generic_pipeline.hpp). Jump straight to it to the usable pipelines. 

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

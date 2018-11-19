## Random notes for the implementation

* Having a hybrid implementation is better than just having MPI (with OpenMP or with CUDA) for the grade.
* First do very quickly the sequential code.
* Always check your results with the sequential code.

==============N-Body--Steps==============

--------Design Choices and Baselines---------

1. Choice of the Algorithm -> LeepFrog LEAP

2. DirectSum Baseline
2. a. Transformation from C to C++
2. b. Make use of the C++ advantages (std::vec, Templates)

3. a. Construction of the Tree
3. b. Calculation of the Force using the tree
3. c. Tree update

4. Parallelization
4. a. Construction of the tree
4. b. Force calcultation using the tree
4. c. Tree update

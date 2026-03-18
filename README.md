# RISCV-V V extension simulator

RISC-V vector extension v0.7 (base) simulator implemented in C++.

# Requirements

* CMake >= 3.16
* Boost >= 1.66

# Building

```
cmake -S . -B build
cmake --build build
```

Run tests:

```
ctest --test-dir build
```

Or run the test executables directly:

```
build/tests/vector_examples
build/tests/stdlib
build/tests/unit_tests
```

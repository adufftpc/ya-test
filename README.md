# YA-TEST

## Task

Develop a C/C++ module (library) for a block memory allocator. This allocator should allow memory allocation of a fixed-size block (one by one) from a static pool (pool in static memory). The sizes of the block and the pool are fixed during the program’s execution but should be configurable during the project build process. The module must be adapted to work on embedded platforms of various bit widths in a multitasking environment (RTOS). The module should have a set of unit tests (covering two or three basic cases is sufficient).

Conditions requiring clarification should be interpreted at your discretion, and a corresponding comment should be left in the solution (preferably in the form of a comment in the source code files). Comments should only use Latin letters.

## Comments

Code is designed to be platform independent (i.e. bit width, bit numbering). It uses mutex to provite basic thread-safety. Uses STL, it is not intentet to be used on the MCUs running RTOS (it is made due to lack of MCU board to debug right now). Anyway, to acheive the compatibility the sinchronisation will require minor changes. Mutex should be replaced with RTOS primitives.

## Unit tests

Added very basic tests.

```bash
$ ./ya-test-utests.elf
[==========] Running 3 test suites.
[ RUN      ] common.allocate_all
[       OK ] common.allocate_all (4.61us)
[ RUN      ] common.allocate_more_than_available
[       OK ] common.allocate_more_than_available (1.28us)
[ RUN      ] common.check_deallocation_and_reusing
[       OK ] common.check_deallocation_and_reusing (0.51us)
[==========] 3 test suites ran
[  PASSED  ] 3 suites

Summary:
    Total test suites:          3
    Total suites run:           3
    Total warnings generated:   0
    Total suites skipped:       0
    Total suites failed:        0
SUCCESS: 3 test suites passed in 43.01us
```
#!/bin/zsh
ls src/**/*.cpp src/**/*.h | xargs clang-format -i -style="{ BasedOnStyle: LLVM, Standard: Cpp11, ColumnLimit: 80, Cpp11BracedListStyle: false, AllowShortFunctionsOnASingleLine: Empty }"

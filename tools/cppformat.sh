#!/bin/sh
clang-format -i -style="{ BasedOnStyle: LLVM, Standard: Cpp11, ColumnLimit: 0, Cpp11BracedListStyle: false }" $@

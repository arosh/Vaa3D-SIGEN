#!/bin/sh
ls -1 src/**/*.cpp src/**/*.h | xargs python tools/cpplint.py --filter=-build/include_order,-whitespace/indent,-build/header_guard,-legal/copyright,-runtime/references,-whitespace/comments 2>&1 | grep -Ev "^Done processing "

#!/bin/zsh
ls src/**/*.{h,cpp} vaa3d/*.{h,cpp} | xargs python tools/cpplint.py --filter=-build/include_order,-build/header_guard,-legal/copyright,-runtime/references,-whitespace/indent,-whitespace/comments,-whitespace/line_length,-readability/casting 2>&1 | grep -Ev "^Done processing "

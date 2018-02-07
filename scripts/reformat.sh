#!/usr/bin/env bash

filepaths=$(find . -type f \( -name "*.cpp" -or -name "*.h" -or -name "*.hpp" \))
for filepath in $filepaths; do
    echo "Formatting ${filepath}..."
    clang-format -style=file -i "${filepath}"
done

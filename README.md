# JSON Spirit

JSON Spirit is a JSON library with parser based on Boost.Spirit, fork of the [JSON Spirit from CodeProject](https://www.codeproject.com/Articles/20027/).

**Do not use this library in new projects!**

This fork created to maintain existing projects. For the new projects, use one of these:

- [Tencent/rapidjson](https://github.com/Tencent/rapidjson) - extremely fast and well-tested JSON library with inconvient, but powerful API
- [nlohmann/json](https://github.com/nlohmann/json) - convient and well-tested JSON library for Modern C++

**Why I should not use it?**

This library

* Is **not** thread-safe
* Is **not** fully Unicode compatible
* Can produce warnings in your code
* Can throw non-standard exceptions

We will fix this in future, but right now **do not use this library in new projects**

## Development

[![Build Status](https://travis-ci.org/sergey-shambir/json_spirit.svg?branch=master)](https://travis-ci.org/sergey-shambir/json_spirit)

Current goals:

- cleanup this library (use clang-tidy analyzer and clang-format, setup CI, etc.)
- fix standards conformance (e.g. fix Unicode support for UTF-8 mode)

There's is no goal to complicate library, improve it's perfomance, etc.

## Build requirements

- Compiler with almost full C++14 support
    - GCC 7.0 or higher
    - Clang 4.0 or higher
    - MSVC 2015 or higher
- Boost 1.59 or higher (Boost.Spirit and Boost.Locale)
- CMake 3.8 or higher

>NOTE: original [JSON Spirit from CodeProject](https://www.codeproject.com/Articles/20027/) has lower requirements

# HSM.c

Heirarchical State Machine (HSM) library for complex, stateful, event-driven
applications written in C.

HSM is based on Unified Modeling Language (UML) state machines (also called
state charts).

## Use

Embed the files in `src/` into your project. Then design your state machine. 
Implement in source code, creating states objects, events, and event handlers.

## Dependencies

None! HSM.c is straight C11 code.

If you want to build the examples or run the unit tests, you can either use
[Pixi](https://pixi.sh/) or [CMake](https://cmake.org/). Pixi uses CMake
under the hood. You will also need a C compiler. The Pixi project is configured
to use gcc.

## Examples

The `examples/` directory contains example projects that use HSM.c.

### Turnstile

The Turnstile example project simulates a turnstile using a state machine. For
more information, see the Turnstile [README.md](examples/turnstile/README.md) 

To build with Pixi, use `pixi run turnstile`.

To build with CMake,

```shell
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --target turnstile
```

## Test

Unit tests use the [Unity Test Project](https://github.com/ThrowTheSwitch/Unity)
as a framework. It is linked as a submodule to this project, so either clone
this project using the `--recurse-submodules` flag when cloning this project, or
use `git submodule init` and `git submodule update` to pull it into the project.

If you aren't going to run the unit tests against HSM.c, you do not need to pull
in Unity.

To build tests with Pixi, use `pixi run test`.

To build tests with CMake, 

```shell
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --target hsm_test
```

## License

Copyright 2024, Andrew Lin. All rights reserved.

This library is licensed under the MIT License. See LICENSE.txt or
https://opensource.org/licenses/MIT.


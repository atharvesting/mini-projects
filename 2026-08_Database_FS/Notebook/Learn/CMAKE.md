- First line must always be `cmake_minimum_required` to set a project requirement and be backwards compatible.
- The `project` should follow to set the name of the project.
- In order to set the entry point executable, use `add_executable`.

Most fundamental CMakeLists.txt file:

```cmake
cmake_minimum_required(VERSION 3.20)
project(Hello)
add_executable(Hello Hello.c)
```
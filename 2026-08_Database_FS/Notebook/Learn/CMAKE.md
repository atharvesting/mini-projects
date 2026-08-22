#### Basic CMake Configuration

- First line must always be `cmake_minimum_required` to set a project requirement and be backwards compatible.
- The `project` should follow to set the name of the project.
- In order to set the entry point executable, use `add_executable`.

Most fundamental CMakeLists.txt file:

```cmake
cmake_minimum_required(VERSION 3.20)
project(Hello)
add_executable(Hello Hello.c)
```

#### GTest Link Warnings concerning runtime library mismatches

- Link warnings can show up when googletest uses different runtime libraries compared to our defaults. Enabling the 
option `gtest_force_shared_crt` will make gtest link the runtimes dynamically and match the project in which it 
is included. There are two ways to enable this option:
    - Using the CMake statement: `set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)`.
    - Setting `cmake_force_shared_crt:BOOL` to `ON`.

#### Test Fixtures for reusable data
 - When several tests can work with the same initial data, instead of initializing it within every test, we can define a 
 fixture struct where this data is initialized.

```cpp
 struct SpenvTests
    : public ::testing::Test
{
    virtual void SetUp() override {
        // Initialize data here
    }

    virtual void TearDown() override {
        // Destruct data here if appropriate
    }
};
```
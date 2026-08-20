2026-08-20
---
#### ERROR: Dereferencing a Null Pointer
- The Valgrind message "Address 0x0 is not stack'd, malloc'd or (recently) free'd" means that the program is attempting to
dereference a null pointer. In the context of my program, this meant that I wasn't even creating the pointer properly.

```cpp
// Before:
std::unique_ptr<InputBuffer> input_buffer;  // This doesn't work!
// After:
auto input_buffer = std::make_unique<InputBuffer>();  // STL contains make_unique specially to create unique pointers
```

#### ERROR: Passing `std::unique_ptr<>` into a function
- Ensure that `std::unique_ptr` is passed as a reference or const reference as copy constructors are prohibited.
```cpp
// Before:
void read_input(std::unique_ptr<InputBuffer> input_buffer)
// After:
void read_input(std::unique_ptr<InputBuffer>& input_buffer)
```
---

```cpp
// Before:

// After:

```
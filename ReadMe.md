# Smart Memory Vector  
## Introduction  
This library is currently in an early release stage and may contain some bugs or unexpected behavior. I would be very grateful for any feedback you can provide. I am also open to hearing any suggestions you may have for improvements.  

## Description
SMV is a unique variation of a dynamic array that provides the user with a multitude of ways to control memory and work with indices.  

C++ offers a wide variety of arrays, but here is what sets SMV apart:  
- Complete control over the RAM occupied by a dynamic array. Memory management is permitted from both ends (front/back).  
- Most of these methods were already present in `std::vector` and `std::deque`, but SMV provides more flexible control over them.  
- SMV is applicable to standard devices, as well as to systems with strict memory constraints.  

# Memory Management  
There are two types of memory management for different methods:  
- via `set_growth_factor([value])`. Increases the array's current capacity by a factor of `[value]` [1.1, 10.0] when an element is added to the array.
This is the default growth mechanism for the following methods: `push_back(), push_front(), emplace_back(), emplace_front(), insert()`

- via the `[memory]` parameter in the method call. Increases the array's current capacity by the value `[memory] * sizeof(T)` (must be >= 1).  
Used in the overloads of the following methods: `push_back(), push_front(), insert()`  

# Class Methods  
## Basic Methods
- `size()` - Returns the number of initialized elements
- `capacity()` - Returns the number of elements the array can currently hold
- `begin()` - Returns a pointer to the first element of the array
- `end()` - Returns a pointer to the last element of the array
- `cbegin()` - Returns a const pointer to the first element of the array
- `cend()` - Returns a const pointer to the last element of the array
- `back()` - Returns a reference to the last element (right side)
- `front()` - Returns a reference to the first element (left side)
- `empty()` - Checks if the array is empty and returns a boolean value.
- `push_back([value])` - Adds an element to the right side of the array
- `push_front([value])` - Adds an element to the left side of the array
- `pop_back()` - Destroys the element on the right side
- `pop_front()` - Destroys the element on the left side
- `emplace_back([value])` - Creates an element directly inside the array and appends it to the right side
- `emplace_front([value])` - Creates an element directly inside the array and appends it to the left side
- `erase([index])` - Removes an element from an array by index and centers the entire sequence
- `insert([index], [value])` - Inserts an element at the specified index
- `clear()` - Destroys all elements
## Memory Monitoring Methods
- `get_growth_factor()` - Returns the factor used when the array needs to grow
- `set_growth_factor([value])` - Sets the growth factor used when reallocation occurs [1.1, 10.0]
- `reserve_back([capacity])` - Increases the array's capacity to the specified value (extends from the right side)
- `reserve_front([capacity])` - Increases the array's capacity to the specified value (extends from the left side)
- `insert([index], [value], [memory])` - Inserts an element at the specified index. If the current capacity is exceeded, it recreates the array with a size increased by the specified amount
- `push_back([value], [memory])` - Adds an element to the right side of the array. If the current capacity is exceeded, it recreates the array with a size increased by the specified amount
- `push_front([value], [memory])` - Adds an element to the left side of the array. If the current capacity is exceeded, it recreates the array with a size increased by the specified amount

# Extra
- Required C++17 or later
- Namespace `smv::`

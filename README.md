# pool

A memory pool

Basically you instantiate a template of the pool, giving each pool you want a unique id and optionally, a custom allocator and deallocator.

```cpp
using pool_t = pool<0>;
```

You can then `initialize(size)` the pool with the desired pool size, which will allocate a buffer on your behalf.
Alternatively you can call `initialize(ptr,size)` passing in your own buffer.

`deinitialize()` deinitializes the pool and releases any memory it may have allocated.

From there you can call `allocate(size)`, `deallocate(ptr)`, and `reallocate(ptr,size)` to manipulate memory.
You can call `deallocate_all()` to invalidate all pointers and reset the pool to empty.

The pool will reclaim memory when it can, but will never fragment, because deallocation and reallocation do not create holes in the heap.

This behavior is at the expense of potentially unused memory as overhead, like any pool of this sort.

```
[env:node32s]
platform = espressif32
board = node32s
framework = arduino ; or espidf
lib_deps = 
	codewitch-honey-crisis/htcw_pool@^0.3.0
```
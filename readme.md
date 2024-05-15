# transitive-dependency
I am slowly uncovering the mysteries of our legacy CMake at work. We recently discovered some dependencies being resolved through projects that didn't make sense. We found this by removing a dependency from a `target_link_libraries` call that we thought a project didn't need. Let's call this project 'B'. Turns out, a project (we'll call this 'C') that depended on some other library 'A' resolved symbols through 'B'.


Here's a more visual example: C => B => A
                             

## The discovery
After looking through the CMake [documentation](https://cmake.org/cmake/help/latest/command/target_link_libraries.html#overview) for `target_link_libraries`, dependencies can be resolved transitively through other dependencies. So in the example above, 'A' depends on 'C', but this dependency is not made explicit. Instead, it depends on 'B' which resolved the dependency because of the default behavior of `target_link_libraries`. From what I understand, these instructions are equivalent:
```cmake
target_link_libraries(B A)
target_link_libraries(B PUBLIC A)
```

## The problem
Using our example again, even if 'C' doesn't actually resolve any symbols to 'B', 'C' still depends on 'B'. Here's some output from ldd if we ran it against 'C':
```
...
libb.so => /home/jon/Projects/transitive-dependencies/build/libb-t.so (0x00007effab46a000)
liba.so => /home/jon/Projects/transitive-dependencies/build/liba.so (0x00007effab465000)
...
```

This means 'C' cannot run without 'B'. This is not ideal for delivering projects to customers.

## The solution
For clarity, 100% of our CMake at the time of writing this readme resolves dependencies with something like this:
```cmake
target_link_libraries(B A)
```

To fix the problem, simply replace it with this:
```cmake
target_link_libraries(B PRIVATE A)
...
target_link_libraries(C PRIVATE A)
```

This produces the following ldd output:
```
...
liba.so => /home/jon/Projects/transitive-dependencies/build/liba.so (0x00007fd957033000)
...
```

## Considerations
I'm not suggesting that exposing other libraries through some other project is a bad thing, but it should be used sparingly. In our case, it's a bad thing, so we need to make use of the `PRIVATE` keyword.

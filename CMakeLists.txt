cmake_minimum_required(VERSION 3.17)
project(transitive-dependency)

# Create a library that 'b' and 'c' need
add_library(a SHARED ../src/a.h ../src/a.cpp)

# Create a version of 'b' that exposes 'c'
add_library(b-t SHARED ../src/b.h ../src/b.cpp)
target_link_libraries(b-t a)

# Create an executable that needs 'a' through 'b'
add_executable(c-t ../src/c.cpp)
target_link_libraries(c-t b-t)

# Create another version of 'b' that does not expose 'c'
add_library(b-nt SHARED ../src/b.h ../src/b.cpp)
target_link_libraries(b-nt PRIVATE a)

# Attempting to resolve 'a' through 'b-nt' does not work because it used PRIVATE
# add_executable(c-nt ../src/c.cpp)
# target_link_libraries(c-nt b-nt)

# Create another version of 'c' that resolves it's dependency on 'a' correctly
add_executable(c-nt ../src/c.cpp)
target_link_libraries(c-nt a)

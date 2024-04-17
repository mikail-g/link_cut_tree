cmake_minimum_required (VERSION 3.14)
project (ECE8380_LINKCUT_TREE)

add_library (link_cut link_cut.cpp)
add_executable(test_driver driver.cpp)
target_link_libraries(test_driver PUBLIC link_cut)
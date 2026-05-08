# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/buttontest_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/buttontest_autogen.dir/ParseCache.txt"
  "buttontest_autogen"
  )
endif()

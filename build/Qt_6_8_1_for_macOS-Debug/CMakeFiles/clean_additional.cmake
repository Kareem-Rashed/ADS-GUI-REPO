# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/huffmanguio_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/huffmanguio_autogen.dir/ParseCache.txt"
  "huffmanguio_autogen"
  )
endif()

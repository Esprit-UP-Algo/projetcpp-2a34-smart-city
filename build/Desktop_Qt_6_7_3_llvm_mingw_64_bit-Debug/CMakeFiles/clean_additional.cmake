# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\malek_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\malek_autogen.dir\\ParseCache.txt"
  "malek_autogen"
  )
endif()

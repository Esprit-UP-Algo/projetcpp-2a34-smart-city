# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\projet2eme_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\projet2eme_autogen.dir\\ParseCache.txt"
  "projet2eme_autogen"
  )
endif()

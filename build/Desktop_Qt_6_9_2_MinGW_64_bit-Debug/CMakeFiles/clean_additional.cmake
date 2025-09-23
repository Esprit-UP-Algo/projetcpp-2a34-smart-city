# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\gestion_empolyee_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\gestion_empolyee_autogen.dir\\ParseCache.txt"
  "gestion_empolyee_autogen"
  )
endif()

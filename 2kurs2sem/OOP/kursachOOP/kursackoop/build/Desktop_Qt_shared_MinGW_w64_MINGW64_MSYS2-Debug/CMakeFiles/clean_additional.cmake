# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\kursackoop_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\kursackoop_autogen.dir\\ParseCache.txt"
  "kursackoop_autogen"
  )
endif()

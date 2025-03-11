# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\DataSimulator_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\DataSimulator_autogen.dir\\ParseCache.txt"
  "DataSimulator_autogen"
  )
endif()

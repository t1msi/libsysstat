# https://stackoverflow.com/questions/9680420/looking-for-a-cmake-clean-command-to-clear-up-cmake-output
set(cmake_generated ${CMAKE_BINARY_DIR}/CMakeCache.txt
                    ${CMAKE_BINARY_DIR}/cmake_install.cmake
                    ${CMAKE_BINARY_DIR}/Makefile
                    ${CMAKE_BINARY_DIR}/CMakeFiles
)

set(cmake_generated_sysstat ${CMAKE_BINARY_DIR}/lib/sysstat/CMakeCache.txt
    ${CMAKE_BINARY_DIR}/lib/sysstat/cmake_install.cmake
    ${CMAKE_BINARY_DIR}/lib/sysstat/Makefile
    ${CMAKE_BINARY_DIR}/lib/sysstat/CMakeFiles
)


foreach(file ${cmake_generated})

  if (EXISTS ${file})
     file(REMOVE_RECURSE ${file})
  endif()

endforeach(file)

foreach(file ${cmake_generated_sysstat})

  if (EXISTS ${file})
     file(REMOVE_RECURSE ${file})
  endif()

endforeach(file)

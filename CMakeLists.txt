cmake_minimum_required(VERSION 3.10)
project(stats LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Debug)
set(EXECUTABLE_NAME "stats")

include(CheckIncludeFiles)
add_subdirectory("${CMAKE_SOURCE_DIR}/lib/sysstat/")

# Change path from /src if nedeed, or add more directories
file(GLOB main_sources
        "${CMAKE_SOURCE_DIR}/src/main.cpp"
        )
# file(GLOB baselib_sources
#         "${CMAKE_SOURCE_DIR}/lib/baselib/ProtocolClasses/crsf.cpp"
#         "${CMAKE_SOURCE_DIR}/lib/baselib/ProtocolClasses/protocol.cpp"
#         "${CMAKE_SOURCE_DIR}/lib/baselib/ProtocolClasses/sbus.cpp"
#         "${CMAKE_SOURCE_DIR}/lib/baselib/Calculation/bboxtowgs.cpp"
#         "${CMAKE_SOURCE_DIR}/lib/baselib/Calculation/statistics.cpp"
#         "${CMAKE_SOURCE_DIR}/lib/baselib/Calculation/formulaes.cpp"
#         "${CMAKE_SOURCE_DIR}/lib/baselib/Calculation/regulator.cpp"
#         "${CMAKE_SOURCE_DIR}/lib/baselib/Logic/logic.cpp"
#         "${CMAKE_SOURCE_DIR}/lib/baselib/Common/serialport.cpp"
#         "${CMAKE_SOURCE_DIR}/lib/baselib/Common/logdevice.cpp"
#         "${CMAKE_SOURCE_DIR}/lib/baselib/Common/config.cpp"
#         "${CMAKE_SOURCE_DIR}/lib/baselib/Common/gimbalinteraction.cpp"
#         "${CMAKE_SOURCE_DIR}/lib/baselib/Common/network.cpp"
#        )

add_executable(stats
        ${main_sources}
        # ${baselib_sources}
)

# add_library(stats SHARED
#     ${main_sources}
# )

# install(TARGETS stats DESTINATION .)

# Add more include directories if needed
target_include_directories(stats 
    PUBLIC    "${CMAKE_SOURCE_DIR}/include"
    PUBLIC    "${CMAKE_SOURCE_DIR}/lib/sysstat/include"
    # PUBLIC    "${CMAKE_SOURCE_DIR}/lib/baselib/include"
)

target_link_libraries(stats rdstats rdsensors)

# If you have precompiled headers you can add them like this
# target_precompiled_headers(stats PRIVATE "${CMAKE_SOURCE_DIR}/src/pch.h")

add_custom_target(clean-all
   COMMAND ${CMAKE_BUILD_TOOL} clean
   COMMAND ${CMAKE_COMMAND} -P clean-all.cmake
)

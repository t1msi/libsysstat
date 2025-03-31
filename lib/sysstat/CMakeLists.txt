cmake_minimum_required(VERSION 3.10)

project(stats LANGUAGES C CXX)

set(CMAKE_BUILD_TYPE Debug)

include(CheckIncludeFiles)
include(CheckSourceCompiles)

file(GLOB librdstats_sources
        "${CMAKE_SOURCE_DIR}/src/rd_stats.c"
        "${CMAKE_SOURCE_DIR}/src/count.c"
)

file(GLOB librdsensors_sources
        "${CMAKE_SOURCE_DIR}/src/rd_sensors.c"
)
# CFLAGS += -Wall -Wstrict-prototypes -pipe -O2 -fno-tree-slp-vectorize
# Check for header and function deps for sysstats lib
# CHECK_INCLUDE_FILES(
#     CMAKE_REQUIRED_INCLUDES
#     "ctype.h
#     errno.h
#     libintl.h
#     locale.h
#     linux/sched.h
#     net/if.h
#     regex.h
#     signal.h
#     stdio.h
#     stdint.h
#     fcntl.h
#     inttypes.h
#     libgen.h
#     pwd.h
#     time.h
#     unistd.h
#     pcp/pmapi.h
#     pcp/impl.h
#     sys/time.h
#     sys/statvfs.h
#     sys/types.h
#     sys/file.h
#     sys/ioctl.h
#     sys/param.h
#     sys/stat.h
#     sys/sysmacros.h
#     sys/utsname.h
#     sys/wait.h"
#     LANGUAGE C
# )
# IF (NOT HAVE_SYSSTAT_HEADERS)
#     message(FATAL_ERROR "Can't find some headers for sysstat lib")
# ENDIF (NOT HAVE_SYSSTAT_HEADERS)
#
# check_function_exists(strchr HAVE_SYSSTAT_FUNC_STRCHR)
# IF (NOT HAVE_SYSSTAT_FUNC_STRCHR)
#     message(FATAL_ERROR "Can't find strchr for sysstat lib")
# ENDIF (NOT HAVE_SYSSTAT_FUNC_STRCHR)
# check_function_exists(strcspn HAVE_SYSSTAT_FUNC_STRCSPN)
# IF (NOT HAVE_SYSSTAT_FUNC_STRCSPN)
#     message(FATAL_ERROR "Can't find strcspn for sysstat lib")
# ENDIF (NOT HAVE_SYSSTAT_FUNC_STRCSPN)
# check_function_exists(strspn HAVE_SYSSTAT_FUNC_STRSPN)
# IF (NOT HAVE_SYSSTAT_FUNC_STRSPN)
#     message(FATAL_ERROR "Can't find strspn for sysstat lib")
# ENDIF (NOT HAVE_SYSSTAT_FUNC_STRCSPN)
# check_function_exists(strstr HAVE_SYSSTAT_FUNC_STRSTR)
# IF (NOT HAVE_SYSSTAT_FUNC_STRSTR)
#     message(FATAL_ERROR "Can't find strstr for sysstat lib")
# ENDIF (NOT HAVE_SYSSTAT_FUNC_STRSTR)

# Check for lm_sensors library (temperature/voltage/fan) (bionic "libsensors4-dev", opensuse "libsensors4-devel")

# set(CMAKE_REQUIRED_LIBRARIES "sensors")
check_source_compiles(C "
#include <sensors/sensors.h>
#include <sensors/error.h>

int main() {
    int* tmp;
    sensors_cleanup();
    sensors_get_detected_chips((struct sensors_chip_name*)\"arm2\", tmp);

    return 0;
}
"
HAVE_SENSORS_VAR
)

# Check JSON, SVG support (testing)

# National Language Support (man pages)

# Link Time Optimization

# Data history to keep by sa2 (Daily reports)

# Number of days after which datafiles are compressed (после заверщения полета сразу архивировать)

# Check whether sa directory should be cleaned

# cron setup

# Check whether sadc should collect all possible activities

# Check whether files should only be copied

# Check whether debug mode should be activated

add_library(rdstats STATIC
    ${librdstats_sources}
    "include/common.h"
    "include/rd_sensors.h"
    "include/rd_stats.h"
    "include/systest.h"
)

add_library(rdsensors STATIC
    ${librdsensors_sources}
    "include/common.h"
    "include/rd_sensors.h"
    "include/rd_stats.h"
    "include/systest.h"
)

# target_compile_features(rdstats PUBLIC CMAKE_C_COMPILE_FEATURES)
set_target_properties(rdstats PROPERTIES LINKER_LANGUAGE CXX)
target_compile_definitions(rdstats PUBLIC SOURCE_SADC)

set_target_properties(rdsensors PROPERTIES LINKER_LANGUAGE CXX)
# target_compile_features(rdsensors PUBLIC CMAKE_C_COMPILE_FEATURES)

#DEBUG USE_NLS HAVE_LINUX_SCHED_H
if(HAVE_SENSORS_VAR EQUAL True)
    target_compile_definitions(rdstats   PUBLIC HAVE_SENSORS)
    target_link_libraries(rdstats   PUBLIC sensors)
    target_compile_definitions(rdsensors PUBLIC HAVE_SENSORS)
    target_link_libraries(rdsensors   PUBLIC sensors)
endif(HAVE_SENSORS_VAR EQUAL True)

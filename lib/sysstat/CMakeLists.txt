cmake_minimum_required(VERSION 3.10)
project(sysstat LANGUAGES C)

set(CMAKE_BUILD_TYPE Debug)

include(CheckSourceCompiles)

file(GLOB sadc_sources
        "src/sadc.c"
        "src/activity.c"
        "src/sa_wrap.c"
        "src/common.c"
        # "src/ioconf.c"
        "src/sa_common.c"
        "src/count.c"
        "src/rd_stats.c"
        "src/rd_sensors.c"
)

# sar.c sadf.c sadf_misc.c (заменить на saf.c)
file(GLOB saf_sources
        "src/saf.c"
        "src/saf_misc.c"
    # sar
        "src/activity.c"
        "src/sa_common.c"
        "src/format.c"
        "src/common.c"
        # "src/ioconf.c"
        "src/pr_xstats.c"
        "src/count.c"
        "src/rd_sensors.c"
    # sadf
        "src/rd_stats.c"
        "src/json_stats.c"
)

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

add_executable(sadc
    ${sadc_sources}
    "include/common.h"
    "include/rd_sensors.h"
    "include/rd_stats.h"
    # "include/ioconf.h"
    # "include/sysconfig.h"
    "include/systest.h"
    "include/version.h"
    "include/sa.h"
)

#DEBUG USE_NLS
set_target_properties(sadc PROPERTIES LINKER_LANGUAGE C)
target_compile_definitions(sadc PUBLIC SOURCE_SADC SA_DIR="/var/log/sa")
target_include_directories(sadc PUBLIC "include/")

# only sadc and rd_sensors
if(HAVE_SENSORS_VAR EQUAL True)
    target_compile_definitions(sadc PUBLIC HAVE_SENSORS)
    target_link_libraries(sadc      PUBLIC sensors)
endif(HAVE_SENSORS_VAR EQUAL True)



# sadf.h (заменить на saf.h)
add_executable(saf
    ${saf_sources}
    "include/saf.h"
    # sar
    "include/common.h"
    "include/rd_sensors.h"
    "include/rd_stats.h"
    # "include/ioconf.h"
    # "include/sysconfig.h"
    "include/systest.h"
    "include/version.h"
    "include/sa.h"
    "include/pr_xstats.h"
    # sadf
    "include/json_stats.h"
)

set_target_properties(saf PROPERTIES LINKER_LANGUAGE C)
target_compile_definitions(saf PUBLIC SOURCE_SAF SA_DIR="/var/log/sa" SADC_PATH="/home/refty/src/stats/lib/sysstat/sadc" OBJ)
target_include_directories(saf PUBLIC "include/")


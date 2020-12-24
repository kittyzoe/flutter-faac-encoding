
cmake_minimum_required(VERSION 3.6.0)



# include_directories(../include)


find_library(alog-lib log)


add_definitions(-DHAVE_CONFIG_H)
file(GLOB faac_src
        "*.h"
        "*.cpp"
        "faac1299/*.c"
        "faac1299/*.h"
        "faac1299/*/*.h"
)
include_directories(faac1299/inc/)

add_library(
    faac
    SHARED
    ${faac_src}
    )

get_target_property(_main_compile_options faac COMPILE_OPTIONS)
message(STATUS "have? __SSE2__ ???????????????????????????????????????????? : ${__SSE2__}")

#include_directories(faac1299/inc/)
#find_library(faac-lib faac)
add_library(
    thorffis
    SHARED
    cpp/thor_ffis.cpp

    )

target_link_libraries(thorffis faac ${alog-lib})










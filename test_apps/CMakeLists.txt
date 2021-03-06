cmake_minimum_required(VERSION 3.6)
# 默认编译标准
set(CMAKE_CXX_STANDARD 11)

set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{CMAKE_PREFIX_PATH})  # 这里定位并使用CMAKE_PREFIX_PATH
set(CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} $ENV{QT5_INSTALL_PATH})   # Qt5

# OpenCV
find_package(OpenCV REQUIRED)
find_package(Qt5Core REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# 辅助宏，用于搜索对应目录所有子目录项
MACRO(sub_dir_list result curdir)
    FILE(GLOB children RELATIVE ${curdir} ${curdir}/*)
    SET(dirlist "")
    FOREACH (child ${children})
        IF (IS_DIRECTORY ${curdir}/${child})
            LIST(APPEND dirlist ${child})
        ENDIF ()
    ENDFOREACH ()
    SET(${result} ${dirlist})
ENDMACRO()


get_property(ros_ext_lib GLOBAL PROPERTY ROS_EXTEND)
get_property(UseROS GLOBAL PROPERTY BUILD_WITH_ROS)

message("")
message("----------TEST--------------------------------------------------------")
file(GLOB children RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/*)
set(cpplist "")

foreach (file ${children})
    set(file_path_full ${CMAKE_CURRENT_SOURCE_DIR}/${file})
    get_filename_component(f_extf ${file_path_full} EXT)
    get_filename_component(f_path ${file_path_full} ABSOLUTE)
    get_filename_component(f_base ${file_path_full} NAME_WE)

    if (${f_extf} STREQUAL ".cpp")
        string(FIND ${f_base} "ros" is_ros)

        if (${is_ros} GREATER 0)
            if (${UseROS} STREQUAL "ON")
                add_executable(${f_base} ${file_path_full})
                message(STATUS "TEST ROS   : " ${f_base})
                target_link_libraries(${f_base} cobotsys qwt ${ros_ext_lib})
                install(TARGETS ${f_base} RUNTIME DESTINATION bin)
            endif ()
        else ()
            message(STATUS "TEST APP   : " ${f_base})
            if (WIN32)
                add_executable(${f_base} WIN32 ${file_path_full})
                target_link_libraries(${f_base} Qt5::WinMain cobotsys qwt)
            else ()
                add_executable(${f_base} ${file_path_full})
                target_link_libraries(${f_base} cobotsys qwt)
            endif ()
            install(TARGETS ${f_base} RUNTIME DESTINATION bin)
        endif ()
    endif ()
endforeach ()
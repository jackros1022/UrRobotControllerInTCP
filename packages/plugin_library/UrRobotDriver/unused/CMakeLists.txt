

project(UrRobotDriver)

if (WIN32) # 在Window下这个库不编译
else ()

    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_AUTORCC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)

    # 貌似如果使用AUTOMOC，需要把Qt5Core这个引用进来。这个find_package是必须的。
    find_package(Qt5Core REQUIRED)
    find_package(Qt5Gui REQUIRED)
    find_package(Qt5Widgets REQUIRED) # Fuck, 如果要使用autouic那么必须引用这个Qt5Widgets.

    include_directories(../src)
    include_directories(../ur_adapter)

    file(GLOB ${PROJECT_NAME}_files ur_adapter/* ur_widget_utility/* src/*)

    add_library(${PROJECT_NAME} SHARED ${${PROJECT_NAME}_files})

    target_link_libraries(${PROJECT_NAME} cobotsys)
endif ()


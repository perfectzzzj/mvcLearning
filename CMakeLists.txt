cmake_minimum_required(VERSION 3.16)
project(SimpleTree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_PREFIX_PATH "F:/Qt6/6.10.2/msvc2022_64")


find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

add_executable(SimpleTree
    main.cpp
    treemodel.cpp
    treemodel.h
    treemodel_p.h
)

target_link_libraries(SimpleTree PRIVATE Qt6::Widgets)

# === 关键：windeployqt 放这里 ===
if (WIN32)
    add_custom_command(
        TARGET SimpleTree
        POST_BUILD
        COMMAND "${Qt6_DIR}/../../../bin/windeployqt.exe"
                "$<TARGET_FILE:SimpleTree>"
        COMMENT "Running windeployqt"
    )
endif()
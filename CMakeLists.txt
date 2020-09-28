cmake_minimum_required(VERSION 3.5)

project(Smoke)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set_property( GLOBAL PROPERTY USE_FOLDERS ON)

find_package(Qt5 COMPONENTS Core Quick QuickControls2 REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE SRC_FILES src/*)
file(GLOB_RECURSE QML_FILES qml/*)
file(GLOB_RECURSE SHADER_FILES shaders/*)

# Recreate folder architecture inside Visual Studio (might work for other IDEs as well)
# src
foreach(_source IN ITEMS ${SRC_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()
# qml
foreach(_source IN ITEMS ${QML_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()
# shaders
foreach(_source IN ITEMS ${SHADER_FILES})
    get_filename_component(_source_path "${_source}" PATH)
    string(REPLACE "${CMAKE_SOURCE_DIR}" "" _group_path "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_group_path}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

add_executable(${PROJECT_NAME} ${SRC_FILES}
    ${QML_FILES} ${SHADER_FILES}) # QML_FILES and SHADER_FILES are added here only so that they will appear in the solution explorer of Visual Studio

# Setup working directory in Visual Studio :
set_target_properties(
    ${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

target_compile_definitions(${PROJECT_NAME}
  PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(${PROJECT_NAME}
  PRIVATE Qt5::Core Qt5::Quick Qt5::QuickControls2)

# Copy qml folder after a build
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/qml/
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/qml
)
# Copy shaders folder after a build
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders/
    $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
)
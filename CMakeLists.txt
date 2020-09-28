cmake_minimum_required(VERSION 3.5)

project(Smoke)

#set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt5 COMPONENTS Core Quick QuickControls2 REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

add_executable(${PROJECT_NAME}
  src/main.cpp
  src/App.hpp
  src/App.cpp
  src/FrameBufferObject.hpp
  src/FrameBufferObject.cpp
  src/MessageHandler.hpp
  src/MessageHandler.cpp
  src/Scene.hpp
  src/Scene.cpp
  src/Triangle.hpp
  src/Triangle.cpp
  src/main.qml
)

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
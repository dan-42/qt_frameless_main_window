cmake_minimum_required(VERSION 3.17)


##
## Function to run `windeployqt` to copy all needed qt libs to the executable
##
function(qtf_copy_qt_dependencies_to_folder target executable)
  get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
  get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
  find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

  if(WIN32 AND NOT WINDEPLOYQT_EXECUTABLE)
    message(FATAL_ERROR "function: qtf_copy_qt_dependencies_to_folder() command windeployqt not found")
  endif()

  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(QT_WINDEPLOYQT_BUILD_TYPE_PARAMETER, "--debug")
  else()
    set(QT_WINDEPLOYQT_BUILD_TYPE_PARAMETER, "--release")
  endif()

  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND "${CMAKE_COMMAND}" -E
        env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
            --verbose 0
            ${QT_WINDEPLOYQT_BUILD_TYPE_PARAMETER}
            --no-translations
            --no-quick-import
            --no-webkit2
            --no-compiler-runtime
            --no-system-d3d-compiler
            --no-angle
            --no-opengl-sw
            \"${executable}\"
    COMMENT "Deploying Qt to ${executable}"
  )

endfunction(qtf_copy_qt_dependencies_to_folder)

##
## Function to run `windeployqt` to copy all needed qt libs to the executable
##
function(qtf_copy_qt_dependencies_to_target target)
  qtf_copy_qt_dependencies_to_folder(${target} $<TARGET_FILE:${target}>)
endfunction(qtf_copy_qt_dependencies_to_target)

###############################################################################
###############################################################################
###############################################################################

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)

##
## Set global defines
##
add_compile_definitions(
  _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
  _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
  UNICODE
  _UNICODE
)

set(qtf_VERSION_NUMBER 1.1.0)

project(
  qtf
  VERSION ${qtf_VERSION_NUMBER}
)

find_package(Qt5 COMPONENTS Core Gui Widgets)


file(GLOB_RECURSE QTF_CPP_FILES "${CMAKE_CURRENT_LIST_DIR}/src/qtf/*.cpp")
file(GLOB_RECURSE QTF_HPP_FILES "${CMAKE_CURRENT_LIST_DIR}/src/qtf/*.hpp")


add_library(libqtf STATIC
    ${QTF_CPP_FILES}
)

target_include_directories(
  libqtf
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

set_target_properties(
  libqtf
  PROPERTIES
    AUTOMOC ON
    AUTOUIC OFF
    AUTORCC OFF
)

target_link_libraries(libqtf
  PUBLIC
    Qt5::Core
    Qt5::Gui
    Qt5::Widgets
)

##
##
##
add_executable(qtf_example WIN32)

##
##
##
target_include_directories(
  qtf_example
  PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
)

##
##
##
target_sources(
  qtf_example
  PRIVATE
    "${CMAKE_CURRENT_LIST_DIR}/src/main.cpp"
)

##
## Enable Qt tools
##
find_package(Qt5 COMPONENTS Core Gui Widgets)
set_target_properties(
  qtf_example
  PROPERTIES
    AUTOMOC ON
    AUTOUIC OFF
    AUTORCC OFF
)

##
## Link agains external dependencies
##
target_link_libraries(qtf_example
  PRIVATE
    libqtf
)

##
## Copy dependacies to target
##
qtf_copy_qt_dependencies_to_target(qtf_example)
cmake_minimum_required(VERSION 3.16)

project(Watermeter LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

find_package(unity REQUIRED CONFIG)

add_library(Watermeter OBJECT
    src/ClassAnalogDigitalMeter.h src/ClassAnalogDigitalMeter.cpp)

target_include_directories(Watermeter PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

add_executable(HelperTests tests/test_watermeter_helpers.cpp)
target_link_libraries(HelperTests PRIVATE unity::unity Watermeter)
add_test(NAME HelperTests COMMAND HelperTests)

add_executable(WaterMeterGetValueTests tests/test_watermeter_getvalue.cpp)
target_link_libraries(WaterMeterGetValueTests PRIVATE unity::unity Watermeter)
add_test(NAME WaterMeterGetValueTests COMMAND WaterMeterGetValueTests)

include(GNUInstallDirs)
install(TARGETS Watermeter
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

cmake_minimum_required(VERSION 3.10)

project(geod VERSION 0.0.1)

include(GNUInstallDirs)

message(STATUS ${SYSTEMD_UNIT_PATH})

if(NOT SYSTEMD_UNIT_PATH)
    set(SYSTEMD_UNIT_PATH "/lib/systemd/system/")
endif()

find_package(adcr REQUIRED)
find_package(cppzmq REQUIRED)
find_package(spdlog REQUIRED)
find_package(CLI11 REQUIRED)
find_package(concurrentqueue REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PUBLIC adc::adcr cppzmq concurrentqueue::concurrentqueue spdlog::spdlog CLI11::CLI11)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES configuration/geod.service DESTINATION ${SYSTEMD_UNIT_PATH})

set(TARGET_GEOD_CLIENT geoc)
add_executable(${TARGET_GEOD_CLIENT} examples/geoc/main.cpp)
target_compile_features(${TARGET_GEOD_CLIENT} PRIVATE cxx_std_17)
target_link_libraries(${TARGET_GEOD_CLIENT} PUBLIC cppzmq spdlog::spdlog CLI11::CLI11)

install(TARGETS ${TARGET_GEOD_CLIENT} DESTINATION ${CMAKE_INSTALL_BINDIR})

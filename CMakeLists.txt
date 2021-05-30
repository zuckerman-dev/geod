cmake_minimum_required(VERSION 3.10)

project(geod VERSION 0.0.1)

include(GNUInstallDirs)

set(SYSTEMD_UNIT_PATH "/lib/systemd/system/")

find_package(adc CONFIG REQUIRED)
find_package(adc1256 CONFIG REQUIRED)

find_package(cppzmq REQUIRED)
find_package(spdlog REQUIRED)
find_package(CLI11 REQUIRED)

add_executable(${PROJECT_NAME} src/main.cpp)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(${PROJECT_NAME} PUBLIC adc::ads1256 cppzmq spdlog::spdlog CLI11::CLI11)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES configuration/geod.service DESTINATION ${SYSTEMD_UNIT_PATH})

set(TARGET_GEOD_CLIENT geoc)
add_executable(${TARGET_GEOD_CLIENT} examples/geoc/main.cpp)
target_compile_features(${TARGET_GEOD_CLIENT} PRIVATE cxx_std_17)
target_link_libraries(${TARGET_GEOD_CLIENT} PUBLIC cppzmq spdlog::spdlog CLI11::CLI11)

install(TARGETS ${TARGET_GEOD_CLIENT} DESTINATION ${CMAKE_INSTALL_BINDIR})

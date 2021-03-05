cmake_minimum_required(VERSION 3.10)

project(geod VERSION 0.0.1)

include(GNUInstallDirs)

set(SYSTEMD_UNIT_PATH "/lib/systemd/system/")

add_executable(${PROJECT_NAME} src/main.cpp)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR})
install(FILES configuration/geod.service DESTINATION ${SYSTEMD_UNIT_PATH})
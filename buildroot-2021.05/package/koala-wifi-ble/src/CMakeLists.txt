cmake_minimum_required(VERSION 2.8.7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++17 -pthread")

project(koala-wifi-ble)

include_directories(
    "${PROJECT_SOURCE_DIR}/capicxx"
)

file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/*.cpp
    ${PROJECT_SOURCE_DIR}/capicxx/v1/com/koala/KoalaWifiBleSomeIPDeployment.cpp
    ${PROJECT_SOURCE_DIR}/capicxx/v1/com/koala/KoalaWifiBleSomeIPStubAdapter.cpp
)

find_package(CommonAPI REQUIRED CONFIG)
#link_directories(${CMAKE_PREFIX_PATH}/lib)

add_executable(koala-wifi-ble ${SOURCES})
target_link_libraries(koala-wifi-ble CommonAPI CommonAPI-SomeIP vsomeip3)

install(TARGETS koala-wifi-ble DESTINATION bin)

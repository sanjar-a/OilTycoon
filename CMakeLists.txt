cmake_minimum_required(VERSION 3.20)

project(OilTycoon
    VERSION 0.1.0
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(OilTycoon
    src/main.cpp
    src/Types.cpp
    src/Location.cpp
    src/Reservoir.cpp
    src/Well.cpp
    src/ExplorationProject.cpp
    src/DrillingProject.cpp
    src/TransportationNetwork.cpp
    src/StorageFacility.cpp
    src/Technology.cpp
    src/Company.cpp
    src/Game.cpp
)

target_include_directories(OilTycoon
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

if (MSVC)
    target_compile_options(OilTycoon PRIVATE /W4)
else()
    target_compile_options(OilTycoon PRIVATE -Wall -Wextra -Wpedantic)
endif()
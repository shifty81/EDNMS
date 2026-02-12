cmake_minimum_required(VERSION 3.20)
project(EDNMS VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Engine static library
add_library(EDNMSEngine STATIC
    Engine/Core/Log.cpp
    Engine/Math/Vec3d.cpp
)
target_include_directories(EDNMSEngine PUBLIC ${CMAKE_SOURCE_DIR})

# Simulation static library (does NOT depend on Engine)
add_library(EDNMSSimulation STATIC
    Simulation/World/Chunk.cpp
)
target_include_directories(EDNMSSimulation PUBLIC ${CMAKE_SOURCE_DIR})

# Game executable
add_executable(EDNMS Game/main.cpp)
target_link_libraries(EDNMS PRIVATE EDNMSEngine EDNMSSimulation)

# Tests
enable_testing()
add_executable(EDNMSTests
    Tests/test_main.cpp
    Tests/test_ecs.cpp
    Tests/test_math.cpp
    Tests/test_components.cpp
    Tests/test_chunk_format.cpp
)
target_link_libraries(EDNMSTests PRIVATE EDNMSEngine EDNMSSimulation)
target_include_directories(EDNMSTests PRIVATE ${CMAKE_SOURCE_DIR})
add_test(NAME EDNMSTests COMMAND EDNMSTests)

cmake_minimum_required(VERSION 3.0)
PROJECT("Bubba3D WindExample")

# Set flags
set(LIBRARY_OUTPUT_PATH "${PROJECT_SOURCE_DIR}/build")
set(OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/build")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_STATIC")

# Compile BUBBA-3D
add_subdirectory(Bubba-3D)

# Files to compile
set(GAME_SOURCE_FILES src/main.cpp)

# Define the executable
set(EXECUTABLE_NAME WindExample)
add_executable(${EXECUTABLE_NAME} ${GAME_SOURCE_FILES})

# Link in BUBBA-3D in the executable
target_link_libraries(${EXECUTABLE_NAME} LINK_PUBLIC Bubba3D)
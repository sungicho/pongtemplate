# Minimum required CMake version cmake_minimum_required(VERSION 3.50)
cmake_minimum_required(VERSION 4.0)


# Set the project name (will be the executable name) and language
project(ChessProject CXX)

include(FetchContent)
# create export compile for lsp
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# Set the C++ standard to 17 (recommended for raylib-cpp)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Configure the build directory for the executable (e.g., build/bin/Raylib_App)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

# --- Source Files ---
# Collect all source files (checks 'src/' directory and the root for main.cpp)
file(GLOB_RECURSE PROJECT_SOURCES
    "src/*.cpp"
    "src/*.c"
    "main.cpp"
	"assets/*"
)
# === json ====
# find_package(nholmann_json CONFIG REQUIRED)
find_package(MPI REQUIRED)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)



# Doctest required by nlohmann json
FetchContent_Declare(
  doctest
  GIT_REPOSITORY https://github.com/doctest/doctest.git
  GIT_TAG v2.4.12
)


FetchContent_MakeAvailable(nlohmann_json doctest)
# --- Raylib Dependency Setup ---

# Attempt to find raylib using a configuration file (modern approach)
find_package(raylib CONFIG)

if(NOT TARGET raylib)
    message(STATUS "raylib not found via config, attempting manual search...")
    
    # Manually find the raylib header file and static/shared library
    find_path(RAYLIB_INCLUDE_DIR raylib.h)
    find_library(RAYLIB_LIB raylib)

	if(RAYLIB_INCLUDE_DIR AND RAYLIB_LIB)
        message(STATUS "Found raylib manually: ${RAYLIB_INCLUDE_DIR}")
        
        # Define a reusable Raylib interface target with required link flags
        add_library(raylib INTERFACE)
        target_include_directories(raylib INTERFACE ${RAYLIB_INCLUDE_DIR})
        
        # Add common system libraries required by raylib (OpenGL, math, threads, etc.)
        target_link_libraries(raylib INTERFACE 
            ${RAYLIB_LIB} 
            "-lGL" 
            "-lm" 
            "-lpthread" 
            "-ldl"
            )
        
        # On Linux, link X11 dependencies
        if(CMAKE_SYSTEM_NAME MATCHES "Linux")
            target_link_libraries(raylib INTERFACE X11)
        endif()

    else()
        message(FATAL_ERROR "raylib library or headers not found. Please install raylib (e.g., via a package manager like vcpkg or apt/brew) or manually set include/library paths.")
    endif()
endif()

##add_subdirectory(thirdparty)
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
#add_executable(${PROJECT_NAME} doctest.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE 
    ${json_SOURCE_DIR}/tests/thirdparty/doctest
)

enable_testing()

#add_executable(my_tests tests.cpp)
# Link the executable to the raylib target
target_link_libraries(${PROJECT_NAME} PRIVATE 
		raylib 
		#doctest
		nlohmann_json::nlohmann_json
		doctest::doctest
		MPI::MPI_CXX
)

cmake_minimum_required(VERSION 3.0)
project(G4TestBench VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "..." FORCE)
endif()

option(WITH_G4_VIS "Build with Geant4 Vis drivers" ON)

# Find Geant4
if(WITH_G4_VIS)
    find_package(Geant4 REQUIRED ui_all vis_all)
else()
    find_package(Geant4 REQUIRED ui_all)
endif()
include(${Geant4_USE_FILE})

# Find ProIO
find_package(proio REQUIRED)
include_directories(${proio_INCLUDE_DIRS})

# Find g4pbc
find_package(g4pbc REQUIRED)
include_directories(${g4pbc_INCLUDE_DIRS})

# Set sources and headers
file(GLOB sources ${PROJECT_SOURCE_DIR}/src/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/src/*.hh)
file(GLOB detector_sources ${PROJECT_SOURCE_DIR}/src/detectors/*.cc)
file(GLOB phys_list_sources ${PROJECT_SOURCE_DIR}/src/physicsLists/*.cc)
include_directories(${PROJECT_SOURCE_DIR}/src)

# Create targets
add_executable(g4testbench ${sources} ${detector_sources} ${phys_list_sources})
target_link_libraries(g4testbench ${Geant4_LIBRARIES} proio proio.pb g4pbc::g4pbc)

# Install targets and headers
install(TARGETS g4testbench DESTINATION bin)

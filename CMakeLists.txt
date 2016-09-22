cmake_minimum_required(VERSION 3.1)
project(PhotonMapping)

# The version number.
set (PhotonMapping_VERSION_MAJOR 1)
set (PhotonMapping_VERSION_MINOR 0)

# Sets
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set (CMAKE_CXX_STANDARD 11)

file(GLOB_RECURSE yaml extlibs/yaml-cpp/src/*)
file(GLOB_RECURSE proj_files src/*)

include_directories(extlibs/boost-1.61.0-custom/subcore)
include_directories(extlibs/boost-1.61.0-custom/flattened_smart_ptr)
include_directories(extlibs/yaml-cpp/include)
include_directories(extlibs/eigen-3.2.9)
include_directories(extlibs/libssrckdtree-1.0.8)
include_directories(src)

add_executable(
	photon_mapping
	${yaml}
	${proj_files}
)

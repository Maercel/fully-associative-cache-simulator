# CMakeList.txt : CMake project for cache-simulator, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (
	cache-simulator
	main.cpp
	Cache.cpp MainMemory.cpp MemoryConfig.cpp MemorySystem.cpp
	ReplacementAlgorithms.cpp WriteStrategy.cpp Processor.cpp)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET cache-simulator PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.

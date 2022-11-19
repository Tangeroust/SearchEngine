cmake_minimum_required (VERSION 3.20)

project (search_engine)

set(CMAKE_CXX_STANDARD 17)

set(JSON BuildTests OFF CACHE INTERNAL "")

add_subdirectory(nlohmann_json)
add_executable (search_engine main.cpp 
								ConverterJSON.h 
								ConverterJSON.cpp 
								
								InvertedIndex.h
								InvertedIndex.cpp

								SearchServer.h
								SearchServer.cpp

								config.json 
								requests.json)

target_link_libraries(search_engine PRIVATE nlohmann_json::nlohmann_json)

include(FetchContent)
FetchContent_Declare(
	googletest
	URL
	https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()

target_link_libraries(search_engine PRIVATE gtest_main)
include(GoogleTest)
gtest_discover_tests(search_engine)
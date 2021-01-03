cmake_minimum_required(VERSION 3.10)
project (cppgtfs)

add_library(cppgtfs STATIC
        src/cppgtfs/gtfs/Service.cpp
        src/cppgtfs/Writer.cpp
        src/util/CsvParser.cpp
        src/util/CsvWriter.cpp)

target_include_directories(cppgtfs PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
set_target_properties(cppgtfs PROPERTIES
        CXX_STANDARD 17
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS NO
)

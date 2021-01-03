cmake_minimum_required(VERSION 3.10)
project (cppgtfs)

add_library(cppgtfs STATIC
        src/cppgtfs/gtfs/Service.cpp
        src/cppgtfs/Writer.cpp
        src/util/CsvParser.cpp
        src/util/CsvWriter.cpp)

target_include_directories(cppgtfs PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)
target_compile_features(cppgtfs PRIVATE cxx_std_17)

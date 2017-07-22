cmake_minimum_required (VERSION 2.8.11)

file(GLOB_RECURSE ad_util_SOURCES *.cpp)

add_library(ad_csvparser ${ad_util_SOURCES})
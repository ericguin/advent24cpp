cmake_minimum_required(VERSION 3.20)

set(CMAKE_CXX_STANDARD 23)

project(AdventOfCode)

function(CreateDay day_folder)
    message(STATUS "Adding day " ${day_folder})
    add_executable(${day_folder}_e ${day_folder}/main.cc)
    add_executable(${day_folder}_t ${day_folder}/test.cc)
    target_include_directories(${day_folder}_e PUBLIC ${day_folder} common)
    target_include_directories(${day_folder}_t PUBLIC ${day_folder} common)
    target_compile_definitions(${day_folder}_t PUBLIC TEST)
    file(GLOB ins RELATIVE ${CMAKE_SOURCE_DIR} "${day_folder}/*.txt")
    foreach(in ${ins})
        message(STATUS "Copying over " ${in})
        configure_file("${in}" "${in}" COPYONLY)
    endforeach()
endfunction()

function(BuildDays)
    file(GLOB days RELATIVE ${CMAKE_SOURCE_DIR} "day*/")
    foreach(day ${days})
        CreateDay(${day})
    endforeach()
endfunction()

BuildDays()
cmake_minimum_required(VERSION 3.15)
project(NeuralNetwork CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# neural network main executable
add_executable(nn 0nn.cpp)

# training sample generator
#add_executable(makeTrainingSamples makeTrainingSamples.cpp)

# compiler warnings
if(MSVC)
    target_compile_options(nn PRIVATE /W3 /utf-8)
    #target_compile_options(makeTrainingSamples PRIVATE /W3)
else()
    target_compile_options(nn PRIVATE -Wall -Wextra)
    #target_compile_options(makeTrainingSamples PRIVATE -Wall -Wextra)
endif()

# link math library on non-Windows platforms
if(NOT WIN32)
    target_link_libraries(nn PRIVATE m)
    #target_link_libraries(makeTrainingSamples PRIVATE m)
endif()

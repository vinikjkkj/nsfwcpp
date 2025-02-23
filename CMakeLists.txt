cmake_minimum_required(VERSION 3.21)
project(nsfwcpp CXX)

find_package(Drogon CONFIG REQUIRED)
find_package(OpenCV CONFIG REQUIRED)
find_package(onnxruntime CONFIG REQUIRED)

include_directories("/")

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME}
    PRIVATE 
        Drogon::Drogon
        opencv::opencv
        onnxruntime::onnxruntime
)
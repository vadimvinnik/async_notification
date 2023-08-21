cmake_minimum_required(VERSION 3.25)

project(demo
    LANGUAGES CXX
)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE
        async_notification::async_notification
        Threads::Threads
)


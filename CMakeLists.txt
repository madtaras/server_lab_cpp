cmake_minimum_required(VERSION 3.10)
project(server_lab_cpp)

set(CMAKE_CXX_STANDARD 17)
FIND_PACKAGE( Boost 1.40 COMPONENTS program_options filesystem REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_executable(server server.cpp)
add_executable(client client.cpp)

TARGET_LINK_LIBRARIES(server LINK_PUBLIC ${Boost_LIBRARIES} -lboost_thread-mt)
TARGET_LINK_LIBRARIES(client LINK_PUBLIC ${Boost_LIBRARIES} -lboost_thread-mt)
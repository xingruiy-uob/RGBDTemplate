cmake_minimum_required(VERSION 2.8)
project(ProjectTemplate CXX)

find_package(Pangolin 0.5 REQUIRED)
find_package(OpenCV 3.4 REQUIRED)
find_package(Eigen3 3.3 REQUIRED)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)

add_library(${PROJECT_NAME} SHARED "")

target_sources(${PROJECT_NAME}
PRIVATE
    src/Map.cpp
    src/Frame.cpp
    src/KeyFrame.cpp
    src/Viewer.cpp
    src/System.cpp
    src/Tracking.cpp
)

target_compile_features(${PROJECT_NAME}
PUBLIC
    cxx_std_11
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tools/CameraOpenNI/include
)

target_link_libraries(${PROJECT_NAME}
PUBLIC
    Eigen3::Eigen
    ${OpenCV_LIBRARIES}
    ${Pangolin_LIBRARIES}
)

# Build Examples
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_executable(live_demo ${PROJECT_SOURCE_DIR}/demo/DemoLive.cpp)
target_link_libraries(live_demo 
PRIVATE 
    ${PROJECT_NAME} 
    ${PROJECT_SOURCE_DIR}/tools/CameraOpenNI/lib/libCameraOpenNI.so
)
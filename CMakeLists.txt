cmake_minimum_required(VERSION 2.8.3)
project(ttlidar_node)

set(TTLIDAR_SDK_PATH "./sdk/")

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rosconsole
  sensor_msgs
)


include_directories(
  ${TTLIDAR_SDK_PATH}/include
  ${TTLIDAR_SDK_PATH}/src
  ${TTLIDAR_SDK_PATH}/src/arch
  ${TTLIDAR_SDK_PATH}/src/arch/linux
  ${catkin_INCLUDE_DIRS}
)



file(GLOB TTLIDAR_SDK_SRC 
  "${TTLIDAR_SDK_PATH}/src/*.cpp"
  "${TTLIDAR_SDK_PATH}/src/arch/*.cpp" 
  "${TTLIDAR_SDK_PATH}/src/arch/linux/*.cpp"
)


catkin_package()


add_executable(ttlidar_node src/ttlidar_node.cpp ${TTLIDAR_SDK_SRC})
target_link_libraries(ttlidar_node ${catkin_LIBRARIES} -lrt  -lpthread)

add_executable(subscribe_demo_node src/subscribe_demo_node.cpp)
target_link_libraries(subscribe_demo_node ${catkin_LIBRARIES})

install(TARGETS ttlidar_node subscribe_demo_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(DIRECTORY launch rviz sdk
  DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
  USE_SOURCE_PERMISSIONS
)



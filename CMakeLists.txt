cmake_minimum_required(VERSION 3.0.2)
project(joy_sample)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  geometry_msgs
  std_msgs
  sensor_msgs
  move_base_msgs
  actionlib_msgs
  tf2_ros
  tf2_geometry_msgs

)
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES joy_sample
#  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
)


include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

add_executable(joy_sub_node src/joy_sub_node.cpp)
target_link_libraries(joy_sub_node ${catkin_LIBRARIES})

add_executable(move_base_node src/move_base_node.cpp)
target_link_libraries(move_base_node ${catkin_LIBRARIES})
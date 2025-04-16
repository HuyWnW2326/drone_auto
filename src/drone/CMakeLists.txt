cmake_minimum_required(VERSION 3.8)
project(drone)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)
find_package(px4_msgs REQUIRED)
find_package(std_msgs REQUIRED)

# Create cpp executables 
add_executable(drone_auto_node src/drone_auto.cpp)
ament_target_dependencies(drone_auto_node rclcpp px4_msgs std_msgs)

# Install cpp executables 
install(TARGETS
  drone_auto_node
  DESTINATION lib/${PROJECT_NAME})

ament_package()

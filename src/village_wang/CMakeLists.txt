cmake_minimum_required(VERSION 3.5)
project(village_wang)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(village_interfaces REQUIRED)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()

add_executable(wang2_node src/wang2.cpp)
<<<<<<< HEAD
ament_target_dependencies(wang2_node rclcpp)
install(TARGETS
=======
ament_target_dependencies(wang2_node rclcpp village_interfaces)
install(TARGETS 
>>>>>>> 525b18b9634066c324fb6a9c651524fa9e13056d
  wang2_node
  DESTINATION lib/${PROJECT_NAME}
)

# 将launch、config目录打包进install/share/village_wang/下
install(DIRECTORY
launch
config
DESTINATION share/${PROJECT_NAME}/
)
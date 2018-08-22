cmake_minimum_required(VERSION 3.5)
project(test_library)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(pluginlib REQUIRED)

add_library(test_library SHARED storage.cpp)
install(
  TARGETS test_library
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)

ament_target_dependencies(test_library pluginlib)

pluginlib_export_plugin_description_file(test_library test_plugin.xml)

ament_export_include_directories(include)
ament_export_dependencies(test_library pluginlib)

add_executable(minimaldll main.cpp)
target_link_libraries(minimaldll test_library)
ament_target_dependencies(minimaldll pluginlib)

ament_package()

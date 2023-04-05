# Same controller as my_controller, but with custom interface for communication.

# Create package and dependencies
```
ros2 pkg create --build-type=ament_cmake my_controller2 --dependencies control_msgs controller_interface hardware_interface pluginlib rclcpp rclcpp_lifecycle realtime_tools example_interfaces

```
# Add a header file (.hpp)
Just copy and modify the header from my_controller package.

Remember to modify the header guard and change namespace.

# Add a .cpp source file
Just copy and modify the header from my_controller package.

Remember to modify the content.
# Write a plugin description file
```
touch ~/ros2_control_example/src/my_controller2/my_controller2.xml
```
Modify content with
```
<library path="my_controller2">
  <class name="my_controller2/MyController2"
         type="my_controller2::MyController2" base_class_type="controller_interface::ControllerInterface">
  <description>
    MyController2 ros2_control controller.
  </description>
  </class>
</library>

```

# Modify CMakeLists.txt
## find_package
Delete the line because we no longer use `control_msgs` message package.
```
find_package(control_msgs REQUIRED)
```

Add
```
find_package(std_msgs REQUIRED)
find_package(custom_interfaces REQUIRED)
```
## add_library
```
add_library(
  my_controller2
  SHARED
  src/my_controller2.cpp
)
```

## target_include_directories
```
target_include_directories(
  my_controller2
  PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>
)
```

## ament_target_dependencies
```
ament_target_dependencies(
  my_controller2
  std_msgs
  custom_interfaces
  controller_interface
  hardware_interface
  pluginlib
  rclcpp
  rclcpp_lifecycle
  realtime_tools
)
```

## target_compile_definitions
```
target_compile_definitions(my_controller2 PUBLIC "PLUGINLIB__DISABLE_BOOST_FUNCTIONS")
```

## pluginlib_export_plugin_description_file
```
pluginlib_export_plugin_description_file(
  controller_interface my_controller2.xml)
```

## install
```
install(
  TARGETS
  my_controller2
  RUNTIME DESTINATION bin
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
)

install(
  DIRECTORY include/
  DESTINATION include
)

install(
  DIRECTORY
    config
  DESTINATION
    share/${PROJECT_NAME}/
)
```

## ament_export_
```
ament_export_include_directories(
  include
)
ament_export_libraries(
  my_controller2
)
ament_export_dependencies(
  std_msgs
  custom_interfaces
  controller_interface
  hardware_interface
  pluginlib
  rclcpp
  rclcpp_lifecycle
  realtime_tools
)
```

# Add dependencies into the package.xml file
```
  <depend>std_msgs</depend>
  <depend>custom_interfaces</depend>
  <depend>controller_interface</depend>
  <depend>hardware_interface</depend>
  <depend>pluginlib</depend>
  <depend>rclcpp</depend>
  <depend>rclcpp_lifecycle</depend>
  <depend>realtime_tools</depend>
  <depend>example_interfaces</depend>

  <test_depend>ament_lint_auto</test_depend>
  <test_depend>ament_lint_common</test_depend>
  <test_depend>ament_cmake_gmock</test_depend>
  <test_depend>controller_manager</test_depend>
  <test_depend>hardware_interface</test_depend>
  <test_depend>ros2_control_test_assets</test_depend>
```

# Create a Configuration File for the Controller Manager and Controllers
```
mkdir ~/ros2_control_example/src/my_controller2/config
touch ~/ros2_control_example/src/my_controller2/config/my_controller2_custom.yaml
```
With content:
```
# Controller manager configuration
controller_manager:
  ros__parameters:
    update_rate: 50  # Hz

    # Define a name for controllers that we plan to use
    joint_state_broadcaster:
      type: joint_state_broadcaster/JointStateBroadcaster

    my_controller2:
      type: my_controller2/MyController2

# Properties of the custom controler and definition of joints to use
my_controller2:
  ros__parameters:
    joints:
      - joint1
      - joint2
    interface_name: position
```

# Update the Gazebo plugin configuration parameters
Find the place that handle Gazebo plugin, and assign the new controller yaml path.

In this workspace the file located in:
```
~/ros2_control_example/src/robot_model/description/gazebo_ros2_control.xacro
```
Modify the following part:
```
  <!-- Gazebo's ros2_control plugin -->
  <gazebo>
    <plugin filename="libgazebo_ros2_control.so" name="gazebo_ros2_control">
      <robot_sim_type>gazebo_ros2_control/GazeboSystem</robot_sim_type>
      <parameters>$(find my_controller2)/config/my_controller2_custom.yaml</parameters>
    </plugin>
  </gazebo>
```
